//setting up as server
#include <WiFiNINA.h>
char ssid[] = "eir06675133-2.4G";
char pass[] = "adj3sct9";
WiFiServer server(5200);

//telemetry bools
bool go_command = 0;
bool is_moving = 0;

//PID constants
double Kp = 0.10;
double Ki = 0;
double Kd = 0.02;

//PID initialisation
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;

//pin set up
const int Dir1L = 5;
const int Dir2L = 6;
const int Dir3R = 4;
const int Dir4R = 3;
const int LEYE = A0;
const int REYE = A1;

//soundsensor
const int echo = 8;
const int trig = 9;
int pingTravelTime;
double pingTravelTime_cenimeters;

int change;
int Speed;
int reverse_speed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  //find ip address for processing
  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip); 
  server.begin();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  setPoint = 870;
  previousTime = millis();
}

void loop() {
   WiFiClient client = server.available();
  if(client){
    while (client.connected()) {
      if (client.available()) {
        char command = client.read();
        if(command > 0){
          Serial.write(" Command ");
          Serial.write(command);
          Serial.write(" ");
          Serial.write("\n");
          
          //get pingTravelTime
          get_distance();
          
          //set the input
          input = pingTravelTime;
          
          pingTravelTime_cenimeters = pingTravelTime/58;
          output = pid_function(input);

          //use output to find a value for Speed
          get_speed();
             
          switch(command){
            case '-':
              print_telemetry();
             break;   
            case 'G':
              go_command = 1;
              is_moving = 1;
              break;
           case 'S':
              go_command = 0;
              is_moving = 0;
              break;
           default :
              server.write("An error has occured");
          }
        }
        else{
          Serial.write("No command recieved\n");
        }
      }

      if(go_command){
        buggy_linefollow();
        }
      else{
        analogWrite(Dir1L, LOW);
        analogWrite(Dir2L, LOW);
        analogWrite(Dir3R, LOW);
        analogWrite(Dir4R, LOW);
        }
  }
 }
}

double pid_function(double input){
  currentTime = millis();
  elapsedTime = currentTime - previousTime;
  error = setPoint - input;
  cumError += error * elapsedTime;
  rateError = (error - lastError)/elapsedTime;
  output = Kp * error + Ki * cumError + Kd * rateError;
  lastError = error;
  previousTime = currentTime;
  return output;
  delay(10); //why is this here? can we just get rid of it?
}

void get_distance(){
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pingTravelTime = pulseIn(echo, HIGH);
}

void get_speed(){
  if(output<=0){
  if (output >-255){
    Speed = 255;
  }
  Speed = map(output, -100, 100, 255, -255);
  reverse_speed = 0;
    }
  else{
    Speed = 0;
    reverse_speed = map(output, 100, -100, 255, -255);
  }
}

void print_telemetry(){
 if(go_command){
  server.write("a");//go command
  }
 else{
  server.write("b");//stop command
  }
 if(pingTravelTime < 2000){
  server.write("c");//object detected
  }
 else{
  server.write("d");//object not detected
  }
  server.write(pingTravelTime_cenimeters);
  Serial.print(pingTravelTime_cenimeters);
}

void buggy_linefollow(){
  if(( digitalRead( REYE ) == 0) && ( digitalRead( LEYE ) == 0)){
    analogWrite(Dir1L, Speed);
    analogWrite(Dir2L, LOW);
    analogWrite(Dir3R, Speed);
    analogWrite(Dir4R, LOW);
  }
  else if(( digitalRead( REYE ) == 1) && ( digitalRead( LEYE ) == 0)){
    analogWrite(Dir1L, Speed);
    analogWrite(Dir2L, LOW);
    analogWrite(Dir3R, LOW);
    analogWrite(Dir4R, Speed);
  }
  else if(( digitalRead( REYE ) == 0) && ( digitalRead( LEYE ) == 1)){
    analogWrite(Dir1L, LOW);
    analogWrite(Dir2L, Speed);
    analogWrite(Dir3R, Speed);
    analogWrite(Dir4R, LOW);
  }
}
