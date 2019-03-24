/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 The analogWrite() function uses PWM, so if
 you want to change the pin you're using, be
 sure to use another PWM capable pin. On most
 Arduino, the PWM pins are identified with 
 a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

 This example code is in the public domain.
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

int on_cnt = 0;
int cnt = 0;
int cnt2 = 0;
const int blink_brightness = 120;
int btn = 8;
int fade_state = 0;

struct parm
{
  int min;
  int max;
  int inc;
  int val; 
  int brightness; //

  parm(int min_i, int max_i, int inc_i, int val_i, int brightness_i)
  {
    min = min_i; max = max_i; inc = inc_i; val = val_i; brightness = brightness_i;
    if (brightness)
    {
      pinMode(brightness, OUTPUT);
      setPwmFrequency(brightness, 1);

    }
  }

  void fade()
  {
    int tmp = val + inc;
    if ( ( (tmp >= min) and (inc <  0) ) or
         ( (tmp <= max) and (inc > 0) ) )
         {
            if (not brightness)
            {
            Serial.print("blink: ");
            Serial.println(val);
            }
            val = tmp;
            apply();
         }
 
  }

  void apply()
  {
    if (brightness)
    {
        analogWrite(brightness, val);
        Serial.print("bright: ");
        Serial.println(val);

    }
  }
};

parm blink(16,92,1,16,0);
parm bright(10,140,2, 10, 9);
parm* cur_parm;
// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(btn, INPUT);
  pinMode(btn, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Yo");
  cur_parm = &bright;
  cur_parm->apply();

}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  //analogWrite(led, brightness);

  delay(50);
  int sw_state = digitalRead(btn);

  if (not sw_state) // !! 1 when not pressed !!! 
    on_cnt ++;
  
  if (fade_state)
  {
    if (sw_state) // open
    {
      fade_state = 0; 
      Serial.println("Leaving fade mode");
      cur_parm->inc = -cur_parm->inc;
    }
    else
    {
      cur_parm->fade();
    }  
  }
  else //not in fade state
  {
    if (on_cnt > 10) // enter fade state
    {
      Serial.print("Entering fade mode cnt=");
      Serial.println(on_cnt);
      fade_state = 1;
    }
    else if (on_cnt)// not in fade state yet
    {
      if (sw_state) // switch off
      {
        cur_parm = (cur_parm == &blink) ? &bright : &blink;
        Serial.print("switching mode to:");
        Serial.println(cur_parm->brightness);
        cur_parm->apply();
      }
    }

    
  }

  if ( (sw_state) and (on_cnt) )// reset switch on counter
  {
    on_cnt = 0;
    Serial.println("SW release");
  }
  
  cnt = ( cnt < (blink.val >> 3) ) ? cnt + 1 : 0;
  cnt2 = (cnt2 + 1) & 255;
  if ( (cur_parm == &blink) )
  {
      if ((cnt == 0) or (cnt2 > 228) )
        analogWrite(bright.brightness, blink_brightness);
      else
        analogWrite(bright.brightness, 0);              
  }
 
}
