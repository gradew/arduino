#define PIN_TEMP A0

void setup() {
  pinMode(PIN_TEMP, INPUT);
  Serial.begin(115200);
}

void loop() {
  int val=analogRead(PIN_TEMP);
  float voltage = val * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;
  Serial.println(temperatureC);
  delay(1000);
}
