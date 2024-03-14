#include <Servo.h>

//input pins
const byte enableButton = 2;
const byte VROne = A6;

//create servo variables
const int safeAngle = 90;
int angleOne;

//safe variable
bool safe = true;

//create servo objects
Servo servoOne;

//!!KEEP UPDATED WITH SERVO LIST!!
void setup() {
  //attach servos to pins
  servoOne.attach(5);

  //init reset buttom
  pinMode(enableButton, INPUT);
  
  //init serial debugging
  Serial.begin(31250);
}

void loop() {
  //read in angle
  angleOne = AnalogPinReadDeg(VROne);

  //update safety
  safe = SafetyUpdate(enableButton, 10, true, 1);
  
  //if unsafe (run mode), update servo1 angle
  if(!safe){
    Serial.print("run :");
    Serial.println(angleOne);
    UpdateServo(servoOne, angleOne);
  }else{
    Serial.println("safe");
    SafeMode(safeAngle);
  }
  delay(20);

}

int AnalogPinReadDeg(int AnalogPin){
  //read in analog
  int position = analogRead(AnalogPin);

  //scale to deg
  position = map(position, 0, 1023, 0, 180);

  return position;
}

//pull SafetyPin High to return false (run mode)
bool SafetyUpdate(int SafetyPin, int Cycles, bool CurrentSafetyMode, int Ms){
  //filters out false reads by making sure "cycles" in a row all say the same thing
  //default safe mode
  //pull SafetyPin High to return false (run mode)
  
  byte modeReadOne = digitalRead(SafetyPin);
  byte modeReadTwo = digitalRead(SafetyPin);

  //read multiple times
  //if all reads are run mode, return false (run)
  //otherwise return true (safe)
  for(int i = 0; i <= Cycles; i++){
    delay(Ms);
    modeReadTwo = digitalRead(SafetyPin);

    //precheck for 1 cycle
    if((Cycles == 1) && (i == 1)){
      //check both reads are the same
      if(modeReadOne == modeReadTwo){
        //if High, return false (run), else return true (safe)
        if(modeReadTwo == HIGH){
          return false;//true
        }else{
          return true;//safe
        }
      }
    }

    //check all are same
    //if at the end, read and update;
    if(i == Cycles){
      //if last read is same as first, and is also run mode, return false (run)
      //else return true (safe mode)
      if(modeReadOne == modeReadTwo){
        if(modeReadOne == HIGH){
          return false;//run
        }
      }else{
        return true;//safe
      }
    }else{
      //if two modes are different, return safe
      if(modeReadOne != modeReadTwo){
        return true;//safe
      }
    }
  }

  return true; //default return safe
}

void UpdateServo(Servo runningServo, int angle){
  runningServo.write(angle);
}

//!!KEEP UPDATED WITH SERVO LIST!!
void SafeMode(int safeAngle){
  //set default safe behavior
  servoOne.write(safeAngle);
};