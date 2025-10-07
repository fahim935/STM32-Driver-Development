#Author  - Fahim Akhtar
# STM32F407 Bare-Metal Driver Suite

Complete bare-metal peripheral driver suite for STM32F407 microcontroller, developed from scratch without HAL/LL libraries. This project demonstrates professional embedded systems development through direct hardware register manipulation.

## 🚀 Featured Drivers

|  Driver  |  Status        |  Features |
|----------|----------------|-----------|
| **GPIO** | 🔄 In Progress | Pin configuration, interrupt handling, alternate function |
| **SPI**  | 🔄 In Progress | Master/slave modes, DMA support, error handling |
| **I2C**  | ⏳ Planned     | Multi-master, SMBus, clock stretching |
| **UART** | ⏳ Planned     | Polling/Interrupt/DMA modes, flow control |
| **USART**| ⏳ Planned     | Synchronous mode, LIN, smartcard |
| **RCC**  | ✅ Partial     | Clock system configuration |

## 📋 Project Overview

This repository contains a complete set of bare-metal drivers for STM32F407, implementing:

- **Zero-dependency architecture**  - No HAL, no external libraries
- **Register-level programming**    - Direct hardware access
- **Comprehensive error handling**  - Robust driver implementation
- **Modular design**                - Reusable, maintainable code structure
- **Practical examples**            - Real-world usage demonstrations

## 🛠️ Technical Stack

- **MCU**      : STM32F407VGT6 (ARM Cortex-M4)
- **Toolchain**: ARM GCC, STM32CubeIDE
- **Debugging**: ST-Link, Logic Analyzer, Oscilloscope
- **Protocols**: SPI, I2C, UART, USART
- **Standards**: MISRA C compliance where applicable

## 📁 Repository Structure
STM32F407-Bare-Metal-Drivers/
- ├── docs/ # Technical documentation
- ├── drivers/ # Complete driver suite
- │ ├── inc/ # Header files
- │ └── src/ # Implementation files
- ├── projects/ # Practical applications
- ├── utils/ # Common utilities
- └── README.md # This file

##🚀 Prerequisites
- STM32F407 Discovery Board
- STM32CubeIDE or ARM GCC toolchain
- ST-Link programmer/debugger

##🎓 Skills Demonstrated
- This project showcases embedded systems expertise sought by top semiconductor companies:
    - Low-Level Hardware Programming - Direct register manipulation
    - Protocol Implementation - SPI, I2C, UART from datasheet
    - System Architecture - Modular driver design
    - Debugging & Validation - Hardware verification techniques