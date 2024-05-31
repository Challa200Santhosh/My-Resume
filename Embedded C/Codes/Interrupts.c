#define LED_PIN 9
#define BUTTON_PIN 3

volatile byte ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blinkLed, RISING);
}

void loop() {
  // nothing here!
}

void blinkLed() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}