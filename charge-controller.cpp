// ARDUINO BASED SMARTPHONE CHARGER WITH OPTIMAL CHARGE CUTOFF

const int relayPin = 7;
const int shuntPin1 = A0;
const int shuntPin2 = A1; 

const float shuntResistance = 0.2; 
const float cutoffCurrent = 0.15; // Cutoff current in Amps (e.g., 80mA)

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Start charging
  Serial.begin(9600);
}

void loop() {
  float chargingCurrent = readChargingCurrent();

  Serial.print("Charging Current: ");
  Serial.print(chargingCurrent);
  Serial.println(" A");

  if (chargingCurrent < cutoffCurrent) {
    digitalWrite(relayPin, LOW); // Stop charging
    Serial.println("Charging complete - Relay OFF");
    delay(10000);
  } else {
    digitalWrite(relayPin, HIGH); // Continue charging
    Serial.println("Charging... Relay ON");
  }
  delay(10000);
}

float readChargingCurrent() {
  int raw1 = analogRead(shuntPin1);
  int raw2 = analogRead(shuntPin2);

  float voltage1 = raw1 * (5.0 / 1023.0);
  float voltage2 = raw2 * (5.0 / 1023.0);

  float voltageDrop = voltage1 - voltage2;
  float current = voltageDrop / shuntResistance;

  return current;
}
