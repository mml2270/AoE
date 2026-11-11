
/*
  Slap Detection Subsystem
  Preliminary prototype

  Player 1 IR: D2
  Player 2 IR: D3
  Piezo:       A0

  Assumption: IR receiver output is LOW
  when its beam is interrupted.
*/

const int IR_PLAYER_1 = 2;
const int IR_PLAYER_2 = 3;
const int PIEZO_PIN = A0;

// These values will be calibrated during testing.
const int PIEZO_THRESHOLD = 80;

const unsigned long DEBOUNCE_US = 5000;
const unsigned long IMPACT_WINDOW_US = 400000;
const unsigned long SIMULTANEOUS_US = 30000;
const unsigned long IMPACT_LOCKOUT_US = 80000;

bool previousP1 = HIGH;
bool previousP2 = HIGH;

unsigned long p1Time = 0;
unsigned long p2Time = 0;

unsigned long lastP1Change = 0;
unsigned long lastP2Change = 0;
unsigned long lastImpact = 0;

bool p1Detected = false;
bool p2Detected = false;

void setup() {
  Serial.begin(9600);

  pinMode(IR_PLAYER_1, INPUT_PULLUP);
  pinMode(IR_PLAYER_2, INPUT_PULLUP);

  Serial.println("Slap Detection System Ready");
}

void loop() {
  unsigned long now = micros();

  // Read infrared sensors
  bool currentP1 = digitalRead(IR_PLAYER_1);
  bool currentP2 = digitalRead(IR_PLAYER_2);

  // Player 1 beam interruption
  if (currentP1 != previousP1) {
    if (now - lastP1Change >= DEBOUNCE_US) {
      lastP1Change = now;
      previousP1 = currentP1;

      if (currentP1 == LOW) {
        p1Time = now;
        p1Detected = true;
        Serial.println("Player 1: Hand detected");
      }
    }
  }

  // Player 2 beam interruption
  if (currentP2 != previousP2) {
    if (now - lastP2Change >= DEBOUNCE_US) {
      lastP2Change = now;
      previousP2 = currentP2;

      if (currentP2 == LOW) {
        p2Time = now;
        p2Detected = true;
        Serial.println("Player 2: Hand detected");
      }
    }
  }

  // Read piezo sensor
  int piezoValue = analogRead(PIEZO_PIN);

  // Detect impact, with a lockout against repeated triggers
  if (piezoValue > PIEZO_THRESHOLD &&
      now - lastImpact >= IMPACT_LOCKOUT_US) {

    lastImpact = now;

    Serial.println("IMPACT DETECTED");

    bool p1Recent =
      p1Detected && (now - p1Time <= IMPACT_WINDOW_US);

    bool p2Recent =
      p2Detected && (now - p2Time <= IMPACT_WINDOW_US);

    if (p1Recent && p2Recent) {
      unsigned long difference =
        (p1Time > p2Time) ? p1Time - p2Time
                         : p2Time - p1Time;

      if (difference <= SIMULTANEOUS_US) {
        Serial.println("RESULT: AMBIGUOUS");
      } else if (p1Time < p2Time) {
        Serial.println("RESULT: PLAYER 1 LIKELY FIRST");
      } else {
        Serial.println("RESULT: PLAYER 2 LIKELY FIRST");
      }
    }
    else if (p1Recent) {
      Serial.println("RESULT: PLAYER 1 LIKELY");
    }
    else if (p2Recent) {
      Serial.println("RESULT: PLAYER 2 LIKELY");
    }
    else {
      Serial.println("RESULT: IMPACT WITHOUT RECENT HAND ENTRY");
    }

    // Reset events after processing an impact
    p1Detected = false;
    p2Detected = false;
  }
}
