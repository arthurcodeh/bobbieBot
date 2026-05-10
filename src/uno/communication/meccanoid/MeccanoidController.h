#pragma once
#include "../../members/Member.h"

#define MAX_MEMBERS 10

class MeccanoidController {
private:
    Membre* membres[MAX_MEMBERS];
    uint8_t memberCount = 0;

public:
    void add_member(Membre* m);
    void update() const;
};
