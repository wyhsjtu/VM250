#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * Use arduino mega 2560
 *
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17
//小车移动定义接口
//For board (Driving the car)
int pinI1=9;//I1
int pinI2=8;//I2
int pinI3=7;//I3
int pinI4=6;//I4
//Reloading part + Pulling string
int pinI5=5;
int pinI6=4;
int pinI7=3;
int pinI8=2;
//Electromagnetic
//  Use electric relay
int pinRelay1=1;

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   true
//#define pressures   false
#define rumble      true
//#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

// Reset func 
void (* resetFunc) (void) = 0;

void setup(){
 
  Serial.begin(9600);
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(pinI5,OUTPUT);
  pinMode(pinI6,OUTPUT);
  pinMode(pinI7,OUTPUT);
  pinMode(pinI8,OUTPUT);
  pinMode(pinRelay1,OUTPUT);
  
  delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
  case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
}


void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1){ //skip loop if no controller found
    resetFunc();
  }
  /*
  if (!routine(DELAY_TIME, 0)) { //returns true every 50 milliseconds
    return;
  }
  */
  if(type == 2){ //Guitar Hero Controller
    Serial.println("Not supported");
  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START)){         //will be TRUE as long as button is pressed
  //Do not add functions to this.
  //It connects to the receiver during standby.
      Serial.println("Start is being held");
    }
    if(ps2x.Button(PSB_SELECT)){
      Serial.println("Select is being held");
      }   

    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("PSB_PAD_UP is pressed.\n");
     // Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      digitalWrite(pinI5,HIGH);//Motor NO. 1
      digitalWrite(pinI6,LOW);
  }
  if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("PSB_PAD_DOWN is pressed.\n");
     // Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    digitalWrite(pinI5,LOW);//Motor NO. 1
    digitalWrite(pinI6,HIGH);
    }
  if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.print("PSB_PAD_LEFT is pressed.\n");
      digitalWrite(pinI7,HIGH);//Motor NO. 2
      digitalWrite(pinI8,LOW);
  }
    if(ps2x.Button(PSB_PAD_RIGHT)){
    Serial.print("PSB_PAD_RIGHT is pressed.\n");
      digitalWrite(pinI7,LOW);//Motor NO. 2
      digitalWrite(pinI8,HIGH);
  }
    
    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed, Relay 1 is activated.");
    digitalWrite(pinRelay1,HIGH);
      if(ps2x.Button(PSB_R3)){
        Serial.println("R3 pressed, Relay is deactivated.");
    digitalWrite(pinRelay1,LOW);
    }
  //***************************************
      if(ps2x.Button(PSB_L2)){
        Serial.println("L2 pressed, stop the upper motors.");
        digitalWrite(pinI5,HIGH);//使直流电机(右)刹车
        digitalWrite(pinI6,HIGH);
        digitalWrite(pinI7,HIGH);//使直流电机左刹车
        digitalWrite(pinI8,HIGH);}
      }
  //-----------------------  
      if(ps2x.Button(PSB_R2)){
        Serial.println("R2 pressed");
        Serial.print("stop \n \n");
        digitalWrite(pinI4,HIGH);//使直流电机(右)刹车
        digitalWrite(pinI3,HIGH);
        digitalWrite(pinI1,HIGH);//使直流电机左刹车
        digitalWrite(pinI2,HIGH);}
      if(ps2x.Button(PSB_TRIANGLE)){
        Serial.println("Triangle pressed");
        Serial.print("straight\n \n");
        digitalWrite(pinI4,LOW);//使直流电机（右）逆时针转
        digitalWrite(pinI3,HIGH);
        digitalWrite(pinI1,LOW);//使直流电机（左）顺时针转
        digitalWrite(pinI2,HIGH); }       
    }

    if(ps2x.ButtonPressed(PSB_CIRCLE)){              //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
            Serial.print("right \n \n");
                  digitalWrite(pinI4,HIGH);//使直流电机(右)顺时针转
                  digitalWrite(pinI3,LOW);
                  digitalWrite(pinI1,LOW);//使直流电机(左)顺时针转
                  digitalWrite(pinI2,HIGH);
                  }
          
    if(ps2x.NewButtonState(PSB_CROSS)){              //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
      Serial.print("back\n \n");
                  digitalWrite(pinI4,HIGH);//使直流电机（右）顺时针转
                  digitalWrite(pinI3,LOW);
                  digitalWrite(pinI1,HIGH);//使直流电机（左）逆时针转
                  digitalWrite(pinI2,LOW);
      }
    
    if(ps2x.ButtonReleased(PSB_SQUARE)){              //will be TRUE if button was JUST released
      Serial.println("Square just released");
            Serial.print("left \n \n");
                digitalWrite(pinI4,LOW);//使直流电机(右)逆时针转
                digitalWrite(pinI3,HIGH);
                digitalWrite(pinI1,HIGH);//使直流电机(左)逆时针转
                digitalWrite(pinI2,LOW);
    }     
  }
  /*
  switch (g_state) {
    case STATE::NO_DELAY:
      break;
    case STATE::WAJUE:
      wajue();
      break;
  }
  */


bool routine(const unsigned long int t, const unsigned int i) {
  static unsigned long int timeOfLast[16] = {0}; //this initializes an array to 0
  if (i >= 16) {
    return false;
  }
  unsigned long int timeNow = millis();
  if (timeNow - timeOfLast[i] >= t) {
    timeOfLast[i] = timeNow;
    return true;
  }
  return false;
}
