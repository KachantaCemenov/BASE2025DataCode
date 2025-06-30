
#include <SD.h>

#include <SPI.h>
#include <MS5607.h> 

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

File myFile;
String filename;
MS5607 P_Sens;

void setup() {

    Serial.begin(9600); 

    for(int g=28; g<34; g+=2)
    {
        pinMode(g, INPUT);
        Serial.print("Pin mapped as INPUT: "); Serial.println(g);
    }
    
    
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
    
    /*
    //Prepare the ADXL345 accelerometer
    adxl.powerOn();
    adxl.setRangeSetting(2);
    Serial.println("ADXL345 initialized");
    */
   
    
    //filename = "DATA202.csv";
    //filename = "20220628.csv";  
    //filename = "DATA20220628.csv";
    //filename = "20220628DATA.csv";
    
    filename = "TEST_04.csv";
    myFile = SD.open(filename, FILE_WRITE);
    
    if(myFile)
    {
        myFile.println("dataTag,Middle,Side,Top,Horizontal,Vertical,Pressure (mBar),Temperature (C),Altitude (m)");
        myFile.close();
    }
    else
    {
        myFile.close();
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
void saveData(int foo, int a, int b, int c, int d, int e, float p, float t, float h){
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile)
  {
    myFile.println(String(foo)+","+String(a)+","+String(b)+","+String(c)+","+String(d)+","+String(e)+","+String(p)+","+String(t)+","+String(h));
    myFile.close();
    Serial.println("Data recorded.");
  }
  else
  {
    Serial.println("Unable to recorded.");
    myFile.close();
    return;
  }
}

/*
 * void loop() is the main body of the program - it manages the
 *   counters and pressure sensor and packages data as needed for processing via the
 *   saveData() function.
 */
void loop() {
    if(millis()%30000==0) 
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
        
        Serial.println("==================================================");
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
        Serial.print("Max Roll: "); Serial.println(maxRoll);
        Serial.print("Max Pitch: "); Serial.println(maxPitch);
        
        avgRoll = avgRoll/secondDataCount;
        avgPitch = avgPitch/secondDataCount;
        
        Serial.print("Average roll: "); Serial.println(avgRoll);
        Serial.print("Average pitch: "); Serial.println(avgPitch);
        */

        saveData(dataTag, count1, count2, count3, count12, count13, pres, temp, alt);
        dataTag += 1;
        
        count1 = 0; //Middle
        count2 = 0; //Side
        count3 = 0; //Top
        count12 = 0; //Horizontal
        count13 = 0; //Vertical    
        
        /*
        maxRoll = 0; avgRoll = 0;
        maxPitch = 0; avgPitch = 0;
        */
    }
    /*
    if(millis()%1000==0) //Every second
    {
        
        smokeData = analogRead(smokeInput); 
        maxSmokeData = max(smokeData,maxSmokeData);

        //Make avgSmokeData the sum of gas concentration in 30s
        avgSmokeData += smokeData;
        

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

        secondDataCount++;
    }
    */
    
    
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
    


}
