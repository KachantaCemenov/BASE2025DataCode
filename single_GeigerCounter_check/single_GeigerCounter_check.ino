
int count1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(28, INPUT); Serial.print("Pin mapped as INPUT: "); Serial.println(28);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value1 = digitalRead(28); //middle
  if (value1 == 1)
  {
    count1++;
  }

  if (millis()%5000 == 0)
  {
    Serial.println(count1);
    count1 = 0;
  }
}
