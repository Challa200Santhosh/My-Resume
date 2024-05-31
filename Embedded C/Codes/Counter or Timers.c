const int ledPin = 3;
const int buttonPin = 2;

int ledState = LOW;
int buttonState;
int lastButtonState = HIGH;
int counter = 0;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
        counter++;
        Serial.print("Button pressed ");
        Serial.print(counter);
        Serial.println(" times");
      }
    }
  }
  lastButtonState = reading;
}