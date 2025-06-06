
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

int dataTag = 0;

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

//Input pin for MQ-9 gas sensor
int smokeInput = A1;
int smokeData;
int smokeDataCount = 0;
int maxSmokeData = 0;
float avgSmokeData = 0;

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

    Serial.begin(9600); //9600 can be adjusted as needed

    /* //Omit this because there are no Geiger counters present
    for(int g=28; g<34; g+=2)
    {
        pinMode(g, INPUT);
        Serial.print("Pin mapped as INPUT: "); Serial.println(g);
    }
    */

    pinMode(smokeInput, INPUT);
    Serial.print("Pin mapped as INPUT: "); Serial.println(smokeInput);

    //preparing the SD card:
    if(!SD.begin(8))
    {
        Serial.println("Error - no SD.begin");
        return;
    }
    else
    {
        Serial.println("SD card detected.");
    }
    
    //preparing the MS5607 sensor
    if(!P_Sens.begin())
    {
        Serial.println("Error in Communicating with sensor, check your connections!");
    }else{
    Serial.println("MS5607 initialization successful!");
    }
   
    
    //filename = "DATA202.csv";
    //filename = "20220628.csv";  
    //filename = "DATA20220628.csv";
    //filename = "20220628DATA.csv";
    
    filename = "FLI01.csv";
    myFile = SD.open(filename, FILE_WRITE);
    
    if(myFile)
    {
        myFile.println("dataTag,Middle,Side,Top,Horizontal,Vertical,Pressure (mBar),Temperature (C),Altitude (m), Gas Concentration average (ppm), Gas Concentration max (ppm)");
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
*/
void saveData(int foo, int a, int b, int c, int d, int e, float p, float t, float h, float j, int y){
  if(myFile)
  {
    myFile.println(String(foo)+","+String(a)+","+String(b)+","+String(c)+","+String(d)+","+String(e)+","+String(p)+","+String(t)+","+String(h)+","+String(j)+","+String(y));
    myFile.close();
    myFile = SD.open(filename, FILE_WRITE);
    Serial.println("Data recorded.");
  }
  else
  {
    Serial.println("unable to recorded.");
    return;
  }
}

/*
 * void loop() is the main body of the program - it manages the
 *   counters and pressure sensor and packages data as needed for processing via the
 *   saveData() function.
 */
void loop() {
    if(millis()%3000==0) //60,000 can be changed as needed 
    {
        
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
        Serial.print("Temperature :  ");
        Serial.print(temp);
        Serial.println(" C"); //CHANGE TO KELVIN
        Serial.print("Pressure    :  ");
        Serial.print(pres);
        Serial.println(" mBar");
        Serial.print("Altitude    :  ");
        Serial.print(alt);
        Serial.println(" meter");
        /*
        Serial.print("count1: "); Serial.println(count1);
        Serial.print("count2: "); Serial.println(count2);
        Serial.print("count3: "); Serial.println(count3);
        Serial.print("count12: "); Serial.println(count12);
        Serial.print("count13: "); Serial.println(count13);
        */

        
        Serial.print("Max gas concentration: "); Serial.println(maxSmokeData);
        //Calculate avgSmokeData
        avgSmokeData = avgSmokeData/smokeDataCount;
        Serial.print("Average gas concentration: "); Serial.println(avgSmokeData);

        saveData(dataTag, count1, count2, count3, count12, count13, pres, temp, alt, avgSmokeData, maxSmokeData);
        dataTag += 1;
        /*
        count1 = 0; //Middle
        count2 = 0; //Side
        count3 = 0; //Top
        count12 = 0; //Horizontal
        count13 = 0; //Vertical    
        */
        
        maxSmokeData = 0;
        avgSmokeData = 0;
        smokeDataCount = 0;
    }

    if(millis()%1000==0) //Read smokeData every half second
    {
        smokeData = analogRead(smokeInput); smokeDataCount++;
        maxSmokeData = max(smokeData,maxSmokeData);

        //Make avgSmokeData the sum of gas concentration in 30s
        avgSmokeData += smokeData;
    }
    
    
    /*
    int value1 = digitalRead(28); //middle 
    int value2 = digitalRead(30); //side
    int value3 = digitalRead(32); //top 

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
    */


}
