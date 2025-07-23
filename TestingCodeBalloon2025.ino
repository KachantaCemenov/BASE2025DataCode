
/*
 * Balloon Data Analysis - Version 170627
 * By Zach Wilkerson
 * Summer 2017
 * Edited by Ian Kuhl Summer 2022 to add a MS5607 pressure sensor and its relevent data.
 * 
 * This application is designed to take data input from three
 *   Sparkfun Geiger counters and save them on a per minute basis
 *   to an interfaced SD card.
 *
 * Initialization of count variables for each of the Geiger
 *   counters, for coincidences, and for use in the loop; then
 *   initializing SD-related variables.
 */

#include <SPI.h>
#include <SD.h>
#include <MS5607.h> //Includes the library of the pressure sensor from https://github.com/UravuLabs/MS5607
#include <SparkFun_ADXL345.h>
/*
#include <RTClib.h>

RTC_Millis rtc;
*/

int dataTag;

//The counts are for geiger counters. 
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count12 = 0;
int count13 = 0;


//temperature, pressure and altitude
float temp;
float pres;
float alt;

/*
//Input pin for MQ-9 gas sensor
int smokeInput = A1;
int smokeData;
int maxSmokeData = 0;
float avgSmokeData = 0;
*/

int secondCount = 0;
ADXL345 adxl = ADXL345();
float X_out, Y_out, Z_out;
float roll, pitch, rollF, pitchF = 0;
float maxRoll, avgRoll, maxPitch, avgPitch = 0;

File myFile;
String filename;
MS5607 P_Sens;

// 11 12 13
/*
 * void setup() prepares the pins for data reception and
 *   transmission, and then it readies the arduino for serial
 *   data transfer.
 */
void setup() {

    Serial.begin(115200);      // For serial monitor (USB)
    Serial1.begin(115200);     // UART1: Mega <-> Heltec
    //rtc.begin(DateTime(F(__DATE__), F(__TIME__)));


    pinMode(22, INPUT); Serial.print("Pin mapped as INPUT: "); Serial.println(22);
    pinMode(24, INPUT); Serial.print("Pin mapped as INPUT: "); Serial.println(24);
    pinMode(26, INPUT); Serial.print("Pin mapped as INPUT: "); Serial.println(26);
    
    /*
    pinMode(smokeInput, INPUT);
    Serial.print("Pin mapped as INPUT: "); Serial.println(smokeInput);
    */

    //preparing the SD card:
    if(!SD.begin(10))
    {
        Serial.println("Error - no SD.begin");
    }
    else
    {
        Serial.println("SD card detected.");
    }
    
    //preparing the MS5607 sensor
    if(!P_Sens.begin())
    {
        Serial.println("Error in Communicating with sensor MS5607, check your connections!");
    }else{
    Serial.println("MS5607 initialization successful!");
    }

    //Prepare the ADXL345 accelerometer
    adxl.powerOn();
    adxl.setRangeSetting(2);
    Serial.println("ADXL345 initialized");
   
    
    //filename = "ROOF0703.csv";
    filename = "0715.csv";
    myFile = SD.open(filename, FILE_WRITE);
    
    if(myFile)
    {
        myFile.println("dataTag,Middle,Side,Top,Horizontal,Vertical,Pressure (mBar),Temperature (K),Altitude (m), Average Roll, Max Roll, Average Pitch, Max Pitch");
        myFile.close();
    }
    else
    {
        
        return;
    }
    
}

/*void saveData() is a helping function that processes the data
*   and saves them to an attached SD card.
*   dataTag = foo
*   count1 = a
*   count2 = b
*   count3 = c
*   count12 = d
*   count13 = e
*   pres = p
*   temp = t
*   alt = h
*   Gas Concentration average = j
*   Gas Concentration max = y
    avgRoll = i
    maxRoll = o
    avgPitch = k
    maxPitch = l
*/
void saveData(int foo, int a, int b, int c, int d, int e, float p, float t, float h, float i, float o, float k, float l){
  
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile)
  {
    myFile.println(String(foo)+","+String(a)+","+String(b)+","+String(c)+","+String(d)+","+String(e)+","
        +String(p)+","+String(t)+","+String(h)+","+String(i)+","+String(o)+","+String(k)+","+String(l));
    myFile.close();
    
    Serial.println("Data recorded.");
  }
  else
  {
    Serial.println("Unable to recorded.");
    return;
  }
}

/*
 * void loop() is the main body of the program - it manages the
 *   counters and pressure sensor and packages data as needed for processing via the
 *   saveData() function.
 */
 long lastMinute = 0;
 long lastSecond = 0;
void loop() {
    
    if(millis()-lastMinute >= 60000) 
    {
        //DateTime now = rtc.now();
        //dataTag = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute());
        
        if(P_Sens.readDigitalValue())
        {
            temp = P_Sens.getTemperature();
            temp += 273.15; //Conver Celcius to Kelvin
            pres = P_Sens.getPressure();
            alt = P_Sens.getAltitude();
        }
        else
        {
            Serial.println("Error in reading digital value in sensor!");
        }
        
        
        /*
        temp = 0;
        pres = 0;
        alt = 0;
        */
        
        Serial.println("==================================================");
        Serial.println(dataTag);
        Serial.print("Temperature :  ");
        Serial.print(temp);
        Serial.println(" K");
        Serial.print("Pressure    :  ");
        Serial.print(pres);
        Serial.println(" mBar");
        Serial.print("Altitude    :  ");
        Serial.print(alt);
        Serial.println(" meter");
        
        Serial.print("count1: "); Serial.println(count1);
        Serial.print("count2: "); Serial.println(count2);
        Serial.print("count3: "); Serial.println(count3);
        Serial.print("count12: "); Serial.println(count12);
        Serial.print("count13: "); Serial.println(count13);

        

        /*
        Serial.print("Max gas concentration: "); Serial.println(maxSmokeData);
        //Calculate avgSmokeData
        avgSmokeData = avgSmokeData/secondCount;
        Serial.print("Average gas concentration: "); Serial.println(avgSmokeData);
        */
        
        avgRoll = avgRoll/secondCount;
        avgPitch = avgPitch/secondCount;

        Serial.print("Max Roll: "); Serial.println(maxRoll);
        Serial.print("Max Pitch: "); Serial.println(maxPitch);
        Serial.print("Average roll: "); Serial.println(avgRoll);
        Serial.print("Average pitch: "); Serial.println(avgPitch);



        saveData(dataTag, count1, count2, count3, count12, count13, pres, temp, alt, avgRoll, maxRoll, avgPitch, maxPitch);
        Serial1.println(String(count1)+","+String(count2)+","+String(count3)+","+String(count12)+","+String(count13)+","
            +String(pres)+","+String(temp)+","+String(alt)+","+String(avgRoll)+","+String(maxRoll)+","+String(avgPitch)+","+String(maxPitch));
        count1 = 0; //Middle
        count2 = 0; //Side
        count3 = 0; //Top
        count12 = 0; //Horizontal
        count13 = 0; //Vertical    
        
        /*
        maxSmokeData = 0;
        avgSmokeData = 0;
        */
        secondCount = 0;
        

        maxRoll = 0; avgRoll = 0;
        maxPitch = 0; avgPitch = 0;

        dataTag++;
        lastMinute = millis();
    }
    
    if(millis() - lastSecond >= 1000) //Every second
    {
        /*
        smokeData = analogRead(smokeInput); 
        maxSmokeData = max(smokeData,maxSmokeData);

        //Make avgSmokeData the sum of gas concentration in 30s
        avgSmokeData += smokeData;
        */

        int x,y,z;   
        adxl.readAccel(&x, &y, &z);

        X_out = float(x)/256;
        Y_out = float(y)/256;
        Z_out = float(z)/256;
        pitch = atan(X_out / sqrt( pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
        roll = atan(Y_out / sqrt( pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  
  
        // Low-pass filter
        rollF = 0.94 * rollF + 0.06 * roll;
        pitchF = 0.94 * pitchF + 0.06 * pitch;

        //Compare absolute value because roll, pitch can be negative
        if (abs(maxRoll) < abs(rollF))
        {
            maxRoll = rollF;
        }
        if (abs(maxPitch) < abs(pitchF))
        {
            maxPitch = pitchF;
        }

        avgRoll += rollF; avgPitch += pitchF;

        secondCount++;
        lastSecond = millis();
    }
    
    
    int value1 = digitalRead(22); //middle 
    int value2 = digitalRead(24); //side
    int value3 = digitalRead(26); //top 

    if(value1==1 || value2==1 || value3==1)
    {
        if(value1==1)
        {
            count1++;
            if(value2==1)
            {
                count12++;
            }
            if(value3==1)
            {
                count13++;
            }
        }
        if(value2==1)
        {
            count2++;
        }
        if(value3==1)
        {
            count3++;
        }
    } 
    


}
