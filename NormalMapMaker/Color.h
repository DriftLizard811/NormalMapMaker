#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include <iostream>

struct Color
{
    Color() { R = 0; G = 0; B = 0; }
    Color(int r, int g, int b) { R = r; G = g; B = b; }

    void DebugPrint() { std::cout << "R: " << R << ", G: " << G << ", B: " << B << std::endl; }
    void Output(std::ofstream& out) { out << R << std::endl << G << std::endl << B << std::endl; }

    int R = 0;
    int G = 0;
    int B = 0;
};

#endif