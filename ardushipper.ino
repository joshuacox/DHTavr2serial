#include "DHT.h"
//  #include "Time.h"
#include <Time.h>  
#include <Wire.h>  
//#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

const int VAL_PROBE1 = 0; // Analog pin 0

const int VAL_PROBE2 = 1; // Analog pin 1
const int VAL_PROBE3 = 2; // Analog pin 2
const int VAL_PROBE4 = 3; // Analog pin 3
const int MOISTURE_LEVEL = 250; // the value after the LED goes ON

void LedState(int state) {
    digitalWrite(13, state);
}
unsigned long nextInterval = 4;
int sampleInterval = 4;
int incomingByte = 0;

DHT dht;

/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

//#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;

void setup()
{ 
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  setTime(0);
  //setSyncProvider(RTC.get);   // the function to get the time from the RTC
  //if(timeStatus()!= timeSet) {
   //  Serial.println("Unable to sync with the RTC");
  //}
  //else {
  //   Serial.println("RTC has set the system time");
  //}

  //Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  //pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  //if (!SD.begin(chipSelect)) {
   // Serial.println("Card failed, or not present");
    // don't do anything more:
   // return;
  //}
  //Serial.println("card initialized.");
  
  //Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F) v1.1");

  dht.setup(2); // data pin 2

  String initdataString = "";
  initdataString += "SoilMoisture1"; 
  initdataString += ",SoilMoisture2";
  initdataString += ",SoilMoisture3";
  initdataString += ",SoilMoisture4";
  initdataString += ",Humidity"; 
  initdataString += ",C"; 
  initdataString += ",timeStamp"; 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  //if (dataFile) {
  //  dataFile.println(initdataString);
   // dataFile.close();
    // print to the serial port too:
   // Serial.println(initdataString);
 // }
  // if the file isn't open, pop up an error:
  //else {
  //  Serial.println("error opening datalog.txt");
  //}
}

void loop()
{
  if(sampleInterval < 2) {
    delay(dht.getMinimumSamplingPeriod());
  }
  time_t t = now();
  if( t > nextInterval)
  {
      if (timeStatus() == timeSet) {
        //digitalClockDisplay();
      } else {
        Serial.println("The time has not been set.  Please run the Time");
        Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
        Serial.println();
        //delay(4000);
      }
      //delay(1000);
     
      nextInterval = t + sampleInterval;
      float humidity = dht.getHumidity();
      float temperature = dht.getTemperature();
      int moisture1 = analogRead(VAL_PROBE1);
      int moisture2 = analogRead(VAL_PROBE2);
      int moisture3 = analogRead(VAL_PROBE3);
      int moisture4 = analogRead(VAL_PROBE4);


        if(moisture1 < MOISTURE_LEVEL) {
            LedState(HIGH);
        } else   {
            LedState(LOW);
        }
         // make a string for assembling the data to log:
      String dataString = "";
      //if (Serial.available() > 0) {
                // read the incoming byte:
          //      incomingByte = Serial.read();

                // say what you got:
                //Serial.print("I received: ");
                //Serial.println(incomingByte, DEC);
                //sendSerialData();  
      Serial.print("DHT1122-DHTstatus ");
      Serial.print(dht.getStatusString());
      Serial.println(" ");
      Serial.print("DHT1122-Moisture ");
      Serial.print(" Moisture1 ");
      Serial.print(moisture1);
      Serial.print(", Moisture2 ");
      Serial.print(moisture2);
      Serial.print(", Moisture3 ");
      Serial.print(moisture3);
      Serial.print(", Moisture4 ");
      Serial.print(moisture4);
      Serial.println(" ");
      Serial.print("DHT1122-Humidity ");
      Serial.print(humidity, 1);
      Serial.println(" ");
      Serial.print("DHT1122-Celsius ");
      Serial.print(temperature, 1);
      Serial.println(" ");
      Serial.print("DHT1122-Fahrenheit ");
      Serial.print(dht.toFahrenheit(temperature), 1);
      Serial.println(" ");
      Serial.println("3478-ENDTRANSMISSION");
       // }
      // read three sensors and append to the string:
      /* for (int analogPin = 0; analogPin < 3; analogPin++) {
        int sensor = analogRead(analogPin);
        dataString += String(sensor);
        if (analogPin < 2) {
          dataString += ","; 
        }
      } */
      dataString += String(int((10 * moisture1)));
          dataString += ",";
      dataString += String(int((10 * moisture2)));
          dataString += ",";
      dataString += String(int((10 * moisture3)));
          dataString += ",";
      dataString += String(int((10 * moisture4)));
          dataString += ",";
      dataString += String(int((10 * humidity)));
          dataString += ",";
      dataString += String(int((10 * temperature)));
          dataString += ",";
      dataString += String(t);

      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      //File dataFile = SD.open("datalog.txt", FILE_WRITE);

      // if the file is available, write to it:
      //if (dataFile) {
        //dataFile.println(dataString);
        //dataFile.close();
        // print to the serial port too:
        //Serial.println(dataString);
      //}
      // if the file isn't open, pop up an error:
      //else {
       // Serial.println("error opening datalog.txt");
      //}
      //Serial.println(dataString);
  }
}

void sendSerialData(){
}
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits){ // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ;
    Serial.print(c);
    if( c == TIME_HEADER ) {
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){
        c = Serial.read();
        if( c >= '0' && c <= '9'){
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
        }
      }
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }
  }
}


