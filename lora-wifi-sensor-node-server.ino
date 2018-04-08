//Imported headers
#include <WiFi.h>
#include <WiFiUdp.h>
#include <U8x8lib.h>

//OLED specific variables
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

//WIFI Variables
//const char* ssid     = "NMHUPublic";
//const char* password = "";

const char* ssid     = "private_24ghz";
const char* password = "password";

//UDP Server variables
WiFiUDP udp;
String datReq;
int packetSize; 
char packetBuffer[512];
unsigned int node_udp_Port = 5000;

//Sensor variables
int echo = 13;
int trig = 12;

void setup(){
  
  Serial.begin(9600);

  //OLED screen ini, and run.
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  //WIFI ini, and connect.
  delay(10);
  wifi_connect();
 
  Serial.print("Port:");
  u8x8.print("Port:");
  
  Serial.println(node_udp_Port);
  u8x8.println(node_udp_Port);
  
  Serial.println("Node Ready");
  u8x8.println("Node Ready");
   
}

void loop(){

  //Get the packet size
  packetSize = udp.parsePacket();

  //Wait until we receive a packet.
  if(packetSize>0){ 

    //Read in the data packet, and cast to string.
    udp.read(packetBuffer, 512);
    String datReq(packetBuffer);

    //Check if packet recieved was our inch read command.
    if (datReq =="COMM IN"){

      long val = sensor_read(1);
      
      //Prepare response packet, and send back to client
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print(val); 
      udp.endPacket();
    }
    //Check if packet recieved was our cm read command.
    else if (datReq =="COMM CM"){

      long val = sensor_read(0);
      
      //Prepare response packet, and send back to client
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print(val); 
      udp.endPacket();
    }
    //If not it was a wrong command
    else {
      
      //Prepare response packet, and send back to client
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print("ERROR: invalid request"); 
      udp.endPacket();
    }
  }

  //Clear the packet buffer
  memset(packetBuffer, 0, 512);
}

void wifi_connect(){

    //SSID notification.
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //Delete old config
    WiFi.disconnect(true);

    //Register event handler
    WiFi.onEvent(WiFiEvent);

    //Attempt WIFI connection
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        u8x8.print(".");
    }
}
void WiFiEvent(WiFiEvent_t event){
   
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
         
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  

          u8x8.print("\n");
          u8x8.print("IP:");
          u8x8.print(WiFi.localIP());
          u8x8.print("\n");
          
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),node_udp_Port);
          break;
          
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost conn");

          u8x8.print("\n");
          u8x8.print("WIFI lost conn");
         
          break;
    }
}
long sensor_read(int flag){

  long duration, val;

  //Send out a ping of 2 or more microseconds
  //Low pulse ensures clean high pulse.
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

  //Read the signal from the ping on echo pin.
  pinMode(echo,INPUT);
  duration = pulseIn(echo, HIGH);

  //Convert the time into distance
  if(flag){
    val = microsecondsToInches(duration);
  }
  else{
    val = microsecondsToCentimeters(duration);
  }
  
  Serial.print(val);
  Serial.print("in ");
  Serial.println();

  delay(100);

  return val;
}
long microsecondsToInches(long microseconds){

  //73.746 microseconds per inch since sound travels at 1130 feet per
  //second. This gives the distance travelled by the ping, outbound
  //and return, so we divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds){
  
  //The speed of sound is 340 m/s or 29 microseconds per centimeter.
  //The ping travels out and back, so to find the distance of the
  //object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

