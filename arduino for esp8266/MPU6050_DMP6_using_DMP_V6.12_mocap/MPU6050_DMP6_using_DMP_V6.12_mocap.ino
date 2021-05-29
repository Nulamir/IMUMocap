
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps_V6_12.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

//wifi
#include <ESP8266WiFi.h>
#define WIFI_SSID "MOCAP"
#define WIFI_PASS "*****"


// UDP
#include <WiFiUdp.h>
#define SERVER "192.168.6.129"
#define SERVER_PORT 5555
WiFiUDP UDP;

#define LOCAL_PORT 1025
// sensor settings
#define SENSOR_ID 14


#include <TimeLib.h>
time_t t = now();

unsigned long timing;

MPU6050 mpu;
#define OUTPUT_READABLE_QUATERNION

#define INTERRUPT_PIN D5  // use pin 2 on Arduino Uno & most boards
#define LED_PIN D4 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 gy;         // [x, y, z]            gyro sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void ICACHE_RAM_ATTR dmpDataReady() {
  mpuInterrupt = true;
}


char packetBuffer[255];

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin(sda, scl);
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // wait for ready
  Serial.println(F("\nSend any character to begin DMP programming and demo: "));
  //while (Serial.available() && Serial.read()); // empty buffer
  //while (!Serial.available());                 // wait for data
  //while (Serial.available() && Serial.read()); // empty buffer again

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(-1088);
  mpu.setYAccelOffset(-725);
  mpu.setZAccelOffset(946);
  mpu.setXGyroOffset(-57);
  mpu.setYGyroOffset(-105);
  mpu.setZGyroOffset(-23);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
  //  mpu.CalibrateAccel(6);
  //  mpu.CalibrateGyro(6);
  //  Serial.println();
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);


  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
   
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  UDP.begin(LOCAL_PORT);
  
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
int32_t data[6];
int32_t quat_i32[4];
uint8_t* ptrByte;

void loop() {

  if (millis() - timing > 29){

  int packetSize = UDP.parsePacket();

  if (packetSize) {

    int len = UDP.read(packetBuffer, 255);

    if (len > 0) {
      Serial.println(packetBuffer);
      if (*packetBuffer == 82) {
          Serial.println(packetBuffer);
          ESP.restart();
        }
    }

  }

  
  // if programming failed, don't try to do anything
  if (!dmpReady) return;
  // read a packet from FIFO
  //ждем прерывание - готовность отдать буфер
  while (!mpuInterrupt)
  {
   ESP.wdtFeed();
   };
  //выставляем флаг в ложь 
  mpuInterrupt = false;
  
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
   // send sensor data to server

#ifdef OUTPUT_READABLE_QUATERNION   
   mpu.dmpGetQuaternion(&quat_i32[0], fifoBuffer);
   data[0] = SENSOR_ID;
   data[1] = quat_i32[0];
   data[2] = quat_i32[1];
   data[3] = quat_i32[2];
   data[4] = quat_i32[3];
   t = now();
   data[5] = t;
 //  Serial.println(millis());

   UDP.beginPacket(SERVER,SERVER_PORT);
   UDP.write( (uint8_t*) data ,6 * sizeof(int32_t));
   UDP.endPacket();
#endif


    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    timing = millis();
  }

  }
}
