# SLRhythm
**SLRhythm** is a high-performance rhythm game developed in C for the **PIC18LF4550** microcontroller. Designed for the **Graphic Kit 1**, it features real-time tile rendering, multi-column gameplay, and precise accuracy tracking using fixed-point arithmetic.

## 🚀 Features
* **4-Column Gameplay:** Classic rhythm mechanics with tiles falling across four distinct lanes.
* **State Machine Logic:** Seamless transitions between a graphical menu and active gameplay.
* **Dynamic Level System:** Support for multiple "charts" (levels) with varying difficulty and patterns.
* **Accuracy Engine:** Real-time accuracy calculation using fixed-point math ($fx8$) for high performance on 8-bit hardware.
* **Persistent High Scores:** Tracks the best performance for each map during the session.
* **Responsive Input:** Polled hit-scanning every 5ms and a 1ms system clock (via Timer 2) for low-latency feedback.

## 🛠️ Hardware Requirements
* **MCU:** Microchip PIC18LF4550.
* **Display:** DOGM128 (128x64 pixels) via SPI.
* **Peripheral Kit:** Graphic Kit 1 (utilizing buttons RB0, RB2-RB5 and on-board LEDs).
* **Clock:** External XTAL.

## 💻 Software Stack
* **Language:** C
* **Compiler:** Microchip XC8
* **IDE:** MPLAB X
* **Libraries:** * `moduleDogm128`: Display driver.
    * `fx8.h`: Fixed-point math library for performance optimization.

---

## 🎮 How to Play

### Controls
| Button | Action |
| :--- | :--- |
| **RB5 (SW1)** | Lane 1 / Menu Up |
| **RB4 (SW2)** | Lane 2 / Menu Down |
| **RB3 (SW4)** | Lane 3 |
| **RB0 (SW5)** | Lane 4 / Menu Select |
| **RB2 (SW3)** | Reset Game / Hold (1s) to Return to Menu |

### Mechanics
1.  **Selection:** Navigate the menu to choose between three levels (Level 1, Level 2, or Level 3).
2.  **Hitting Tiles:** Press the corresponding lane button when a tile reaches the hit zone at the bottom of the screen.
3.  **Long Tiles:** For tiles with length ($len > 0$), you must hit the start and release accurately at the end.
4.  **Scoring:** Your total score and accuracy percentage are displayed in real-time at the top of the screen.

---

## 📂 Project Structure
* `main.c`: Core state machine, interrupt service routines, and UI rendering loops.
* `gameLogic.c/h`: The "brain" of the game—handles tile spawning, hit detection, and accuracy math.
* `level.h`: Contains hardcoded level data (charts) for the game maps.
* `mcu.h`: Pinout definitions and hardware-specific macros for the PIC18.
* `fx8.h`: Custom fixed-point implementation to avoid floating-point overhead.

---

## 🔧 Technical Details

### Timing & Interrupts
The game utilizes **Timer 2** to generate a millisecond heartbeat (`millis`). 
* **Hit Scanning:** Performed every 5ms.
* **Frame Rate:** The display updates every 20ms (~50 FPS) to ensure smooth tile movement.

### Accuracy Calculation
Accuracy is calculated as a ratio of successful hits to total passed tiles, converted to a percentage using fixed-point multiplication:
$$Accuracy = \frac{PassedTiles - Misses}{PassedTiles} \times 100$$

### Tile Lifecycle
Tiles are managed in a circular-style buffer for each column. They are spawned into the active buffer once the `millis` timer reaches their `start` time minus a look-ahead window, and are despawned once they move off-screen.

---

## 📝 Authors
* **LM**
* **JS**
* *Created: March 2026*