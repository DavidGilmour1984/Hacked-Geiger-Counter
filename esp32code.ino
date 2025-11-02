int threshold = 750;       // Default threshold
const int adcPin = 36;     // VP pin (ADC1_CH0)
const int pulsePin = 15;   // Pin to pull low

bool inPulse = false;
int minVal = 4095;

unsigned long startTime;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // 0–4095
  pinMode(pulsePin, OUTPUT);
  digitalWrite(pulsePin, HIGH); // keep high initially
  startTime = millis();
}

void loop() {

  // trigger D15 low 20 seconds after power-on for 0.5 sec
  if ((millis() - startTime) >= 20000 && (millis() - startTime) < 20500) {
    digitalWrite(pulsePin, LOW);
  } else if ((millis() - startTime) >= 20500) {
    digitalWrite(pulsePin, HIGH);
  }

  // Read ADC from VP pin
  int v = analogRead(adcPin);

  // Start of pulse
  if (!inPulse && v < threshold) {
    inPulse = true;
    minVal = v;
  }

  // Track minimum during pulse
  if (inPulse) {
    if (v < minVal) minVal = v;

    // End of pulse, signal rising back above threshold
    if (v > threshold) {
      inPulse = false;
      Serial.println(minVal);
    }
  }
}
