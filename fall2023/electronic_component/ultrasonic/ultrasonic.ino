// Define the pins for the ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;

// Define the pin for the buzzer (if using)
const int buzzerPin = 4;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT); // If using a buzzer
}

void loop() {
  // Send a short pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  int distance = duration / 58.2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Map the distance to a pitch or volume value
  int pitch = map(distance, 5, 100, 50, 1000); // Adjust the values as needed

  // Generate a tone with the buzzer (if using)
  tone(buzzerPin, pitch);

  // Add a delay to prevent rapid changes
  delay(100);
}
