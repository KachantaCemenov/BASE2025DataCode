
int count1 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(30, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis()%10000 == 0)
  {
    Serial.println(count1);
    count1 = 0;
  }

  int val1 = digitalRead(30);
  if (val1 == 1)
  {
    count1++;
  }
}
