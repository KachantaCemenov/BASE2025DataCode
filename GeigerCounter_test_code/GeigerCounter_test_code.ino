
int count1 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  pinMode(3, INPUT);
}
int val1 = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (millis()%10000 == 0)
  {
    Serial.println(count1);
    count1 = 0;
  }

  val1 = digitalRead(3);
  if (val1 == 1)
  {
    count1++;
  }
}
