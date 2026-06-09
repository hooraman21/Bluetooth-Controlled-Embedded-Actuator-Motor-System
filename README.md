# Bluetooth-Controlled-Embedded-Actuator-Motor-System
An Arduino Uno (ATmega328P) project integrating an HC-05 module for low-latency wireless communication. Uses a 5V relay for active-LOW isolation and an L298N H-Bridge for bidirectional DC motor logic. Built with modular Embedded C firmware featuring real-time command parsing and terminal status tracking.
# Embedded Systems — Open Ended Lab 
## Multi-Peripheral Control via Bluetooth & Serial Interfaces

A hands-on hardware-software implementation demonstrating asynchronous terminal control over high-power peripherals. This system handles bidirectional communication using standard hardware **Serial** and software-defined serial communication (**SoftwareSerial**) to interface an **HC-05/HC-06 Bluetooth module** with an Arduino UNO microcontroller. It allows wireless or tethered real-time operation of an isolated DC motor and an electronic relay switch.

Developed as **Lab (Open Ended Lab)** for the **Embedded Systems** course under the **Department of Computer Engineering** at **Pak-Austria Fachhochschule: Institute of Applied Sciences and Technology (PAF-IAST)**.

---

## 👥 Authors
- **Hoor Aman** (Reg No: `B24F0225CE039`)
- **Submitted To:** Engr. Rafiullah
- **Department:** BS Computer Engineering
- **Subject:** Embedded Systems

---

## 🌟 Features & Functionality

### Task 1: Isolated LED Control via Relay Module
* **Optoisolated Switching:** Utilizes an active-low 5V relay module to safely toggle a high-efficiency LED indicator circuit.
* **State Preservation:** Keeps track of the current operational state (`true`/`false`) to toggle the relay cleanly without state overlapping.

### Task 2: DC Motor Direction & State Manipulation
* **Dual-Bridge H-Bridge Control:** Interfaces with an L298N dual H-bridge motor driver to change current direction through the motor coils.
* **Full Motion Matrix:** Supports dynamic runtime changes for **Forward**, **Backward**, and **Hard Stop** commands instantly via code routines.

### Multi-Channel Communication Layer
* **Simultaneous Listening:** Features non-blocking polling loops that check both the physical USB hardware Serial port and the custom Bluetooth SoftwareSerial lines concurrently.
* **Interactive Control Menu:** Streams a clean, readable text-based Command Menu directly to the user's terminal upon system startup or when requested.

---

## 🛠️ Circuit Architecture & Pin Configuration

### Hardware Component Stack:
1. Arduino UNO Microcontroller
2. L298N Dual H-Bridge Motor Driver Module
3. 5V SPDT Relay Module & LED Circuit Load
4. HC-05 / HC-06 Bluetooth Serial Transceiver Module
5. DC Toy Motor (5V-12V)

### Dynamic Connection Matrix:
| Source Module | Component Pin | Arduino UNO Pin | Configuration / Notes |
| :--- | :--- | :--- | :--- |
| **Bluetooth Module** | TX Pin | **Pin 10 (RX)** | SoftwareSerial Receive Line |
| | RX Pin | **Pin 11 (TX)** | SoftwareSerial Transmit Line |
| **Relay Driver** | IN (Control) | **Pin 7** | OUTPUT (Active LOW = Relay ON) |
| **L298N H-Bridge** | IN1 | **Pin 4** | OUTPUT (Motor Direction Control A) |
| | IN2 | **Pin 3** | OUTPUT (Motor Direction Control B) |
| | ENA / ENB | *Jumpered* | Hardwired to 5V (Always Enabled) |

---

## 📲 Terminal Command Protocol Mapping

When connected to either the computer **Arduino Serial Monitor** (9600 Baud) or a smartphone **Bluetooth Serial Terminal App** (connected to the Bluetooth node module), use the following terminal inputs:

### 💡 LED Controls (Task 1)
* `1` : Turn LED ON (Sets Relay Control Pin LOW)
* `0` : Turn LED OFF (Sets Relay Control Pin HIGH)
* `t` / `T` : Toggle the LED dynamically based on its current runtime state

### ⚙️ Motor Controls (Task 2)
* `f` / `F` : Rotate DC Motor **Forward** (IN1=HIGH, IN2=LOW)
* `b` / `B` : Rotate DC Motor **Backward** (IN1=LOW, IN2=HIGH)
* `s` / `S` : **Stop** Motor Instantly (IN1=LOW, IN2=LOW)

### 📊 Utility Commands
* `?` : Request current peripheral vector diagnostic status
* `m` / `M` : Re-print the main structural **Control Menu** text block

---

## 💻 Core Firmware Firmware Pipeline

The system codebase is compiled using native C++ processing loops:

```cpp
void handleCommand(char cmd) {
  switch (cmd) {
    // LED Control Vectors
    case '1': digitalWrite(RELAY_PIN, LOW); ledState = true; break;
    case '0': digitalWrite(RELAY_PIN, HIGH); ledState = false; break;
    
    // Motor Motion Directives
    case 'f': case 'F': motorForward(); break;
    case 'b': case 'B': motorBackward(); break;
    case 's': case 'S': stopMotor(); break;
    
    // Telemetry Request
    case '?': sendStatus(); break;
  }
}
