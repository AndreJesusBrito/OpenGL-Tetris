#ifndef WORDDISPLAY_H
#define WORDDISPLAY_H

#include <tuple>
#include <stdexcept>

#include "chardisplay.h"

class WordDisplay
{
    public:
    private:
        std::string m_word;
        double m_side;
        CharDisplay::TripleDouble m_pos{0.0, 0.0, 0.0};
        CharDisplay::TripleDouble m_color{0.0, 0.0, 0.0};
    public:
    WordDisplay() = delete;
    WordDisplay(std::string word, double side, CharDisplay::TripleDouble pos, CharDisplay::TripleDouble color);
    WordDisplay(std::string word, double side);

    void changeColor(CharDisplay::TripleDouble color);
    void changeColor(double red, double green, double blue);

    void move(CharDisplay::TripleDouble pos);
    void move(double x, double y, double z);

    void generate();
};

#endif