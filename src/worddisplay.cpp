#include "worddisplay.h"

WordDisplay::WordDisplay(std::string word, double side, CharDisplay::TripleDouble pos, CharDisplay::TripleDouble color)
    : m_word{word}, m_side{side}, m_pos{pos}, m_color{color}{}


void WordDisplay::changeColor(CharDisplay::TripleDouble color) {
    m_color = color;
}

void WordDisplay::changeColor(double red, double green, double blue) {
    changeColor(std::make_tuple(red, green, blue));
}


void WordDisplay::move(CharDisplay::TripleDouble pos) {
    m_pos = pos;
}

void WordDisplay::move(double x, double y, double z) {
    move(std::make_tuple(x, y, z));
}


void WordDisplay::generate()
{
    CharDisplay ch('I', m_side);
    ch.generate();
}