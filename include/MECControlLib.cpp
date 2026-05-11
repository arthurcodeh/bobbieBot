#include "MECControlLib.h"

// Constructeur
MECControl::MECControl() {
  // Initialisation des pins (basé sur setup() du firmware)
  for (byte pin = 2; pin <= 53; pin++) {  // Pins digitaux 2-53 (Uno/Mega)
    digitalPinModes[pin] = PIN_MODE_UNUSED;
    pinMode(pin, INPUT);
    digitalLatchTriggered[pin] = false;
    digitalLatchDebounceStart[pin] = 0;
    currentPWMValue[pin] = 0;
    startPWMValue[pin] = 0;
    endPWMValue[pin] = 0;
    pwmDuration[pin] = 0;
    pwmStartTime[pin] = 0;
  }
  for (byte pin = 0; pin <= 15; pin++) {  // Pins analogiques 0-15
    analogPinModes[pin] = PIN_MODE_INPUT;
    analogLatchTriggered[pin] = false;
    analogLatchDebounceStart[pin] = 0;
  }
}

// Analog Pins
void MECControl::analogPinMode(byte pin, const char* mode) {
  if (strcmp(mode, "Input") == 0) {
    analogPinModes[pin] = PIN_MODE_INPUT;
  } else if (strcmp(mode, "LatchingInput") == 0) {
    analogLatchTriggered[pin] = false;
    if (analogRead(pin) > 255) {
      analogPinModes[pin] = PIN_MODE_INPUT_LATCHING_LOW;
    } else {
      analogPinModes[pin] = PIN_MODE_INPUT_LATCHING_HIGH;
    }
  }
}

int MECControl::analogRead(byte pin) {
  if (analogPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH || analogPinModes[pin] == PIN_MODE_INPUT_LATCHING_LOW) {
    if (analogPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH) {
      if (analogLatchTriggered[pin]) return 1023;
      else return 0;
    } else {
      if (analogLatchTriggered[pin]) return 0;
      else return 1023;
    }
    analogLatchTriggered[pin] = false;  // Reset latch
  } else {
    return ::analogRead(pin);  // Lecture standard
  }
}

// Digital Pins
void MECControl::digitalPinMode(byte pin, const char* mode) {
  currentPWMValue[pin] = 0;
  if (strcmp(mode, "Input") == 0) {
    digitalPinModes[pin] = PIN_MODE_INPUT;
    ::pinMode(pin, INPUT);
  } else if (strcmp(mode, "InputPullup") == 0) {
    digitalPinModes[pin] = PIN_MODE_INPUT;
    ::pinMode(pin, INPUT_PULLUP);
  } else if (strcmp(mode, "LatchingInput") == 0) {
    ::pinMode(pin, INPUT);
    digitalLatchTriggered[pin] = false;
    if (::digitalRead(pin)) {
      digitalPinModes[pin] = PIN_MODE_INPUT_LATCHING_LOW;
    } else {
      digitalPinModes[pin] = PIN_MODE_INPUT_LATCHING_HIGH;
    }
  } else if (strcmp(mode, "LatchingInputPullup") == 0) {
    ::pinMode(pin, INPUT_PULLUP);
    digitalLatchTriggered[pin] = false;
    if (::digitalRead(pin)) {
      digitalPinModes[pin] = PIN_MODE_INPUT_LATCHING_LOW;
    } else {
      digitalPinModes[pin] = PIN_MODE_INPUT_LATCHING_HIGH;
    }
  } else if (strcmp(mode, "Output") == 0) {
    digitalPinModes[pin] = PIN_MODE_OUTPUT;
    ::pinMode(pin, OUTPUT);
  }
}

const char* MECControl::digitalRead(byte pin) {
  if (digitalPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH || digitalPinModes[pin] == PIN_MODE_INPUT_LATCHING_LOW) {
    if (digitalPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH) {
      if (digitalLatchTriggered[pin]) return "High";
      else return "Low";
    } else {
      if (digitalLatchTriggered[pin]) return "Low";
      else return "High";
    }
    digitalLatchTriggered[pin] = false;  // Reset latch
  } else {
    return ::digitalRead(pin) ? "High" : "Low";
  }
}

void MECControl::digitalWrite(byte pin, const char* state) {
  digitalPinModes[pin] = PIN_MODE_OUTPUT;
  if (strcmp(state, "High") == 0 || strcmp(state, "1") == 0) {
    currentPWMValue[pin] = 255;
    ::digitalWrite(pin, HIGH);
  } else if (strcmp(state, "Low") == 0 || strcmp(state, "0") == 0) {
    currentPWMValue[pin] = 0;
    ::digitalWrite(pin, LOW);
  }
}

// PWM avec ramping
void MECControl::analogWrite(byte pin, byte value, unsigned int timeMs) {
  digitalPinModes[pin] = PIN_MODE_OUTPUT_PWM;
  startPWMValue[pin] = currentPWMValue[pin];
  endPWMValue[pin] = value;
  pwmStartTime[pin] = millis();
  pwmDuration[pin] = timeMs;
}

// Méthode interne pour mettre à jour PWM (appelle ça dans loop() de ton sketch)
void MECControl::updatePWM() {
  for (byte pin = 2; pin <= 53; pin++) {
    if (digitalPinModes[pin] == PIN_MODE_OUTPUT_PWM && currentPWMValue[pin] != endPWMValue[pin]) {
      if (millis() - pwmStartTime[pin] >= pwmDuration[pin]) {
        currentPWMValue[pin] = endPWMValue[pin];
      } else {
        currentPWMValue[pin] = startPWMValue[pin] + ((float)(millis() - pwmStartTime[pin]) / pwmDuration[pin]) * (endPWMValue[pin] - startPWMValue[pin]);
      }
      ::analogWrite(pin, currentPWMValue[pin]);
    }
  }
}

// Gestion des latches (appelle ça dans loop() de ton sketch)
void MECControl::updateLatches() {
  for (byte pin = 2; pin <= 53; pin++) {
    if (digitalPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH) {
      if (::digitalRead(pin) && millis() - digitalLatchDebounceStart[pin] > latchDebounceInterval) {
        digitalLatchTriggered[pin] = true;
        digitalLatchDebounceStart[pin] = millis();
      }
    } else if (digitalPinModes[pin] == PIN_MODE_INPUT_LATCHING_LOW) {
      if (!::digitalRead(pin) && millis() - digitalLatchDebounceStart[pin] > latchDebounceInterval) {
        digitalLatchTriggered[pin] = true;
        digitalLatchDebounceStart[pin] = millis();
      }
    }
  }
  for (byte pin = 0; pin <= 15; pin++) {
    if (analogPinModes[pin] == PIN_MODE_INPUT_LATCHING_HIGH) {
      if (::analogRead(pin) > 255 && millis() - analogLatchDebounceStart[pin] > latchDebounceInterval) {
        analogLatchTriggered[pin] = true;
        analogLatchDebounceStart[pin] = millis();
      }
    } else if (analogPinModes[pin] == PIN_MODE_INPUT_LATCHING_LOW) {
      if (::analogRead(pin) < 256 && millis() - analogLatchDebounceStart[pin] > latchDebounceInterval) {
        analogLatchTriggered[pin] = true;
        analogLatchDebounceStart[pin] = millis();
      }
    }
  }
}

// Général
void MECControl::setAcknowledge(bool on) {
  // Pas essentiel en lib, mais pour compatibilité
}

void MECControl::breakCommand() {
  for (byte pin = 2; pin <= 53; pin++) {
    if (digitalPinModes[pin] == PIN_MODE_OUTPUT_PWM) {
      endPWMValue[pin] = currentPWMValue[pin];
    }
  }
}
