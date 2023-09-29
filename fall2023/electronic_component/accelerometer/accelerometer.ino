
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
  
  
  int Top = 23;
  int Bottom = 18;
  int Left = 5;
  int Right = 19;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mpu.begin();
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!

  Serial.println("MPU6050 Found!");

  //mpu.begin();

  pinMode(Top, OUTPUT);
  pinMode(Bottom, OUTPUT);
  pinMode(Left, OUTPUT);
  pinMode(Right, OUTPUT);

  Serial.println("Adafruit MPU6050 test!2222");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Adafruit MPU6050 test!3333");

  //This boots up the LEDs in a circular pattern for testing and showing the system is rebooting
  digitalWrite(Top, HIGH);
  delay(500);
  digitalWrite(Right, HIGH);
  delay(500);
  digitalWrite(Bottom, HIGH);
  delay(500);
  digitalWrite(Left, HIGH);
  delay(500);
  digitalWrite(Top, LOW);
  delay(500);
  digitalWrite(Right, LOW);
  delay(500);
  digitalWrite(Bottom, LOW);
  delay(500);
  digitalWrite(Left, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (a.acceleration.y >4){
    digitalWrite(Top, HIGH);
  }
  else{
    digitalWrite(Top, LOW);
  }

  if (a.acceleration.y < -4){
    digitalWrite(Bottom, HIGH);
  }
  else{
    digitalWrite(Bottom, LOW);
  }

  if (a.acceleration.x >4){
    digitalWrite(Right, HIGH);
  }
  else{
    digitalWrite(Right, LOW);
  }

  if (a.acceleration.x < -4){
    digitalWrite(Left, HIGH);
  }
  else{
    digitalWrite(Left, LOW);
  }

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(100);

}
