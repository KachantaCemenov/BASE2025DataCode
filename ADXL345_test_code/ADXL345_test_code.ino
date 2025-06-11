// Tri's code, implementing methods from library SparkFun_ADXL345


#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345();
float X_out, Y_out, Z_out;
float roll, pitch, rollF, pitchF = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  adxl.powerOn();
  adxl.setRangeSetting(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);
  
  X_out = float(x)/256;
  Y_out = float(y)/256;
  Z_out = float(z)/256;
  
  /*
  Serial.print(X_out);
  Serial.print(", ");
  Serial.print(Y_out);
  Serial.print(", ");
  Serial.println(Z_out); 
  */

  pitch = atan(X_out / sqrt( pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
  roll = atan(Y_out / sqrt( pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  
  
  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;
  
  Serial.print(rollF);
  Serial.print(" / ");
  Serial.println(pitchF);
  
}
