#include "worddisplay.h"
#include "vector"

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
    std::vector<CharDisplay> display_word;
    int i = 0;
    auto [c_x, c_y, c_z] = m_color;
    for(char& c: m_word)
    {
        CharDisplay c1(c, m_side, {i * 4.0 * m_side, 0.0, 0.0}, {c_x, c_y, c_z});
        display_word.push_back(c1);
        i++;
    }
    auto [x, y, z] = m_pos;
    // glColor3f(c_x, c_y, c_z);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
    for(CharDisplay cd : display_word)
        cd.generate();   
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}