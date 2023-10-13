#include <Arduino_LSM6DS3.h>.
//setting up as server
#include <WiFiNINA.h>
char ssid[] = "Ping";
char pass[] = "pingping";
WiFiServer server(5200);

//telemetry bools
bool go_command = 0;

//motor pins
const int Dir1L = 5;
const int Dir2L = 6;
const int Dir3R = 4;
const int Dir4R = 3;

//speed of car
int Speed = 190;

//
bool set = 0;

//IMU
float a_x, a_y, a_z; //acceleration values
float g_x, g_y, g_z;//gyroscope values

double a_x_curr, a_x_grav;
double g_z_curr, degree;

const int g = 9.81;

const char * spacer = "\n";

long next_print_time, start_time, final_time;
long prev_eq_time;
double current_eq_time_seconds, current_eq_time;

void setup() {
  Serial.begin(9600);

  //find ip address for processing
  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip); 
  server.begin();

  //IMU set
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (true); // halt program
  } 

  //setting up motor pins
  pinMode(Dir1L, OUTPUT);
  pinMode(Dir2L, OUTPUT);
  pinMode(Dir3R, OUTPUT);
  pinMode(Dir4R, OUTPUT);

  a_x_curr = 0;
  g_z_curr = 0;

  next_print_time = millis();
  prev_eq_time = millis();
  current_eq_time = millis();
}
void IMU_stuff(){
   
      
    IMU.readAcceleration(a_x, a_y, a_z);
    IMU.readGyroscope(g_x, g_y, g_z);

    //poor attempt at eliminating error
    if(0.02 <= a_x && a_x < 0.05){
      a_x = 0;
      }
    if(-2.1 >= g_z && g_z >= -3){
      g_z = 0;
      }

    //calculate time since last loop in seconds
    current_eq_time = millis() - prev_eq_time;
    prev_eq_time = millis();
    current_eq_time_seconds = current_eq_time / 1000;

    degree = g_z * current_eq_time_seconds;
    
    //adding them to a current so we aren't just displaying the change
    a_x_curr = a_x_curr + a_x;
    g_z_curr = g_z_curr + degree;

    //acceleration comes back as a fraction of g so it's multiplied  
    a_x_grav = a_x_curr * g; //by g here to change it into m/s
  
  }

void print_telemetry(){
  if(go_command){
    server.write("Go, ");
    }
  else{
    server.write("Stop, ");
    }
  }

void buggy_forward(){
  analogWrite(Dir1L, Speed);
  analogWrite(Dir2L, LOW);
  analogWrite(Dir3R, Speed);
  analogWrite(Dir4R, LOW);
  }
void buggy_right(){
  analogWrite(Dir1L, Speed);
  analogWrite(Dir2L, LOW);
  analogWrite(Dir3R, LOW);
  analogWrite(Dir4R, Speed);
  }
void buggy_left(){
  analogWrite(Dir1L, LOW);
  analogWrite(Dir2L, Speed);
  analogWrite(Dir3R, Speed);
  analogWrite(Dir4R, LOW); 
  }
void buggy_stop(){
  analogWrite(Dir1L, LOW);
  analogWrite(Dir2L, LOW);
  analogWrite(Dir3R, LOW);
  analogWrite(Dir4R, LOW);
  }  
  
void loop() {
  WiFiClient client = server.available();

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){   
  if(client){
    while (client.connected()) {
      if (client.available()) {
        
        char command = client.read();
        if(command > 0){
          Serial.write(" Command ");
          Serial.write(command);
          Serial.write("\n");
        
          switch(command){
            case '-': //Status update
                print_telemetry();
              break;
            case 'G': //Go command pressed
              go_command = 1;
              start_time = millis();
              
              break;
            case 'S': //Stop command pressed
              go_command = 0;
              final_time = millis() - start_time;
              final_time = final_time + millis();
              break;
           default :
              server.write("An error has occured");
          }       
        }
        else{
          Serial.write("No command recieved\n");
        }
      }
      IMU_stuff();
      
      if(go_command){
        buggy_forward();
        
        }
        else{
          if(g_z_curr < 180){
          buggy_right();
          }
          else{
            if(final_time > millis()){
              buggy_forward();
              }
              else{
                buggy_stop();
                }
            
            }
          }   

    }
  }
  }
}
