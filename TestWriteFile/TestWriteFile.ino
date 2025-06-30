#include <SPI.h>
#include <SD.h>

File myFile;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SD.begin(10);
  myFile = SD.open("TextWrite.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.println("Start Writing");
    myFile.close();
  }
  
}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (millis()%1000 == 0)
  {
    myFile = SD.open("WRI.txt", FILE_WRITE);
    if (myFile)
    {
      myFile.println("Writing" + String(i));
      Serial.println("Written");
      myFile.close();
      i++;
    }
    else
    {
      Serial.println("Unable to write");
    }
    
  }
  
}
