const int leds[] = { 3, 4, 5, 6, 7 };  //{2, 3, 4, 5, 6, 7, 8, 9};
const int buttonPin = 15;
const int shiftButtonPin = 16;
const int potPin = A0;

int potValue = 0;
bool buttonPressed = false;

String inputBuffer = "";  // Buffer to store input text

String macroText = "Hello, World!";
bool macro = false;

enum Mode {
  KEYBOARD,
  MOUSE,
};

enum Mode currentMode = KEYBOARD;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(shiftButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], LOW);
  }

  switchMode();


  if (currentMode == KEYBOARD) {
    getKeyboardInput();
  } else {
    getMouseInput();
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

int getPotValue() {
  potValue = analogRead(potPin) / 32;
  displayBinary(potValue);
  return potValue;
}

void getMouseInput() {
  while (digitalRead(buttonPin) == HIGH) {
    int potValue = getPotValue();
    int angle = (potValue * 360) / 32;

    double radians = angle * (M_PI / 180.0);

    // Calculate X and Y components
    int xDistance = (int)(range * cos(radians));
    int yDistance = (int)(range * sin(radians));


    Serial.print("Mouse.move(");
    Serial.print(xDistance);
    Serial.print(", ");
    Serial.print(yDistance);
    Serial.println(")");
    Serial.println("");
  }
}

void getKeyboardInput() {
  while (digitalRead(buttonPin) == HIGH) {
    int potValue = getPotValue();

    int bufferLength = inputBuffer.length();

    // Send backspace characters to remove previous input
    for (int i = 0; i < bufferLength; i++) {
      //Serial.print('\b');
    }

    inputBuffer = "";

    if (potValue == 31) {
      inputBuffer += macroText;
    } else {
      char inputChar = getChar(potValue);
      inputBuffer += inputChar;
    }
    Serial.print(inputBuffer);
    Serial.print(inputBuffer);
    delay(10);
    if (digitalRead(buttonPin) == LOW) {
      Serial.print("_");
    }
  }
}

void switchMode() {
  if (digitalRead(shiftButtonPin) == HIGH) {
    if (currentMode == KEYBOARD) {
      currentMode = MOUSE;
    } else {
      currentMode = KEYBOARD;
    }
    while (digitalRead(shiftButtonPin) == HIGH) {
      delay(50);
    }
  }
}

void displayBinary(int number) {
  for (int i = 0; i < 5; i++) {
    int bit = bitRead(number, i);
    digitalWrite(leds[i], bit);
  }
}

char getChar(int input) {
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
