#include <Wire.h>
#include <PubSubClient.h>
#include "ESP8266WiFi.h"

//WiFi Connection Configuration
const char* ssid = "Honor 8C";
const char* password = "ilovejoboah";

// MQTT Connection Configuration
const char* mqtt_server = "192.168.43.20";
const char* mqtt_username = "user";
const char* mqtt_password = "iL0v3MoonGaYoung";

//I2C address
const int MPU = 0x68;
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
float gForceX, gForceY, gForceZ, rotX, rotY, rotZ;

WiFiClient wifi;
PubSubClient mqtt(wifi);

class Vector3D {
  float x, y, z;

  Vector3D() : x(0), y(0), z(0) {}
}

class MPU6050
{
  unsigned long lastMillis = 0;

  const int address = 0x68;

#define ACCELERATION 0
#define VELOCITY 1
#define POSITION 2

#define X 0
#define Y 1
#define Z 2

  float ACCEL[3][3] = {
    {0, 0, 0}, 
    {0, 0, 0}, 
    {0, 0, 0}
  };

  float GYRO[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
  };

  // Receives and processes the data streamed.
  void getData() {
    unsigned int delta = millis() - lastMillis;

    Wire.beginTransmission(address);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    
    Wire.requestFrom(address, 14, true);
    
    ACCEL_X = ((Wire.read() << 8) | Wire.read()) / 4096.0;
    ACCEL_Y = ((Wire.read() << 8) | Wire.read()) / 4096.0;
    ACCEL_Z = ((Wire.read() << 8) | Wire.read()) / 4096.0;
    GYRO_X  = ((Wire.read() << 8) | Wire.read()) / 32.8;
    GYRO_Y  = ((Wire.read() << 8) | Wire.read()) / 32.8;
    GYRO_Z  = ((Wire.read() << 8) | Wire.read()) / 32.8;

  }
}

void debug() {
  Serial.print(F("ACC: \tX=")); Serial.print(gForceX);
  Serial.print(F("\tY=")); Serial.print(gForceY);
  Serial.print(F("\tZ=")); Serial.println(gForceZ); 

  Serial.print(F("GYR: \tX=")); Serial.print(rotX);
  Serial.print(F("\tY=")); Serial.print(rotY);
  Serial.print(F("\tZ=")); Serial.println(rotZ);
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print(F("Attempting MQTT connection..."));    
    if (mqtt.connect("espclient")) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
    }
  }
}

void setup_wifi() {
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
 
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void setup(){
  Serial.begin(9600);
  
  Wire.begin(0, 2);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  setup_wifi();

  mqtt.setServer(server, 1883);

  Serial.println(server);
}
 
char* init(float val) {
  
  char buff[100];
 
  for (int i = 0; i < 100; i++) {
      dtostrf(val, 4, 2, buff);  //4 is mininum width, 6 is precision
  }
   return buff;
 
}

void dataAcc() {
 
  char mpu6050X[100]= "";   
  strcat(mpu6050X,init(gForceX));
 
  char mpu6050Y[100]= "";   
  strcat(mpu6050Y,init(gForceY));
 
  char mpu6050Z[100]= "";   
  strcat(mpu6050Z,init(gForceZ));
 
  mqtt.publish("AcX/", mpu6050X);
  mqtt.publish("AcY/", mpu6050Y);
  mqtt.publish("AcZ/", mpu6050Z);
}
void dataGy() {
 
  char mpu6050X[100]= "";
  strcat(mpu6050X,init(rotX));
 
  char mpu6050Y[100]= "";
  strcat(mpu6050Y,init(rotY));
 
  char mpu6050Z[100]= "";
  strcat(mpu6050Z,init(rotZ));
  
  mqtt.publish("GyX/", mpu6050X);
  mqtt.publish("GyY/", mpu6050Y);
  mqtt.publish("GyZ/", mpu6050Z);
}
void loop(){
  receiveData();
  processData();
  debug();

  !mqtt.connected()
    ? reconnect() 
    : mqtt.loop();
 
  dataAcc();
  dataGy();
 
  delay(2000);
}

 
