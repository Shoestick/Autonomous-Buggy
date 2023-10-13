import controlP5.*;
import processing.net.*;
ControlP5 cp2, cp1, cp3, cp4, cp5;

controlP5.Button a;
controlP5.Button b;
controlP5.Button c;
controlP5.Button d;

Client myClient;
String pingTravelTime;
int pingTravelTime_int, prev_pingTravelTime;
char data;
long next_command_time;
void setup() {
  
  next_command_time = millis();
  myClient=new Client(this,"192.168.1.3",5200);
  size(800, 550);
  
  ControlFont ButtonFont = new ControlFont(createFont("arial",45));
  ControlFont StatementFont = new ControlFont(createFont("arial",25));
  
  cp1 = new ControlP5(this);
  cp2 = new ControlP5(this);
  cp3 = new ControlP5(this);
  cp4 = new ControlP5(this);
  cp5 = new ControlP5(this);
  
  cp1.addButton("Go").setValue(0).setPosition(50,50).setSize(300,200).setFont(ButtonFont);
  cp2.addButton("Stop").setValue(0).setPosition(50,300).setSize(300,200).setFont(ButtonFont);
  
  cp3.addButton("Command:").setValue(0).setPosition(400,50).setSize(200,50).setFont(StatementFont);
  a = cp3.addButton("Go").setValue(0).setPosition(600,50).setSize(200,50).setFont(StatementFont);
  b = cp3.addButton("Stop").setValue(0).setPosition(600,50).setSize(200,50).setFont(StatementFont);
  
  cp3.addButton("Object:").setValue(0).setPosition(400,125).setSize(200,50).setFont(StatementFont);
  c = cp3.addButton("Detected").setValue(0).setPosition(600,125).setSize(200,50).setFont(StatementFont);
  d = cp3.addButton("Not detected ").setValue(0).setPosition(600,125).setSize(200,50).setFont(StatementFont);
  
  cp3.addButton("Current distance").setValue(0).setPosition(400,200).setSize(450,50).setFont(StatementFont);
  
  cp3.addButton("Setpoint distance: 15cm").setValue(0).setPosition(400,400).setSize(450,50).setFont(StatementFont);
  cp3.addButton("Kp:0.10  Ki:0  Kd:0.02").setValue(0).setPosition(400,450).setSize(450,50).setFont(StatementFont);
  
  
}
void draw(){

  background(0,0,0);
  if(millis() >= next_command_time) {
    myClient.write("-");
    if(myClient.active()){
    //Go command
    data = myClient.readChar();
    if(data == 'a'){
      //go command in effect
      a.show();
      b.hide();
      print("Go command in effect, ");
    }
    else if(data == 'b' ){
      //stop command in effect
      b.show();
      a.hide();
      print("Stop command in effect, ");
    }
    
    //Object detected or not
    data = myClient.readChar();
    if(data == 'c'){
      //object detected
      c.show();
      d.hide();
      print("Object detected, ");
    }
    else if(data == 'd'){
      //object not detected
      d.show();
      c.hide();
      print("Object not detected, ");
    }
    
    pingTravelTime_int = myClient.read();
      if(pingTravelTime_int > 0 && pingTravelTime_int < 90){
        String pingTravelTime = str(pingTravelTime_int);
        textSize(100);
        text(pingTravelTime, 450, 350);
        println(pingTravelTime);
        text("cm", 560, 350);
      }
    
    next_command_time = millis()+250;
      }
    }
  }
public void Go(int theValue) {
  if (myClient.active()){
    myClient.write("G");
    next_command_time = millis()+20;
  }
}
public void Stop(int theValue) {
  if (myClient.active()){
    myClient.write("S");
    next_command_time = millis()+20;
  }
}
