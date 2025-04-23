// ARDUINO BASED SMARTPHONE CHARGER WITH OPTIMAL CHARGE CUTOFF

// === Pin Configuration ===
const int shuntPin1 = A0;     // First side of shunt resistor
const int shuntPin2 = A1;     // Second side of shunt resistor
const int relayPin  = 7;      // Relay IN pin connected to D7

// === Shunt & Current Threshold Settings ===
const float shuntResistance = 0.2;      // 0.2 ohm
const float cutoffCurrent   = 0.15;     // 150 mA cutoff threshold
const float disconnectCurrent = 0.02;   // 20 mA = assume phone unplugged

void setup() {
  analogReference(INTERNAL);        // Use internal 1.1V reference for better resolution
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);      // Start with charging ON

  Serial.println("Phone charging controller initialized.");
}

void loop() {
  float current = readShuntCurrent();  // in Amps
  Serial.print("Current: ");
  Serial.print(current * 1000);  // in mA
  Serial.println(" mA");

  if (current < disconnectCurrent) {
    // No current — assume phone not connected
    Serial.println("Phone unplugged or no load. Keeping charging ON.");
    digitalWrite(relayPin, HIGH);   // Charging ON
  }
  else if (current < cutoffCurrent) {
    // Current is low, phone is likely fully charged
    Serial.println("Battery likely full. Turning OFF charging.");
    digitalWrite(relayPin, LOW);  // Charging OFF
  }
  else {
    // Phone is actively charging
    Serial.println("Charging in progress. Keeping charging ON.");
    digitalWrite(relayPin, HIGH);   // Charging ON
  }

  delay(5000);  // Check every 5 seconds
}

float readShuntCurrent() {
  const int samples = 100;
  long sum1 = 0;
  long sum2 = 0;

  for (int i = 0; i < samples; i++) {
    sum1 += analogRead(shuntPin1);
    sum2 += analogRead(shuntPin2);
    delay(1);
  }

  float avg1 = sum1 / (float)samples;
  float avg2 = sum2 / (float)samples;

  float voltage1 = (avg1 * 1.1) / 1023.0;  // Using 1.1V reference
  float voltage2 = (avg2 * 1.1) / 1023.0;

  float voltageDrop = voltage1 - voltage2;
  float current = voltageDrop / shuntResistance;

  return abs(current);  // Always positive
}
