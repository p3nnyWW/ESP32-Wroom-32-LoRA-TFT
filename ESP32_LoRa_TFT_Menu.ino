
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Ebyte E220 LoRa Module Pin Definitions (UART communication)
#define E220_TX 17           // TX from ESP32 to RX on E220
#define E220_RX 16           // RX on ESP32 from TX on E220
#define E220_M0 18           // Mode pin M0
#define E220_M1 19           // Mode pin M1
#define E220_AUX 34          // Optional AUX pin for status check

// TFT Display Pin Definitions
#define TFT_CS 15            // TFT Chip Select pin
#define TFT_RST 4            // TFT Reset pin
#define TFT_DC 2             // TFT Data/Command pin

// Button Pin Definitions
#define UP_BUTTON 32         // Pin for "Up" button to navigate menu
#define DOWN_BUTTON 33       // Pin for "Down" button to navigate menu
#define SELECT_BUTTON 34     // Pin for "Select" button to choose menu option

// Initialize the TFT display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Initialize Serial2 for E220
HardwareSerial E220Serial(2);

int currentMenuOption = 0;  // Menu option tracker, 0 for Receive, 1 for Send

void setup() {
  Serial.begin(9600); // Initialize Serial for debugging
  E220Serial.begin(9600, SERIAL_8N1, E220_RX, E220_TX);  // Initialize E220 communication

  pinMode(E220_M0, OUTPUT);
  pinMode(E220_M1, OUTPUT);
  setMode(3);    // Set E220 to configuration mode
  delay(100);    // Wait for mode to settle

  // Initialize TFT Display
  tft.begin();
  tft.setRotation(1);        // Set display rotation (adjust as needed)
  tft.fillScreen(ILI9341_BLACK);  // Clear screen with black background

  // Show the welcome screen with ASCII-style text
  displayWelcomeScreen();
  delay(3000);               // Display the welcome message for 3 seconds
  tft.fillScreen(ILI9341_BLACK); // Clear screen after welcome
  
  // Initialize E220 LoRa module after welcome screen
  initializeLoRa();

  // Display main menu after initialization
  displayMenu();

  // Set button pins as input with pull-up resistors
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_BUTTON, INPUT_PULLUP);
}

void loop() {
  // Check for button presses to navigate or select menu options

  if (digitalRead(UP_BUTTON) == LOW) {  // If "Up" button is pressed
    currentMenuOption = (currentMenuOption == 0) ? 1 : 0;  // Toggle option
    displayMenu();           // Update menu display
    delay(200);              // Debounce delay to avoid multiple triggers
  }

  if (digitalRead(DOWN_BUTTON) == LOW) {  // If "Down" button is pressed
    currentMenuOption = (currentMenuOption == 1) ? 0 : 1;  // Toggle option
    displayMenu();           // Update menu display
    delay(200);              // Debounce delay
  }

  if (digitalRead(SELECT_BUTTON) == LOW) { // If "Select" button is pressed
    if (currentMenuOption == 0) {           // Choose Receive Mode
      receiveMode();
    } else if (currentMenuOption == 1) {    // Choose Send Mode
      sendMode();
    }
  }
}

// Display Welcome Screen with ASCII-style "p3nnW" text
void displayWelcomeScreen() {
  // Display "p3nnW" in large text size as a welcome message
  tft.setCursor(20, 40);        // Set initial cursor position
  tft.setTextSize(3);           // Set text size to large
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK); // Text color: Cyan
  tft.println("p3nnW");         // Print "p3nnW" as welcome text in large font
}

// Set mode of the E220 module
void setMode(int mode) {
  switch (mode) {
    case 0: // Normal mode
      digitalWrite(E220_M0, LOW);
      digitalWrite(E220_M1, LOW);
      break;
    case 3: // Configuration mode
      digitalWrite(E220_M0, HIGH);
      digitalWrite(E220_M1, HIGH);
      break;
  }
  delay(10);  // Wait for mode change
}

// Initialize E220 module and display initialization status
void initializeLoRa() {
  Serial.println("Initializing E220...");
  tft.setCursor(20, 30);          
  tft.setTextSize(2);             
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  tft.println("Initializing LoRa...");

  // Send AT commands to configure the E220 (example settings)
  sendATCommand("AT+PARAMETER=9600,8N1,10");  // Example configuration
  sendATCommand("AT+ADDRESS=0");              // Set address

  // Switch to normal mode after configuration
  setMode(0);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.println("LoRa Initialized!");
  delay(1500); // Short delay to show success message
  tft.fillScreen(ILI9341_BLACK); // Clear screen after initialization
}

// Function to send AT command to E220
void sendATCommand(String command) {
  Serial.print("Sending AT command: ");
  Serial.println(command);
  E220Serial.print(command + "\r\n");         // Send AT command followed by newline
  delay(100);

  // Read and display response
  while (E220Serial.available()) {
    String response = E220Serial.readString();
    Serial.print("Response: ");
    Serial.println(response);
  }
}

// Display main menu with "Receive" and "Send" options
void displayMenu() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(20, 30);          // Set cursor for menu text
  tft.setTextSize(3);             // Set text size

  // Highlight the selected menu option with a yellow arrow
  if (currentMenuOption == 0) {    // If "Receive" is selected
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.println("-> Receive");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.println("   Send");
  } else {                         // If "Send" is selected
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.println("   Receive");
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.println("-> Send");
  }
}

// Function for Receive Mode (listens for incoming LoRa messages)
void receiveMode() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

  tft.println("Receiving...");

  // Listen for incoming messages
  if (E220Serial.available()) {
    String receivedMessage = E220Serial.readString();
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(10, 30);
    tft.println("Received:");
    tft.println(receivedMessage);
    delay(2000);
  }
  
  displayMenu();  // Return to the main menu
}

// Function for Send Mode (sends a sample message over LoRa)
void sendMode() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

  tft.println("Sending...");

  // Send a sample message via LoRa
  String message = "Hello from ESP32";
  E220Serial.print(message);
  Serial.println("Message sent: " + message);

  tft.setCursor(10, 60);
  tft.println("Message sent!");

  delay(2000);  // Delay to show confirmation
  displayMenu(); // Return to the main menu
}
