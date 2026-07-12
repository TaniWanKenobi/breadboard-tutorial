const int LED_PIN = 3;      // PWM pin (~) for brightness
const int BUTTON_PIN = 2;   // button to GND, uses internal pull-up
const int POT_PIN = A0;     // potentiometer wiper

bool ledOn = false;             // current on/off state
int lastButtonReading = HIGH;   // last raw reading (HIGH = not pressed with pull-up)
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // no external resistor needed
}

void loop() {
  // --- Read button with debounce ---
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();   // input changed, reset timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // reading has been stable long enough
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;
      // with INPUT_PULLUP, a press pulls the pin LOW
      if (buttonState == LOW) {
        ledOn = !ledOn;   // toggle on each press
      }
    }
  }
  lastButtonReading = reading;

  // --- Set brightness from potentiometer ---
  if (ledOn) {
    int potValue = analogRead(POT_PIN);          // 0..1023
    int brightness = map(potValue, 0, 1023, 0, 255);
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0);                      // off
  }
}
