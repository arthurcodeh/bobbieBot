#ifndef MECControlLib_h
#define MECControlLib_h

#include <Arduino.h>

// Enum pour modes de pins (basé sur le firmware)
enum PinMode {
    PIN_MODE_UNUSED,
    PIN_MODE_INPUT,
    PIN_MODE_INPUT_LATCHING_HIGH,
    PIN_MODE_INPUT_LATCHING_LOW,
    PIN_MODE_OUTPUT,
    PIN_MODE_OUTPUT_PWM
  };

// Classe principale pour gérer les pins
class MECControl {
public:
    MECControl();  // Constructeur

    // Analog Pins
    void analogPinMode(byte pin, const char* mode);  // "Input" ou "LatchingInput"
    int analogRead(byte pin);  // Retourne 0-1023 ou 1023/0 pour latching

    // Digital Pins
    void digitalPinMode(byte pin, const char* mode);  // "Input", "InputPullup", "LatchingInput", "LatchingInputPullup", "Output"
    const char* digitalRead(byte pin);  // "High" ou "Low" (string pour compatibilité)
    void digitalWrite(byte pin, const char* state);  // "High" ou "Low"

    // PWM avec ramping
    void analogWrite(byte pin, byte value, unsigned int timeMs);  // Ramping de la valeur actuelle à 'value' en 'timeMs'

    // Général
    void setAcknowledge(bool on);  // Active/désactive les réponses (pour compatibilité, mais pas essentiel en lib)
    void breakCommand();  // Interrompt ramping en cours

private:
    // Variables internes (adaptées du firmware)
    PinMode digitalPinModes[54];  // Pour Uno/Mega
    PinMode analogPinModes[16];
    bool digitalLatchTriggered[54];
    unsigned long digitalLatchDebounceStart[54];
    bool analogLatchTriggered[16];
    unsigned long analogLatchDebounceStart[16];
    byte currentPWMValue[54];
    byte startPWMValue[54];
    byte endPWMValue[54];
    unsigned int pwmDuration[54];
    unsigned long pwmStartTime[54];

    const unsigned int latchDebounceInterval = 500;  // 500ms debounce

    void updatePWM();  // Méthode interne pour mettre à jour PWM en loop
    void updateLatches();  // Méthode pour mettre à jour les latches
};

#endif
