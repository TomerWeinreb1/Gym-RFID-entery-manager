#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // Configurable, see typical pin layout above
#define SS_PIN 10   // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
String receivedString = "";         // To hold the incoming string from CVI
String customerName = "Customer Not Recognized"; // Default name if the chip is not initialized
int customerAge = -1;               // Default age if not initialized
String customerAddress = "Address Not Set"; // Default address if not initialized
boolean isChipInitialized = false;  // Flag to check if the chip is initialized
boolean awaitingNewName = false;    // Flag to indicate that the next input is the new customer name
boolean awaitingNewAge = false;     // Flag to indicate that the next input is the new customer age
boolean awaitingNewAddress = false; // Flag to indicate that the next input is the new customer address

void setup() {
  Serial.begin(9600);   // Initialize serial communications
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  Serial.println("Arduino is ready.");
}

void loop() {
  // Check if there's a serial input from CVI
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (incomingChar == '\n') {  // End of command
      if (awaitingNewName) {
        // The received string is the new customer name
        customerName = receivedString;
        awaitingNewName = false;
        awaitingNewAge = true;
        Serial.println("Customer name updated to: " + customerName);
        Serial.println("Please send the customer's age.");
      }
      else if (awaitingNewAge) {
        // The received string is the new customer age
        customerAge = receivedString.toInt();
        awaitingNewAge = false;
        awaitingNewAddress = true;
        Serial.println("Customer age updated to: " + String(customerAge));
        Serial.println("Please send the customer's address.");
      }
      else if (awaitingNewAddress) {
        // The received string is the new customer address
        customerAddress = receivedString;
        isChipInitialized = true;
        Serial.println("Customer address updated to: " + customerAddress);
        writeCustomerDataToChip();
        awaitingNewAddress = false;
      }
      else if (receivedString.equals("new")) {
        // Prepare to receive the new customer name
        awaitingNewName = true;
        Serial.println("Please send the new customer name.");
      }
      receivedString = "";  // Clear the received string for the next input
    }
    else {
      receivedString += incomingChar;  // Append the character to the string
    }
  }

  // Look for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (isChipInitialized) {
      // Read the customer name, age, and address from the chip (assuming it's stored)
      String storedName = readCustomerNameFromChip();
      int storedAge = readCustomerAgeFromChip();
      String storedAddress = readCustomerAddressFromChip();
      Serial.println("Customer Name: " + storedName);
      Serial.println("Customer Age: " + String(storedAge));
      Serial.println("Customer Address: " + storedAddress);
    } else {
      Serial.println(customerName); // Default response if the chip is not initialized
    }
    delay(1000); // Add a delay to avoid repeated reading
    mfrc522.PICC_HaltA(); // Stop reading
  }
}

// Placeholder function to write the customer data (name, age, address) to the RFID chip
void writeCustomerDataToChip() {
  // Implement the logic to write customerName, customerAge, and customerAddress to the RFID chip.
  // This is a placeholder; actual writing depends on your RFID chip and library.
}

// Placeholder function to read the customer name from the RFID chip
String readCustomerNameFromChip() {
  // Implement the logic to read customerName from the RFID chip.
  // This is a placeholder; actual reading depends on your RFID chip and library.
  return customerName; // Return the current customerName for demonstration
}

// Placeholder function to read the customer age from the RFID chip
int readCustomerAgeFromChip() {
  // Implement the logic to read customerAge from the RFID chip.
  // This is a placeholder; actual reading depends on your RFID chip and library.
  return customerAge; // Return the current customerAge for demonstration
}

// Placeholder function to read the customer address from the RFID chip
String readCustomerAddressFromChip() {
  // Implement the logic to read customerAddress from the RFID chip.
  // This is a placeholder; actual reading depends on your RFID chip and library.
  return customerAddress; // Return the current customerAddress for demonstration
}

