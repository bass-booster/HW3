//The Pin of the push button
const int buttonPin = 2;

//The Pin of the RGB LED Module
const int RLedPin = 9;
const int GLedPin = 10;
const int BLedPin = 11;

//Button state variables
int buttonState = 0;
bool buttonPressed = false;
unsigned long pressingTime = 0;
const int longPressInterval = 1000;

//Light color selection
int lightNum = 0;
int RLightColor = 0;
int GLightColor = 0;
int BLightColor = 0;

int RCurrentColor = 0;
int GCurrentColor = 0;
int BCurrentColor = 0;

//Modes
int currentMode = 0;

//Blink mode variables
unsigned long blinkTimer = 0;
const int blinkInterval = 500;
bool blinkOn = true;

//Fade (breathing) mode variables
const int fadeAmount = 2;
int fadeDirection = 1;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(RLedPin, OUTPUT);
  pinMode(GLedPin, OUTPUT);
  pinMode(BLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  checkButton();
  updateLEDColor();
  setRGBLEDColor(RCurrentColor, GCurrentColor, BCurrentColor);
}

void setRGBLEDColor(int r, int g, int b){
  analogWrite(RLedPin, r);
  analogWrite(GLedPin, g);
  analogWrite(BLedPin, b);
}

void checkButton(){
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !buttonPressed) {
    pressingTime = millis();
    buttonPressed = true;
  }

  if (buttonState == HIGH && buttonPressed) {
    unsigned long currentTime = millis();

    if (currentTime - pressingTime < longPressInterval) {
      changeLEDColor();
      Serial.println("short click");
    }
    else{
      changeMode();
      Serial.println("long press");
    }

    buttonPressed = false;
  }
}

void changeLEDColor(){
  lightNum++;

  if(lightNum >= 5){
    lightNum = 0;
  }

  //Assign the color
  if(lightNum == 0){ //white
    RLightColor = 0;  GLightColor = 0;  BLightColor = 0;
  }
  if(lightNum == 1){ //red
    RLightColor = 0;  GLightColor = 255;  BLightColor = 255;
  }
  if(lightNum == 2){ //green
    RLightColor = 255; GLightColor = 0;  BLightColor = 255;
  }
  if(lightNum == 3){ //blue
    RLightColor = 255; GLightColor = 255; BLightColor = 0;
  }
  if(lightNum == 4){ //yellow
    RLightColor = 0; GLightColor = 0; BLightColor = 255;
  }

  RCurrentColor = RLightColor;
  GCurrentColor = GLightColor;
  BCurrentColor = BLightColor;
}

void changeMode(){
  currentMode++;
  if(currentMode >= 3){
    currentMode = 0;
  }

  if(currentMode == 1){
    blinkTimer = 0;
    blinkOn = true;
  }
  if(currentMode == 2){
    fadeDirection = 1;
  }
}

void updateLEDColor(){
  if(currentMode == 0){
    // steady
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }

  else if(currentMode == 1){
    // blink
    unsigned long currentTime = millis();
    if(currentTime - blinkTimer > blinkInterval){
      blinkOn = !blinkOn;
      blinkTimer = currentTime;
    }

    if(blinkOn){
      RCurrentColor = RLightColor;
      GCurrentColor = GLightColor;
      BCurrentColor = BLightColor;
    }
    else{
      RCurrentColor = 255;
      GCurrentColor = 255;
      BCurrentColor = 255;
    }
  }

  else if(currentMode == 2){
    // FADE / BREATHING MODE
    bool hitLimit = false;

    // RED channel
    if (RLightColor == 0) {
      RCurrentColor += fadeDirection * fadeAmount;
      if (RCurrentColor <= 0 || RCurrentColor >= 255) {
        RCurrentColor = constrain(RCurrentColor, 0, 255);
        hitLimit = true;
      }
    }

    // GREEN channel
    if (GLightColor == 0) {
      GCurrentColor += fadeDirection * fadeAmount;
      if (GCurrentColor <= 0 || GCurrentColor >= 255) {
        GCurrentColor = constrain(GCurrentColor, 0, 255);
        hitLimit = true;
      }
    }

    // BLUE channel
    if (BLightColor == 0) {
      BCurrentColor += fadeDirection * fadeAmount;
      if (BCurrentColor <= 0 || BCurrentColor >= 255) {
        BCurrentColor = constrain(BCurrentColor, 0, 255);
        hitLimit = true;
      }
    }

    if(hitLimit){
      fadeDirection = -fadeDirection;
    }

    delay(20);
  }
}