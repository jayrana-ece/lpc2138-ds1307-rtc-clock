# LPC2138 DS1307 RTC Digital Clock

An embedded real-time clock project using the **LPC2138 ARM7 microcontroller**, DS1307 real-time clock (RTC), and a 16×2 LCD. The system was developed in Embedded C using Keil µVision and simulated in Proteus.

## Project Overview

The DS1307 maintains time and date information and communicates with the LPC2138 through the I²C interface. The microcontroller reads the RTC registers, processes the time and date values, and displays them on the LCD.

This project demonstrates microcontroller interfacing, I²C communication, RTC register handling, LCD interfacing, and embedded C programming.

## Features

- Real-time clock and date display using the DS1307.
- I²C communication between the LPC2138 and RTC.
- 16×2 LCD interfaced in 4-bit mode.
- Embedded C firmware developed using Keil µVision.
- Circuit design and functional testing through Proteus simulation.
- Configurable initial time and date in the firmware.

## Hardware Components in the Simulation

| Component | Purpose |
|---|---|
| LPC2138 | ARM7 microcontroller and main controller |
| DS1307 | Real-time clock and calendar |
| 16×2 LCD | Displays time and date |
| 32.768 kHz crystal | RTC timekeeping reference |
| Pull-up resistors | I²C bus pull-ups |
| Backup battery | RTC backup supply |
| Power and ground terminals | Circuit power connections |

## Software and Tools

- Keil µVision — Embedded C development and compilation.
- Proteus — Circuit design and simulation.
- Embedded C — Firmware implementation.

## Circuit Connections

### 1. LPC2138 to DS1307

| LPC2138 pin | DS1307 pin |
|---|---|
| P0.2 / SCL0 | SCL |
| P0.3 / SDA0 | SDA |
| 3.3 V supply rail | VCC, according to the circuit's supply design |
| GND | GND |

Additional RTC connections:

- Connect the 32.768 kHz crystal between X1 and X2.
- Connect pull-up resistors from SDA and SCL to the appropriate logic supply.
- Connect VBAT to a suitable backup battery if backup operation is required.
- Leave SOUT unconnected if square-wave output is not used.

**Note:** Verify the RTC supply voltage and logic-level compatibility against the DS1307 datasheet and the actual Proteus circuit before hardware implementation.

### 2. LPC2138 to 16×2 LCD

The LCD operates in 4-bit mode.

| LCD signal | LPC2138 connection |
|---|---|
| RS | P0.16 |
| E | P0.17 |
| D4 | P0.18 |
| D5 | P0.19 |
| D6 | P0.20 |
| D7 | P0.21 |
| RW | GND |
| VSS | GND |
| VDD | LCD supply |
| V0 | Contrast adjustment |

LCD data pins D0–D3 are not used in 4-bit mode.

## Working Principle

1. The LPC2138 initializes the required GPIO and I²C settings.
2. The firmware writes an initial time and date to the DS1307 when configured to do so.
3. The DS1307 maintains time using its clock source.
4. The LPC2138 reads the RTC registers over I²C.
5. The firmware converts the RTC values into displayable characters.
6. The LCD displays the current time and date.
7. The process repeats to refresh the display.

The DS1307 stores its time and date values in BCD format. The firmware must handle the register values correctly before displaying them.

## Initial Simulation Configuration

The previously used initial values were:

- Time: 08:30:00
- Date: 22/08/2026
- DS1307 write address: `0xD0`
- DS1307 read address: `0xD1`

The addresses above use the 8-bit address convention, including the R/W bit. The corresponding 7-bit I²C address is `0x68`.

These are initial firmware settings, not a claim that the RTC always starts at this time.

## Project Structure

- `KEIL/` — Keil project, Embedded C source, and required project files.
- `PROTEUS/` — Proteus schematic and required simulation files.
- `Images/` — Circuit, Keil, and simulation screenshots.
- `docs/` — Optional project report.

## Simulation and Verification

The project was developed and evaluated in a software simulation environment.

Suggested verification cases:

1. Confirm that the circuit starts without simulation errors.
2. Check that the LCD initializes and displays time and date.
3. Observe the seconds field changing over time.
4. Verify the date and time values against the RTC registers.
5. Check that the displayed values remain correctly formatted.

Screenshots should document the actual results obtained during simulation.

**Testing limitation:** This repository documents software development and Proteus simulation. Physical circuit assembly and real-hardware testing have not been claimed.

## Future Improvements

- Add buttons to set the time and date.
- Add an alarm function.
- Improve LCD refresh timing.
- Add a weekday display.
- Validate operation on a physical LPC2138 and DS1307 circuit.

## Author

Jay Rana
