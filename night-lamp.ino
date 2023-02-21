#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define ledPin 13 //Board esp8266 pin D7 

#define ledOnTime 10

//set up the LED lights
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, ledPin, NEO_GRB + NEO_KHZ800);

//set up network credentials
const char* ssid     = "laborator-a7";
const char* password = "laborator2022";

ESP8266WebServer server(80); 
     

//set up the motion sensor
const int MOTION_SENSOR_PIN = 12;   // Board esp8266 pin D6 connected to the OUTPUT pin of motion sensor
int motionStateCurrent      = LOW; // current  state of motion sensor's pin
int motionStatePrevious     = LOW; // previous state of motion sensor's pin

bool isLEDOn = false;
String webpage = "";

int lastCommand = 0;

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);         
    strip.show();                          
    delay(wait);                           
  }
}

void setup(){

  webpage = "<!DOCTYPE html><html><body><h1>Welcome to our Night Lamp App!</h1> <h2> Please Select your preferences:</h2><p><a href=\"/RED\"><button class=\"button\">RED</button></a></p><p><a href=\"/GREEN\"><button class=\"button\">GREEN</button></a></p><p><a href=\"/BLUE\"><button class=\"button\">BLUE</button></a></p><p><a href=\"/CYAN\"><button class=\"button\">CYAN</button></a></p><p><a href=\"/MAGENTA\"><button class=\"button\">MAGENTA</button></a></p><p><a href=\"/YELLOW\"><button class=\"button\">YELLOW</button></a></p><p><a href=\"/WHITE\"><button class=\"button\">WHITE</button></a></p><p><a href=\"/START\"><button class=\"button\">START</button></a></p></body></html>";
  Serial.begin(9600);
  //set up strip as having LOW values at first
  strip.begin();
  strip.show();
  pinMode(MOTION_SENSOR_PIN, INPUT);

  delay(1000);
 
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", [](){
    server.send(200, "text/html", webpage);
  });
  
  server.on("/RED", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(255, 0, 0), 50);
    isLEDOn = true;
    delay(1000);
  
  });
  server.on("/GREEN", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(0, 255, 0), 50);
    isLEDOn = true;
    delay(1000);
  });

  server.on("/BLUE", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(0, 0, 255), 50);
    isLEDOn = true;
    delay(1000);
  });

  server.on("/CYAN", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(0, 255, 255), 50);
    isLEDOn = true;
    delay(1000);
  });

  server.on("/MAGENTA", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(255, 0, 255), 50);
    isLEDOn = true;
    delay(1000);
  });

  server.on("/YELLOW", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(255, 255, 0), 50);
    isLEDOn = true; 
    delay(1000);
  });

  server.on("/WHITE", [](){
    server.send(200, "text/html", webpage);
    colorWipe(strip.Color(255, 255, 255), 50);
    isLEDOn = true;
    delay(1000);
  });

  server.on("/START", [](){
    server.send(200, "text/html", webpage);
    if(isLEDOn == true){
      delay(10000);
      colorWipe(strip.Color(0, 0, 0), 50);
      isLEDOn = false;
    }
    else {
      motionStateCurrent = digitalRead(MOTION_SENSOR_PIN);
        if(motionStateCurrent == HIGH){
            Serial.println("Motion detected!");
            for(int i =0; i <= 50;  i+=5){
              colorWipe(strip.Color(255,255,255), i);
            }
            delay(5000);
            colorWipe(strip.Color(0,0,0), 50);
        }
    }
  });
  
  server.begin();
  Serial.println("Web server started!");
}

void loop(){
  server.handleClient();
}
