#define IR 36
#define ledGreen 32
#define ledRed 33

#define IR2 4
#define led2Green 23
#define led2Red 5

void setup() {
  pinMode(IR2, INPUT);
  pinMode(led2Green, OUTPUT);
  pinMode(led2Red, OUTPUT);
  pinMode(IR, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int val = digitalRead(IR);
  int val2 = digitalRead(IR2);

  if(val == 0)
    digitalWrite(ledRed, HIGH);
  else
    digitalWrite(ledGreen, HIGH);
  Serial.println(val);
  Serial.println("---");

  if(val2 == 0)
    digitalWrite(led2Red, HIGH);
  else
    digitalWrite(led2Green, HIGH);
  Serial.println(val2);
  Serial.println("---");
  delay(500);

    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(led2Red, LOW);
    digitalWrite(led2Green, LOW);

}