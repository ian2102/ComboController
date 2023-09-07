const int leds[] = { 3, 4, 5, 6, 7 };  //{2, 3, 4, 5, 6, 7, 8, 9};
const int buttonPin = 15;
const int shiftButtonPin = 16;
const int potPin = A0;

int potValue = 0;
bool buttonPressed = false;

String inputBuffer = "";  // Buffer to store input text

String macroText = "Hello, World!";
bool macro = false;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(shiftButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}


void loop() {
  while (digitalRead(buttonPin) == HIGH) {

    potValue = analogRead(potPin) / 32;
    displayBinary(potValue);

    // Calculate the length of inputBuffer
    int bufferLength = inputBuffer.length();

    // Send backspace characters to remove previous input
    for (int i = 0; i < bufferLength; i++) {
      Serial.print('\b');
    }

    inputBuffer = "";

    if (potValue == 31) {
      inputBuffer += macroText;
      macro = true;
    } else {
      char inputChar = getChar(potValue);
      inputBuffer += inputChar;
    }
    Serial.println(inputBuffer);
    delay(10);
    if (digitalRead(buttonPin) == LOW) {
      Serial.println(" ");
    }
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], HIGH);
  }
}


void displayBinary(int number) {
  for (int i = 0; i < 5; i++) {
    int bit = bitRead(number, i);
    digitalWrite(leds[i], bit);
  }
}

char getChar(int input) {
  if (digitalRead(shiftButtonPin) == HIGH) {
    // Handle shifted characters here (e.g., uppercase letters)
    switch (input) {
      case 0 ... 25:
        return char('A' + input);  // Convert value 0-25 to 'A'-'Z'
      /*
      case 26:
        return ' ';  // Space character
      case 27:
        return '\n';  // Enter/Return character
      case 28:
        return '\t';  // Tab character
      case 29:
        return '\b';  // Backspace character
      case 30:
        return '\0';  // Null character
      */
      default:
        return '?';  // Return a placeholder character for other values
    }
  } else {
    // Handle regular characters here (e.g., lowercase letters)
    switch (input) {
      case 0 ... 25:
        return char('a' + input);  // Convert value 0-25 to 'a'-'z'
      /*
      case 26:
        return ' ';  // Space character
      case 27:
        return '\n';  // Enter/Return character
      case 28:
        return '\t';  // Tab character
      case 29:
        return '\b';  // Backspace character
      case 30:
        return '\0';  // Null character
      */
      default:
        return '?';  // Return a placeholder character for other values
    }
  }
}
