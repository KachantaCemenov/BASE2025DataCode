#include <SPI.h>
#include <SD.h>



//Interupt pins: 2, 3, 18
int intMiddle = 2;
int intSide = 3;
int intTop = 18;



volatile int middle = 0;
volatile int side = 0;
volatile int top = 0;
volatile bool top_flag, side_flag, middle_flag = false;

int hori = 0;
int verti = 0;

int dataTag = 0;
File myFile;
String filename;

void Count1()
{
  middle++;
  middle_flag = true;
}
void Count2()
{
  side++;
  side_flag = true;
}
void Count3()
{
  top++;
  top_flag = true;
}


void setup(){
  Serial.begin(115200);
	pinMode(intMiddle, INPUT);
  pinMode(intSide, INPUT);
  pinMode(intTop, INPUT);

  attachInterrupt(digitalPinToInterrupt(intMiddle),Count1, FALLING);
  attachInterrupt(digitalPinToInterrupt(intSide),Count2, FALLING);
  attachInterrupt(digitalPinToInterrupt(intTop),Count3, FALLING);

  if(!SD.begin(10))
  {
    Serial.println("Error - no SD.begin");
  }
  else
  {
    Serial.println("SD card detected.");
  }

  filename = "TEST01.csv";
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile)
  {
    myFile.println("dataTag,Middle,Side,Top,Horizontal,Vertical");
    myFile.close();
  }
}
void saveData(int foo, int a, int b, int c, int d, int e){
  
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile)
  {
    myFile.println(String(foo)+","+String(a)+","+String(b)+","+String(c)+","+String(d)+","+String(e));
    myFile.close();
    
    Serial.println("Data recorded.");
  }
  else
  {
    Serial.println("Unable to recorded.");
    return;
  }
}

float startTime = 0;

void loop(){ 
  /*
  volatile int vol_1 = digitalRead(intMiddle);
  volatile int vol_2 = digitalRead(intSide);
  volatile int vol_3 = digitalRead(intTop);
  Serial.println(vol_1);
  Serial.println(vol_2);
  Serial.println(vol_3);
  */

  if (middle_flag && side_flag)
  {
    hori++;
    middle_flag = side_flag = false;
  }
  else if (middle_flag && top_flag)
  {
    verti++;
    middle_flag = top_flag = false;
  }
  

  if (millis() - startTime >= 60000)
  {
    Serial.print(middle); Serial.print("|");
    Serial.print(side); Serial.print("|");
    Serial.print(top); Serial.print("|");
    Serial.print(hori); Serial.print("|");
    Serial.println(verti); 


    saveData(dataTag, middle, side, top, hori, verti);
    middle = 0;
    side = 0;
    top = 0;
    verti = 0;
    hori = 0;

    startTime = millis();
    dataTag++;
  }
}








