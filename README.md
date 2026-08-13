# Smart-Cafe-Robot-ES-333

An autonomous, dual-controller mobile robot designed for food and beverage delivery in a café environment. Built as a course project for **ES 333: Microprocessors and Embedded Systems** at IIT Gandhinagar.

The system features a distributed architecture offloading user interface and safety sensing to a **VEGA RISC-V board** while delegating low-level motor actuation, PWM speed control, line-following, and path tracking to an **STM32 (ARM Cortex-M) microcontroller**.

---

## 📌 Overview & Key Features

* **Dual-Controller Architecture:** High-level UI and obstacle safety managed by VEGA RISC-V; real-time motion and control loop executed on STM32.
* **3-Bit Parallel Inter-Board Communication:** Parallel GPIO bus (`PC6`, `PC8`, `PC9`) transmits table target binary codes from VEGA to STM32.
* **Active-High Hardware E-Stop:** Immediate motor power cutoff line (`PC10`) triggered by the VEGA board's HC-SR04 ultrasonic range tracking.
* **Closed-Loop Line Navigation:** 3-channel IR sensor array tracking with active intersection counting for target table stopping ($Target \le 5$).
* **Timer PWM Speed Control:** Dual-channel hardware PWM generation via STM32 Timer 2 (`TIM2_CH1`, `TIM2_CH4`) connected to an L298N H-Bridge driver.
* **Delivery Acknowledgment:** Destination state feedback via onboard binary LEDs and manual delivery sign-off push-button (`PC11`).

---

## Repo Structure

```text
Smart-Cafe-Robot-ES-333/
├── Stm32 Code files/
│   ├── main.c                   # STM32 FSM, TIM2 PWM, IR navigation, & GPIO logic
│   └── final_lap.ioc     # STM32CubeMX hardware configuration file
├── Vega code            # VEGA board sketch (LCD UI, user buttons, & ultrasonic obstacle detection)
├── reference docs/
│   ├── block_diagram.png        # System architecture diagram
│   ├── fsm_diagram.png          # Finite State Machine flowcharts
│   ├── schematic.pdf            # Full hardware pinout & wiring schematic
│   └── video_link.txt           # Link to demonstration video
└── README.md                    

Complete STM32 Workspace: To access the full auto-generated STM32CubeIDE project workspace (drivers, startup code, and headers) visit this link : https://drive.google.com/file/d/1MSM3QbvPRWi94Gcrqy1bdm5W3tRe6DdV/view?usp=sharing.
