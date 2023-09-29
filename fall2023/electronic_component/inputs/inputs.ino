#define LED 8
#define BUTTON 13
#define POTENTIOMETER A1

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(BUTTON) == LOW) {
    int potValue = analogRead(POTENTIOMETER);
    int brightness = map(potValue, 0, 1023, 0, 255);
    analogWrite(LED, brightness);    
  }
}