#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX=10, TX=11

// ── Pin definitions ──────────────────────────
const int RELAY_PIN = 7;   // Relay → LED
const int IN1       = 4;   // L298N IN1
const int IN2       = 3;   // L298N IN2
// ENA not connected → jumper on L298N = always enabled

bool ledState   = false;
bool motorState = false;

// ── Setup ────────────────────────────────────
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // LED OFF at start
  stopMotor();                   // Motor OFF at start

  Serial.begin(9600);
  BTSerial.begin(9600);

  printMenu();
}

// ── Loop ─────────────────────────────────────
void loop() {
  if (BTSerial.available()) {
    char cmd = (char)BTSerial.read();
    Serial.print("BT CMD: ");
    Serial.println(cmd);
    handleCommand(cmd);
  }
  if (Serial.available()) {
    char cmd = (char)Serial.read();
    handleCommand(cmd);
  }
}

// ── Command Handler ───────────────────────────
void handleCommand(char cmd) {
  switch (cmd) {

    // ── LED via Relay ──
    case '1':
      digitalWrite(RELAY_PIN, LOW);  // Active LOW = ON
      ledState = true;
      BTSerial.println("LED ON");
      Serial.println("LED ON");
      break;

    case '0':
      digitalWrite(RELAY_PIN, HIGH); // HIGH = OFF
      ledState = false;
      BTSerial.println("LED OFF");
      Serial.println("LED OFF");
      break;

    case 't': case 'T':
      ledState = !ledState;
      digitalWrite(RELAY_PIN, ledState ? LOW : HIGH);
      BTSerial.println(ledState ? "LED Toggled ON" : "LED Toggled OFF");
      break;

    // ── DC Motor ──
    case 'f': case 'F':
      motorForward();
      BTSerial.println("Motor: FORWARD");
      Serial.println("Motor: FORWARD");
      break;

    case 'b': case 'B':
      motorBackward();
      BTSerial.println("Motor: BACKWARD");
      Serial.println("Motor: BACKWARD");
      break;

    case 's': case 'S':
      stopMotor();
      BTSerial.println("Motor: STOPPED");
      Serial.println("Motor: STOPPED");
      break;

    // ── Status & Menu ──
    case '?':
      sendStatus();
      break;

    case 'm': case 'M':
      printMenu();
      break;

    default:
      BTSerial.println("Unknown! Send 'm' for menu.");
      break;
  }
}

// ── Motor: Forward ────────────────────────────
void motorForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  motorState = true;
}

// ── Motor: Backward ───────────────────────────
void motorBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  motorState = true;
}

// ── Motor: Stop ───────────────────────────────
void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  motorState = false;
}

// ── Status ────────────────────────────────────
void sendStatus() {
  BTSerial.println("=== STATUS ===");
  BTSerial.print("LED   : "); BTSerial.println(ledState ? "ON" : "OFF");
  BTSerial.print("Motor : "); BTSerial.println(motorState ? "RUNNING" : "STOPPED");
  BTSerial.println("==============");
}

// ── Menu ──────────────────────────────────────
void printMenu() {
  String m = "\n====== CONTROL MENU ======\n";
  m += "LED:\n";
  m += "  1 = LED ON\n";
  m += "  0 = LED OFF\n";
  m += "  t = Toggle LED\n";
  m += "MOTOR:\n";
  m += "  f = Forward\n";
  m += "  b = Backward\n";
  m += "  s = Stop\n";
  m += "OTHER:\n";
  m += "  ? = Status\n";
  m += "  m = This menu\n";
  m += "==========================\n";
  Serial.print(m);
  BTSerial.print(m);
}