import controlP5.*;
import processing.net.*;
ControlP5 cp2, cp1;
Client myClient;
String data, prev_data;
long next_command_time;

void setup() {
  next_command_time = millis();
  myClient=new Client(this,"192.168.229.2",5200);
  
  size(400, 300);
  
  cp1 = new ControlP5(this);
  cp2 = new ControlP5(this);
  
  cp1.addButton("Go").setValue(0).setPosition(50,50).setSize(300,75);
  cp2.addButton("Stop").setValue(0).setPosition(50,150).setSize(300,75);

}
void draw() {
  if(millis() >= next_command_time) {
    myClient.write("-");
    
    data = myClient.readString();
    
    if(data != null){
      println(data);
    }
  
    
    next_command_time = millis()+1000;
  }
  
  
}

public void Go(int theValue) {
  if (myClient.active()){
    myClient.write("G");
    //System.out.printf("%d Go pressed\n",millis());
    
    //next_command_time = millis()+20;
  }
}
  
public void Stop(int theValue) {
  if (myClient.active()){
    myClient.write("S");
    //System.out.printf("%d Stop pressed\n",millis());
    
    //next_command_time = millis()+20;
  }
}
