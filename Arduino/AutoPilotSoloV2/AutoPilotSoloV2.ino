/*
AutoPilot Script on Arduino for ALIVE Project made by Émile Gagné & Guillaume Blain
*/
//Setting the Arduino pins for the motors
const int motorPin1  = 11;
const int motorPin2  = 10;
const int motorPin3  = 9;
const int motorPin4  = 6;
const int SPEED = 140;
//Sensors related settings
const int nbSensors = 3;
const int pins[nbSensors*2] = {23,22,38,39,37,36};
//This is remotely enabled by bluetooth to active the autopilot script
boolean autopilot = false;
//Those are the detections limits in centimeters (It will detect an object if closer to 15 cm) The detectionB is meant to debug curved or angular objects
const int detection = 10;
const int detectionAvant = 8;
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
}


void loop(){
  //This makes sure the data of the gyroscope is up to date
  //Checks to see if there are any commands over BT available and that Autopilot hasn't been set ON (TODO Check to replace with Switch)
  if (Serial.available()>0 && !autopilot) {
    val = Serial.read();
    //1 = Forward, 2 = Right, 3 = Backwards, 4 = Left, 0 = Stop, 5 = Autopilot ON, 6 = Updates Distance Panel
    if(val == 1){
      avancer();
    }else if(val == 2){
      droite();
    }else if(val == 3){
      reculer();
    }else if(val == 4){
      gauche();
    }else if(val == 0){
      arreter();
    }else if(val == 5){
      autopilot = true;
    }else if(val == 6){
      envoyerDistance();
    }
  }
  //AutoPilot script itself
  if(autopilot){
    d = collisionDroite();
    a = collisionAvant();
    g = collisionGauche();
    if(d && a && g){
      reculer();
    }else if(g && a && !d){
      droite();
    }else if(!g && a && d){
      gauche();
    }else if(g && !a && d){
      avancer();
    }else if(!g && !a && d){
      gauche();
    }else if (g && !a && !d){
      droite();
    }else if(!g && !a && !d){
      avancer();
    }else if(!g && a && !d){
      distDroite = getDistance(2);
      distGauche = getDistance(1);
      if(distDroite < distGauche){
        gauche();
      }else if(distGauche < distDroite){
        droite();
      }else{
        reculer();
      }
    }
  }
}

void avancer(){
  analogWrite(motorPin1, SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, SPEED);
}
void reculer(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, SPEED);
  analogWrite(motorPin3, SPEED);
  analogWrite(motorPin4, 0);
}
void arreter(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}
void droite(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, SPEED);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, SPEED);
}
void gauche(){
  analogWrite(motorPin1, SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, SPEED);
  analogWrite(motorPin4, 0);
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

//Updates the Distance Panel of the Java Application by BT

void envoyerDistance(){
  Serial.write(getDistance(0));
  Serial.write('X');
  Serial.write(getDistance(1));
  Serial.write('X');
  Serial.write(getDistance(2));
  Serial.write('X');
}
