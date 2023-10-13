import processing.net.*;


Client arduino;

import controlP5.*;

ControlP5 control;

PFont button_font;
PFont title_font;
PFont telemetry_font;
PFont print_font;



void setup(){
  
  size(1000,350);
  
 
  arduino = new Client(this,"192.168.1.13",5204);
    
  
  
  control = new ControlP5(this);
  print_font = createFont("arial",18);
  telemetry_font = createFont("arial",25);
  title_font = createFont("arial",50);
  button_font = createFont("arial",40);
  
  control.addButton("GO")
     .setPosition(75,150)
      .setSize(150,100)
        .setFont(button_font);
        
        
  control.addButton("STOP")    
    .setPosition(275,150)
      .setSize(150,100)
        .setFont(button_font);          
  
}


void draw(){
  
  char Case = arduino.readChar();
  background(0,0,0);
  
  fill(255,255,255);
  rect(500,-1.0,1000,351);
  
  textFont(telemetry_font);
  fill(0,0,0);
  text("Onboard Telemetry",510,25);
  
  switch(Case){
    
    case 'd':
             
             textFont(print_font);
             fill(0,0,0);
             text("Stopped for Obstacle at 10cm",510,45);
             println("Stopped for Obstacle at 10cm");
             break;
             
    case 'c':
             textFont(print_font);
             fill(0,0,0);
             text("Cleared Obstacle",510,45);
             println("Cleared");
             break;
             
    case 'l':
             textFont(print_font);
             fill(0,0,0);
             text("Turning Left",510,45);
             println("left");
             break;
             
   case 'r':
             textFont(print_font);
             fill(0,0,0);
             text("Turning Right",510,45);
             println("right");
             break;
             
   default:
             break;
    
  }
  
 
  textFont(title_font);
  fill(250,250,250);
  text("Buggy Control",90,100);
  textFont(title_font);
  
  
}


public void GO(){
  if (arduino.active()){
    arduino.write("g");
  }  
}

public void STOP(){
    if (arduino.active()){
    arduino.write("s");
  }  
}
