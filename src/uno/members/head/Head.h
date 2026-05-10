#pragma once
#include "../Member.h"

class Tete : public Membre {
private:
    static ServoSpec headSpecs[1];

public:
    explicit Tete(const int pin)
        : Membre(pin, "head", headSpecs, 1) {}
};
