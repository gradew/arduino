#define RELAY_PINS_START 2
#define RELAYS 4
#define RELAY_DELAY 250

int k;

void setup() {
  for(k=RELAY_PINS_START; k<RELAY_PINS_START+RELAYS; k++){pinMode(k, OUTPUT);}
  pinMode(LED_BUILTIN, OUTPUT);
  
  for(k=RELAY_PINS_START; k<RELAY_PINS_START+RELAYS; k++){digitalWrite(k, HIGH);}
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  for(k=RELAY_PINS_START; k<RELAY_PINS_START+RELAYS; k++)
  {
    digitalWrite(k, LOW); delay(RELAY_DELAY);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  for(k=RELAY_PINS_START; k<RELAY_PINS_START+RELAYS; k++)
  {
    digitalWrite(k, HIGH); delay(RELAY_DELAY);
  }
  digitalWrite(LED_BUILTIN, LOW);
}
