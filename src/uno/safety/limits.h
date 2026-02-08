#pragma once

struct Limits {
    int min;
    int max;

    int clamp(int value) const {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
};
