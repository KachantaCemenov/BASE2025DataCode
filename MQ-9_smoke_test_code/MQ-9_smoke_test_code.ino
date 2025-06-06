//Simple code to see if MQ-9 is working.

int smokeData;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()%1000==0)
  {
    smokeData = analogRead(A1);
    Serial.println(smokeData);
  }
  
  
}
