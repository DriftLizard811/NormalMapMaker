#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include <iostream>
#include "MathLib.h"

const int MIN_CHANNEL_VALUE = 0;
const int MAX_CHANNEL_VALUE = 255;

class Color
{
    public:
        Color() { R = 0; G = 0; B = 0; }
        Color(int r, int g, int b) { R = r; G = g; B = b; }

        void DebugPrint() { std::cout << "R: " << R << ", G: " << G << ", B: " << B << std::endl; }
        void Output(std::ofstream& out) { out << R << std::endl << G << std::endl << B << std::endl; }
    
        //accessors
        int getR() { return R; }
        int getG() { return G; }
        int getB() { return B; }

        //mutators
        void setR(int r) {R = clamp(r, MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE); };
        void setG(int g) {G = clamp(g, MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE); };
        void setB(int b) {B = clamp(b, MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE); };

    private:
        int R = 0;
        int G = 0;
        int B = 0;
};

#endif