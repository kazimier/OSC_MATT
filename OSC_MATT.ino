#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>



const char* ssid = "Labyrinthe"; // ssid
const char* password = "labyrinthe25";// password
IPAddress ip(192, 168, 10, 4 ); //set static ip
IPAddress gateway(192, 168, 10, 1); //set getteway
IPAddress subnet(255, 255, 255, 0);//set subnet
 

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress DestIp(192,168,10,2);        // remote IP of the destination PC
const unsigned int DestPort = 9001;          // remote port 
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)


int PressurePin = 5;               // choose the input pin (for PIR sensor)
int pressureState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status




void setup() 
{
  pinMode(PressurePin, INPUT);     // declare sensor as input
  Serial.begin(115200);


Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
}





 
void loop(){
  val = digitalRead(PressurePin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pressureState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pressureState = HIGH;
     // pirState = 1;
      OSCMessage msg("/1/PressureMatt/1");
        msg.add(pressureState);
       Udp.beginPacket(DestIp, DestPort);
        msg.send(Udp);
       Udp.endPacket();
       msg.empty();
       delay(5);
    }
  } else {

    if (pressureState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pressureState = LOW;
    //  pirState = 0;
        OSCMessage msg("/1/PressureMatt/1");
         msg.add(pressureState);
       Udp.beginPacket(DestIp, DestPort);
        msg.send(Udp);
       Udp.endPacket();
       msg.empty();
       delay(5);;
      
    }
  }
}
