const int leds[] = { 3, 4, 5, 6, 7 };  //{2, 3, 4, 5, 6, 7, 8, 9};
const int buttonPin = 15;
const int shiftButtonPin = 16;
const int potPin = A0;

const int modeLed = 9;

const int range = 40;

int potValue = 0;

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
  switchMode();

  if (currentMode == KEYBOARD) {
    getKeyboardInput();
    digitalWrite(modeLed, KEYBOARD);
  } else {
    getMouseInput();
    digitalWrite(modeLed, MOUSE);
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

    int xDistance = (int)(range * cos(radians));
    int yDistance = (int)(range * sin(radians));

    // arduino nano doesnt support Mouse.move
    // print the change for now
    // Digispark attiny85 does
    Serial.print("Mouse.move(");
    Serial.print(xDistance);
    Serial.print(", ");
    Serial.print(yDistance);
    Serial.println(")");
    Serial.println("");
    delay(100);
  }
}

void getKeyboardInput() {
  while (digitalRead(buttonPin) == HIGH) {
    int potValue = getPotValue();

    int bufferLength = inputBuffer.length();

    // Send backspace characters to remove previous input
    for (int i = 0; i < bufferLength; i++) {
      Serial.print('\b');
    }

    inputBuffer = "";

    if (potValue == 31) {
      inputBuffer += macroText;
    } else {
      char inputChar = getChar(potValue);
      inputBuffer += inputChar;
    }
    Serial.println(inputBuffer);
    delay(10);
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("_");
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
    /* todo implement some more characters
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
