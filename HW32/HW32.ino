
const int buttonPin = 3;  
const int RLedPin = 9;     


int mode = 0;            
bool lastButtonState = HIGH;

unsigned long previousMillis = 0;
unsigned long interval = 1000; 
bool ledState = LOW;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(RLedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  handleButton();
  runMode();
}


void handleButton() {
  bool buttonState = digitalRead(buttonPin);


  if (buttonState == LOW && lastButtonState == HIGH) {
    mode++;
    if (mode > 3) mode = 0;

    Serial.print("Mode changed to: ");
    Serial.println(mode);

    delay(150);  
  }

  lastButtonState = buttonState;
}


void runMode() {
  if (mode == 0) {
    digitalWrite(RLedPin, LOW);
    return;
  }

  if (mode == 1) interval = 1000;     // slow
  if (mode == 2) interval = 300;      // medium
  if (mode == 3) interval = 100;      // fast

  unsigned long currentMillis = millis();

 
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    ledState = !ledState;
    digitalWrite(RLedPin, ledState);
  }
}