/*
AutoPilot Script on Arduino for ALIVE Project made by Émile Gagné & Guillaume Blain
*/
//librarie for the lcd display
#include <LiquidCrystal.h>
//Libraries for the gyroscope (angle)
#include <MPU6050_tockn.h>
#include <Wire.h>
//setting the gyroscope
MPU6050 mpu6050(Wire);
LiquidCrystal lcd(12, 13, 5, 4, 3, 2);
//Setting the Arduino pins for the motors
const int motorPin1  = 11;
const int motorPin2  = 10;
const int motorPin3  = 9;
const int motorPin4  = 6;
int SPEED = 110;
const int ROTATION_SPEED = 100;
const int ROTATION_FORCE = 65;
//Sensors related settings
const int nbSensors = 3;
const int pins[nbSensors*2] = {23,22,38,39,37,36};
//This is remotely enabled by bluetooth to active the autopilot script
boolean autopilot = false;
//Those are the detections limits in centimeters (It will detect an object if closer to 15 cm) The detectionB is meant to debug curved or angular objects
const int detection = 30;
const int detectionAvant = 10;
const int detectionB = 400;
//Definition of the distances of the captors (TODO Replace by Array to be more flexible)
int distAvant,distGauche,distDroite;
//This constant is quite weird, it has to do with the Unicode translation the bluetooth does to the bytes our program sends/receives
const int DEBUG = 48;
//Various global variables used throughout the code
long duration;
int val = 0;
boolean d,a,g;
long b = 0;
int angle;
int anglePositive = 1;
int nbDemiTour = 0;
boolean isRolling = true;


void setup(){
  //Set pins as outputs or inputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], INPUT);
  pinMode(pins[2], OUTPUT);
  pinMode(pins[3], INPUT);
  pinMode(pins[4], OUTPUT);
  pinMode(pins[5], INPUT);
  //Serial on 115200 is for the bluetooth (TODO Try putting it back to 9600 or around, to avoid Unicode problem)
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
   lcd.print("calculating...");
   //Wire and mpu are for the gyroscope (angle)
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  lcd.clear();
  lcd.print("Ready to go !");
}


void loop(){
  //This makes sure the data of the gyroscope is up to date
  nbDemiTour = 0;
  mpu6050.update(); 
  angle = (int) mpu6050.getGyroAngleX();
  if(angle < 0){
    angle = -angle;
    anglePositive = 0;
  }else{
    anglePositive = 1;
  }
  if(angle > 180){
    nbDemiTour = angle/180;
    angle = angle%180;
  }
 
   //these methods are mostly for debugging, they add a lot of delay
   //do not use two at the same time
   
  // displayCollision();
  // displayAccel();
  // displayAngle();
  
  //Checks to see if there are any commands over BT available and that Autopilot hasn't been set ON (TODO Check to replace with Switch)
  if (Serial.available()>0) {
    val = Serial.read();
    //1 = Forward, 2 = Right, 3 = Backwards, 4 = Left, 0 = Stop, 5 = Autopilot ON, 6 = Updates Distance Panel
    if(val == 1){
      avancer();
    }else if(val == 2){
      droite();
    }else if(val == 12){
      avancerDroite();
    }else if(val == 32){
      reculerDroite();
    }else if(val == 3){
      reculer();
    }else if(val == 4){
      gauche();
    }else if(val == 14){
      avancerGauche();
    }else if(val == 34){
      reculerGauche();
    }else if(val == 0){
      arreter();
    }else if(val == 100){
      Serial.write(getDistance(0));
    }else if(val == 101){
      Serial.write(getDistance(1));
    }else if(val == 102){
      Serial.write(getDistance(2));
    }else if(val == 103){
     Serial.write(anglePositive);
    }else if(val == 104){
     Serial.write(angle);
    }else if(val == 105){
     Serial.write(nbDemiTour);
    }else if(val == 106){
     if(isRolling){
      Serial.write(1);
     }else{
      Serial.write(0);
     }
    }else if(val == 33){
      if(Serial.available() > 0){
        SPEED  = Serial.read();
      }
    }else if(val == 5){
      autopilot = true;
    }else if(val == 6){
      autopilot = false;
    }else if(val == 8){
      delay(100);
      val = Serial.read();
      goToAngle(val);
    }
  }


  if(autopilot){
    d = collisionDroite();
    a = collisionAvant();
    g = collisionGauche();
    if(d && a && g){
      reculer();
    }else if(g && a && !d){
     avancerDroite();
    }else if(!g && a && d){
       avancerGauche();
    }else if(g && !a && d){
      avancer();
    }else if(!g && !a && d){
      avancerGauche();
    }else if (g && !a && !d){
     avancerDroite();
    }else if(!g && !a && !d){
      avancer();
    }else if(!g && a && !d){
      distDroite = getDistance(2);
      distGauche = getDistance(1);
      if(distDroite < distGauche){
        avancerGauche();
      }else if(distGauche < distDroite){
        avancerDroite();
      }else{
        reculer();
      }
    }
  }
}

void avancer(){
  isRolling = true;
  analogWrite(motorPin1, SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, SPEED);
}
void avancerDroite(){
   analogWrite(motorPin1, SPEED - ROTATION_FORCE);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, SPEED);
}
void avancerGauche(){
   analogWrite(motorPin1, SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, SPEED - ROTATION_FORCE);
}
void reculer(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, SPEED);
  analogWrite(motorPin3, SPEED);
  analogWrite(motorPin4, 0);
}
void reculerDroite(){
   analogWrite(motorPin1, 0);
  analogWrite(motorPin2, SPEED - ROTATION_FORCE);
  analogWrite(motorPin3, SPEED);
  analogWrite(motorPin4, 0);
}
void reculerGauche(){
   analogWrite(motorPin1, 0);
  analogWrite(motorPin2, SPEED);
  analogWrite(motorPin3, SPEED - ROTATION_FORCE);
  analogWrite(motorPin4, 0);
}
void arreter(){
  isRolling = false;
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}
void droite(){
  isRolling = false;
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, ROTATION_SPEED);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, ROTATION_SPEED);
}
void gauche(){
  isRolling = false;
  analogWrite(motorPin1, ROTATION_SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, ROTATION_SPEED);
  analogWrite(motorPin4, 0);
}
boolean collisionDroite(){
  distDroite = getDistance(2);
  if(distDroite < detection){
    return true;
  }else{
    return false;
  }
}
boolean collisionAvant(){
  distAvant = getDistance(0);
  if(distAvant < detectionAvant){
    return true;
  }else{
    return false;
  }
}
boolean collisionGauche(){
  distGauche = getDistance(1);
  if(distGauche < detection){
    return true;
  }else{
    return false;
  }
}
//Returns the distance of an ultrasound detector in front of the car

byte getDistance(int i){
  i = i*2;
  // Clears the trigPin
  digitalWrite(pins[i], LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(pins[i], HIGH);
  delayMicroseconds(30);
  digitalWrite(pins[i], LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(pins[i+1], HIGH);
  // Calculating the distance
  return duration*0.034/2;
}
void goToAngle(double angle){
  for(int i =0 ; i<5 ; i++){
    delay(10);
   mpu6050.update();
  double angleCurrent = mpu6050.getGyroAngleX();
  double ecart = 0.5;
  while(!(angleCurrent-ecart <= angle && angle <= angleCurrent+ecart)){
    mpu6050.update();
    angleCurrent = mpu6050.getGyroAngleX();
    if(angleCurrent >= angle){
      droite();
      lcd.setCursor(0, 1);
    }
    if(angleCurrent <= angle){
       gauche();
      lcd.setCursor(0, 1);
    }
  }
  arreter();
  }
}

//**************LCD DISPLAY******************//
void displayCollision(){
   lcd.clear();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(1, 0);
  // print the number of seconds since reset:
  lcd.print("detecion : ");
  String toDisplay = "";
  if(collisionDroite()){
     toDisplay += "OO";
  } else {
    toDisplay += "  ";
  }
  if(collisionAvant()){
     toDisplay += "     OO";
  } else {
    toDisplay += "       ";
  }
   if(collisionGauche()){
     toDisplay += "     OO";
  } else {
    toDisplay += "       ";
  }
  lcd.setCursor(0, 1);
   lcd.print(toDisplay);
  delay(50);
}

void displayAngle(){
   mpu6050.update();
   lcd.clear();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  lcd.print("Angle : ");
  lcd.print(mpu6050.getGyroAngleX());
  delay(50);
}

void displayAccel(){
   mpu6050.update();
   lcd.clear();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  lcd.print("Accel : ");
  lcd.print(mpu6050.getAccZ());
  delay(300);
}
//**************LCD DISPLAY******************//
