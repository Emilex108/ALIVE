//L293D
//Motor A
const int motorPin1  = 11;  // Pin 14 of L293
const int motorPin2  = 10;  // Pin 10 of L293
//Motor B
const int motorPin3  = 9; // Pin  7 of L293
const int motorPin4  = 6;  // Pin  2 of L293
int val = 0;
const int trig1 = 23;
const int echo1 = 22;
boolean autopilot = false;
long duration;
//This will run only one time.
void setup(){
    //Set pins as outputs
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    pinMode(trig1, OUTPUT);
    pinMode(echo1, INPUT);
    Serial.begin(115200);
}


void loop(){

 if (Serial.available()>0 && !autopilot)
  {
    val = Serial.read();
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
    }
  }
  if(autopilot){
    Serial.print(getDistance());
    delay(10);
  }
}

void avancer(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 220);
  analogWrite(motorPin3, 220);
  analogWrite(motorPin4, 0);
}
void reculer(){
  analogWrite(motorPin1, 220);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 220);
}
void arreter(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}
void droite(){
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 170);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 170);
}
void gauche(){
  analogWrite(motorPin1, 170);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 170);
  analogWrite(motorPin4, 0);
}
byte getDistance(){
  // Clears the trigPin
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig1, HIGH);
  delayMicroseconds(30);
  digitalWrite(trig1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo1, HIGH);
  // Calculating the distance
  return duration*0.034/2;
}
