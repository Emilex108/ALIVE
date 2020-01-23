/*
AutoPilot Script on Arduino for ALIVE Project made by Émile Gagné & Guillaume Blain
*/

#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(2, 3);
bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout


const int motorPin1  = 11;
const int motorPin2  = 10;
const int motorPin3  = 9;
const int motorPin4  = 6;
const int SPEED = 200;
const int ROTATION_SPEED = 100;
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
  Serial.begin(115200);
  wifiSerial.begin(115200);
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CIFSR",responseTime,DEBUG); // get ip address
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80

  sendToUno("Wifi connection is running!",responseTime,DEBUG);
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
      //envoyerDistance();
    }
  }
  //AutoPilot script itself
  if(autopilot){
    sendData((getDistance(0)+0)+"");
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
  analogWrite(motorPin2, ROTATION_SPEED);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, ROTATION_SPEED);
}
void gauche(){
  analogWrite(motorPin1, ROTATION_SPEED);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, ROTATION_SPEED);
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

/*
* Name: sendData
* Description: Function used to send string to tcp client using cipsend
* Params: 
* Returns: void
*/
void sendData(String str){
  String len="";
  len+=str.length();
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
  delay(100);
  sendToWifi("AT+CIPCLOSE=5",responseTime,DEBUG);
}

/*
* Name: readSerialMessage
* Description: Function used to read data from Arduino Serial.
* Params: 
* Returns: The response from the Arduino (if there is a reponse)
*/
String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}


/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}



/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUno
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
