# Hardware Pinout & Interfacing Documentation

## 1. STM32 Microcontroller Pinout

| Module / Hardware | STM32 Pin | Direction | Functional Purpose |
| :--- | :--- | :--- | :--- |
| **Target Data Bit 0 (`D0`)** | `PC6` | Input (from VEGA) | LSB of 3-bit parallel target table bus |
| **Target Data Bit 1 (`D1`)** | `PC8` | Input (from VEGA) | Middle bit of 3-bit parallel target table bus |
| **Target Data Bit 2 (`D2`)** | `PC9` | Input (from VEGA) | MSB of 3-bit parallel target table bus |
| **Hardware E-Stop Line** | `PC10` | Input (from VEGA) | Active-High interrupt for emergency stop |
| **Delivery ACK Button** | `PC11` | Input | Active-High push-button for customer confirmation |
| **IR Sensor (Right)** | `PC0` | Input | Digital input from right TCRT5000 sensor |
| **IR Sensor (Center)** | `PC2` | Input | Digital input from center TCRT5000 sensor |
| **IR Sensor (Left)** | `PC3` | Input | Digital input from left TCRT5000 sensor |
| **Motor PWM (Left Motor)** | `PA0` (`TIM2_CH1`) | Output | Pulse-Width Modulation speed control to L298N ENA |
| **Motor PWM (Right Motor)** | `PA3` (`TIM2_CH4`) | Output | Pulse-Width Modulation speed control to L298N ENB |
| **Motor Direction (Left)** | `PA4`, `PA5` | Output | Phase control to L298N IN1 & IN2 |
| **Motor Direction (Right)** | `PB8`, `PB9` | Output | Phase control to L298N IN3 & IN4 |
| **Binary Status LEDs** | `PB0`, `PB7`, `PB14` | Output | Onboard destination display & state indication |

---

## 2. VEGA RISC-V Microcontroller Pinout

| Peripheral / Component | VEGA Pin | Mode | Functional Purpose / Destination |
| :--- | :--- | :--- | :--- |
| **Table 1 Button (`BTN1`)** | `Pin 2` | Input (`INPUT_PULLUP`) | Select Destination Table 1 |
| **Table 2 Button (`BTN2`)** | `Pin 3` | Input (`INPUT_PULLUP`) | Select Destination Table 2 |
| **Table 3 Button (`BTN3`)** | `Pin 4` | Input (`INPUT`) | Select Destination Table 3 |
| **Table 4 Button (`BTN4`)** | `Pin 5` | Input (`INPUT_PULLUP`) | Select Destination Table 4 |
| **Table 5 Button (`BTN5`)** | `Pin 6` | Input (`INPUT_PULLUP`) | Select Destination Table 5 |
| **Return Button (`BTN_OK`)** | `Pin 10` | Input (`INPUT_PULLUP`) | Trigger return-to-base state |
| **Ultrasonic Trigger (`TRIG`)** | `Pin 8` | Output | HC-SR04 pulse trigger |
| **Ultrasonic Echo (`ECHO`)** | `Pin 9` | Input | HC-SR04 distance pulse input |
| **Target Data Bit 0 (`D0`)** | `Pin 11` | Output $\rightarrow$ STM32 `PC6` | LSB of 3-bit parallel target table bus |
| **Target Data Bit 1 (`D1`)** | `Pin 12` | Output $\rightarrow$ STM32 `PC8` | Middle bit of 3-bit parallel target table bus |
| **Target Data Bit 2 (`D2`)** | `Pin 13` | Output $\rightarrow$ STM32 `PC9` | MSB of 3-bit parallel target table bus |
| **E-Stop Line (`STOP_PIN`)** | `Pin 14` | Output $\rightarrow$ STM32 `PC10` | Active-High stop flag (<20 cm obstacle detected) |
| **Return Line (`RETURN_PIN`)**| `Pin 15` | Output $\rightarrow$ STM32 | Signals return mode state |
| **Select Strobe (`SELECT_PIN`)**| `Pin 16` | Output $\rightarrow$ STM32 | 100ms strobe pulse on new table selection |
| **16x2 LCD Display** | I2C Bus (`Wire1`) | Output / Bidirectional | Status display screen (Address `0x27`) |
