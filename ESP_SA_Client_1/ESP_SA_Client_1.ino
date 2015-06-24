#include <DallasTemperature.h>

#include <OneWire.h>



/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "JOKER";
const char* password = "ABCDABCDABCDABCDABCDABCDAB";
const char* host = "192.168.0.110";
char cmdbuf [150];  //buffer used to paste the http string
// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
// Sensors
String text ="ps";
String station = "cs";
enum digi {
  mid, low, high};
digi D1,D2;
//data to communicate
float T1,T2,T3,H1;
int A_1 =11;
int A_2 =22;

// Temperature sensors from Dallas. Data wire is plugged into port 3 on the Arduino
#define ONE_WIRE_BUS 2  // GPIO102 ESP8266
#define TEMPERATURE_PRECISION 9
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS); // initiate bus

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// arrays to hold device addresses
DeviceAddress Thermometer0, Thermometer1;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  for (int k=0; k<2; k++ ){Dallas();}
}

int value = 0;
int counter =0;
void loop() {
  delay(5000); // verstuur temp elke 5 seconden
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 sensors.requestTemperatures();
    T1 = sensors.getTempC(Thermometer0);
    T2 = sensors.getTempC(Thermometer1);
 String  content = "GET /pop_Cordoba.php?"; //D1=1&D2=2&A1=3&A2=4";
  content += "D1=";
  content += String (T1); //String(D1);
  content += "&D2=";
  content += "1"; //String(D2);
  content += "&A1=";
  content += String (counter);
  content += "&A2=";
  content += String (T1); //String(D1);
  content += " HTTP/1.1\r\n" ;
  content += "Host: " ;
  content += String( host);
  content += "\r\n";
  content += "Connection: close\r\n\r\n";
  content.toCharArray(cmdbuf, 150);  //Convert String to char array

 // start conversation
 // This will send the request to the server
  client.print(String (cmdbuf));
 // read the response from the http webserver
for (int num =1; num<2; num++) {
  Response (num);
}
 
  
  Serial.println();
  Serial.println("closing connection");
  counter ++;
  Serial.print("Counter : "); Serial.println(counter);
  
}

// reads one line from the http server response 
void Response (int num)
{
  
   int i=0;
   delay(0);
    while((!client.available()) && (i<200)){
      delay(10);
      i++;
    }
String line = client.readStringUntil('\r');
Serial.print(num);Serial.print("  :");Serial.println(line);
}

void Dallas() {

  //******************DALAS TEMPERATURE ********************************************************
  // Start up the Dallas Temperature IC Control Library library
  sensors.begin();

//  // Start up the library
 // sensors.begin();

  // locate devices on the bus
  Serial.print(sensors.getDeviceCount(), DEC);

  // report parasite power requirements
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(Thermometer0, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(Thermometer1, 1)) Serial.println("Unable to find address for Device 1"); 


  printAddress(Thermometer0);
  printAddress(Thermometer1);

  // set the resolution to 9 bit
  sensors.setResolution(Thermometer0, TEMPERATURE_PRECISION);
  sensors.setResolution(Thermometer1, TEMPERATURE_PRECISION);


  Serial.print(sensors.getResolution(Thermometer0), DEC); 
  // Serial.println();

  // Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(Thermometer1), DEC); 
  //Serial.println();
  sensors.requestTemperatures();
  printData(Thermometer0); 
  printData(Thermometer1);
  T1=sensors.getTempC(Thermometer0);
  Serial.println(T1);
  T2=sensors.getTempC(Thermometer1);
  Serial.println(T2);

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  // Serial.print("Temp C: ");
  Serial.print(tempC);

}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  // Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  //  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  //  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  //  Serial.print(" ");
  printTemperature(deviceAddress);
  //  Serial.println();
}
  




