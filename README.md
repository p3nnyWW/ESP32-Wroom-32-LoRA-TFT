
# ESP32 Communication Project with Ebyte E220-900T22D LoRa Module and TFT Display

This project demonstrates the use of an **ESP32 microcontroller** to communicate with an **Ebyte E220-900T22D LoRa module** using UART, display messages on a **TFT screen** (ILI9341), and interact with a simple menu controlled by buttons.

The project supports two main functions:
1. **Send Mode**: Transmits a sample message via LoRa.
2. **Receive Mode**: Listens for and displays incoming LoRa messages.

The project also includes a basic user interface with a welcome screen and a menu for selecting "Receive" or "Send" modes.

## Components Required

- **ESP32 Development Board**
- **Ebyte E220-900T22D LoRa Module**
- **TFT Display (ILI9341)** – 240x320 pixels
- **Three push buttons** for menu navigation
- **Connecting wires** for wiring the components

## Pin Connections

### Ebyte E220-900T22D LoRa Module

| Module Pin | ESP32 Pin | Description                     |
|------------|-----------|---------------------------------|
| VCC        | 3.3V      | Power                           |
| GND        | GND       | Ground                          |
| TX         | GPIO 16   | LoRa RX (to ESP32 TX)          |
| RX         | GPIO 17   | LoRa TX (to ESP32 RX)          |
| M0         | GPIO 18   | Mode control pin 0             |
| M1         | GPIO 19   | Mode control pin 1             |
| AUX        | GPIO 34   | Optional AUX pin for status    |

### TFT Display (ILI9341)

| TFT Pin    | ESP32 Pin | Description                     |
|------------|-----------|---------------------------------|
| VCC        | 3.3V      | Power                           |
| GND        | GND       | Ground                          |
| CS         | GPIO 15   | Chip Select                     |
| RESET      | GPIO 4    | Reset                           |
| DC         | GPIO 2    | Data/Command                    |
| MOSI       | GPIO 23   | SPI MOSI (shared with touchscreen) |
| SCK        | GPIO 18   | SPI Clock (shared with touchscreen) |
| LED        | 3.3V      | Backlight power                 |

### Buttons

| Button       | ESP32 Pin | Description                    |
|--------------|-----------|--------------------------------|
| UP_BUTTON    | GPIO 32   | Navigate up in the menu        |
| DOWN_BUTTON  | GPIO 33   | Navigate down in the menu      |
| SELECT_BUTTON| GPIO 34   | Select menu option             |

## Installation and Setup

1. **Install Arduino Libraries**:
   - Install the following libraries via the Arduino Library Manager:
     - **Adafruit GFX Library**: For graphical functions on the TFT display.
     - **Adafruit ILI9341 Library**: For controlling the ILI9341 TFT display.

2. **Upload the Code**:
   - Open `ESP32_LoRa_TFT_Menu.ino` in the Arduino IDE.
   - Select the correct **Board** and **Port** in the **Tools** menu (e.g., `ESP32 Dev Module`).
   - Press the **Upload** button to flash the code to the ESP32.

3. **Wiring**:
   - Connect the components according to the pin connections outlined above.

## Project Structure

### Main Functions

- **displayWelcomeScreen()**: Displays a welcome message on the TFT screen when the device powers on.
- **setMode(int mode)**: Sets the operating mode of the E220 LoRa module. Modes include:
  - `Mode 0`: Normal communication mode for sending/receiving messages.
  - `Mode 3`: Configuration mode for sending AT commands to the module.
- **initializeLoRa()**: Configures the E220 module by sending AT commands for baud rate, address, etc., then switches to Normal Mode.
- **displayMenu()**: Displays a simple menu on the TFT screen with options for "Receive" and "Send" modes.
- **receiveMode()**: Activates the Receive Mode to listen for incoming LoRa messages. Displays the received message on the TFT screen.
- **sendMode()**: Activates the Send Mode to transmit a sample message over LoRa. Confirms the message was sent on the TFT screen.
- **sendATCommand(String command)**: Sends an AT command to the E220 module and prints the response to the Serial Monitor for debugging.

### Menu Navigation

- **UP_BUTTON**: Scroll up through the menu options.
- **DOWN_BUTTON**: Scroll down through the menu options.
- **SELECT_BUTTON**: Selects the current menu option (either "Receive" or "Send").

## Usage

1. **Power On**: The device shows a welcome screen on the TFT display.
2. **Menu Navigation**:
   - Use the `UP_BUTTON` and `DOWN_BUTTON` to navigate between "Receive" and "Send" options.
   - Press the `SELECT_BUTTON` to choose the highlighted option.
3. **Receive Mode**:
   - In Receive Mode, the ESP32 listens for LoRa messages. Any received message is displayed on the TFT screen.
   - After displaying the message, the device returns to the main menu.
4. **Send Mode**:
   - In Send Mode, the ESP32 sends a sample message ("Hello from ESP32") over LoRa.
   - After sending, the TFT screen displays a confirmation, then returns to the main menu.

## Troubleshooting

- **Serial Data Errors**: If you encounter errors such as "Serial data stream stopped," try the following:
  - Check your USB cable and port.
  - Ensure all components are wired correctly.
  - Press and hold the **BOOT** button during the upload, if necessary.
- **No Display on TFT**: Double-check the wiring, especially the VCC, GND, and control pins (CS, DC, RESET).
- **E220 Configuration Fails**: Verify the M0 and M1 pins are set correctly and that the E220 is in Configuration Mode when sending AT commands.

## Notes

- The **E220-900T22D LoRa module** uses UART, so make sure the `E220_TX` and `E220_RX` pins are correctly connected to ESP32's RX and TX respectively.
- **AT Commands**: You can modify AT commands in `initializeLoRa()` based on your specific network and configuration requirements. Refer to the [Ebyte E220 manual](https://www.ebyte.com/en/product-view-news.aspx?id=180) for more details on configuration options.

## License

This project is open-source and available under the MIT License.

## Acknowledgments

- **Adafruit** for their GFX and ILI9341 libraries, which make working with graphical displays on microcontrollers straightforward.
- **Ebyte** for the E220-900T22D LoRa module, providing long-range communication capabilities in UART format.

---

This README provides a full overview of the project’s setup, operation, and troubleshooting tips. Enjoy experimenting with LoRa communication on your ESP32!
