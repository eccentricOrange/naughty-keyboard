# Automatic Keyboard
This is a simple project written for an STM32F411 Black Pill board. It uses the USB HID protocol to send keystrokes to a computer, using STM32 HAL library.

## Usage
1.  Install STM32Cube Programmer: https://www.st.com/en/development-tools/stm32cubeprog.html
2.  Install the ARM GCC toolchain: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
3.  Build the project using the make file:

    ```bash
    make all
    ```

4.  Connect the STM32F411 board to your computer using an ST-Link programmer.
5.  Flash the board using the make file:

    ```bash
    /path/to/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=SWD -w build/keyboard.bin 0x8000000
    ```

6.  Connect the USB port of the board to your computer. The board should now be recognized as a keyboard by your computer. You may have to reset the board after flashing it.

## Assumptions
*   The board is connected to a computer using the USB port.
*   You are using a standard English keyboard layout.
*   You are running Windows or Ubuntu.
*   You have Google Chrome installed and set up with at least two profiles.