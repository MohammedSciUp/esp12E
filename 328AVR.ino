//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//Arduino code

//DHT11 Lib
#include <DHT.h>

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wiegand.h>
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 30000;  //the value is a number of milliseconds
WIEGAND wg;

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

//Initialisation of DHT11 Sensor
#define DHTPIN 4
DHT dht(DHTPIN, DHT11);
float temp;
float hum;

void setup() {
  Serial.begin(115200);
  wg.begin();
  dht.begin();
  nodemcu.begin(115200);
  delay(1000);
  startMillis= millis();  //initial start time
  Serial.println("Program started");
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  //Obtain Temp and Hum data
  data["hi"] = 33;
  currentMillis = millis(); 
  
  if (currentMillis - startMillis >= period){
    dht11_func();
    data["humidity"] = hum;
    data["temperature"] = temp; 
    startMillis = currentMillis;
  }
  //Assign collected data to JSON Object
  
  if(wg.available())
  {
  data["rfidcode"] = wg.getCode();
  Serial.println("sent");
  Serial.println(wg.getCode());
  
  }
  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();

 
}

void dht11_func() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);

}
