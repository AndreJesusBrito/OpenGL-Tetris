#include "chardisplay.h"


CharDisplay::CharDisplay(char ch, double side, TripleDouble pos, TripleDouble color)
    :  m_side{side}, m_pos{pos}, m_color{color} {
    switch (ch) {
        case 'A': break;
        case 'B': break;
        case 'C': break;
        case 'D': break;
        case 'E':
            m_pixelMatrix = { 1, 1, 1,
                              1, 0, 0,
                              1, 1, 1,
                              1, 0, 0,
                              1, 1, 1 };
            break;
        case 'F': break;
        case 'G': break;
        case 'H': break;
        case 'I': 
            m_pixelMatrix = { 1, 1, 1,
                              0, 1, 0,
                              0, 1, 0,
                              0, 1, 0,
                              1, 1, 1 };
            break;
        case 'J': break;
        case 'K': break;
        case 'L': 
            m_pixelMatrix = { 1, 0, 0,
                              1, 0, 0,
                              1, 0, 0,
                              1, 0, 0,
                              1, 1, 1 };
            break;
        case 'M': break;
        case 'N': 
            m_pixelMatrix = { 1, 1, 0,
                              1, 0, 1,
                              1, 0, 1,
                              1, 0, 1,
                              1, 0, 1 };
            break;
        case 'O': break;
        case 'P': break;
        case 'Q': break;
        case 'R': break;
        case 'S': 
            m_pixelMatrix = { 0, 1, 1,
                              1, 0, 0,
                              0, 1, 0,
                              0, 0, 1,
                              1, 1, 0 };
            break;
        case 'T': break;
        case 'U': break;
        case 'V': 
            m_pixelMatrix = { 1, 0, 1,
                              1, 0, 1,
                              1, 0, 1,
                              0, 1, 0,
                              0, 1, 0 };
            break;
        case 'W': break;
        case 'X': break;
        case 'Y': break;
        case 'Z': break;
        case '0': 
            m_pixelMatrix = { 1, 1, 1,
                              1, 0, 1,
                              1, 0, 1,
                              1, 0, 1,
                              1, 1, 1 };
            break;
        case '1': 
            m_pixelMatrix = { 0, 1, 0,
                              1, 1, 0,
                              0, 1, 0,
                              0, 1, 0,
                              1, 1, 1 };
            break;
        case '2': 
            m_pixelMatrix = { 1, 1, 0,
                              0, 0, 1,
                              0, 1, 0,
                              1, 0, 0,
                              1, 1, 1 };
            break;
        case '3': 
            m_pixelMatrix = { 1, 1, 0,
                              0, 0, 1,
                              0, 1, 0,
                              0, 0, 1,
                              1, 1, 0 };
            break;
        case '4': 
            m_pixelMatrix = { 1, 0, 0,
                              1, 0, 1,
                              1, 1, 1,
                              0, 0, 1,
                              0, 0, 1 };
            break;
        case '5': 
            m_pixelMatrix = { 1, 1, 1,
                              1, 0, 0,
                              1, 1, 0,
                              0, 0, 1,
                              1, 1, 0 };
            break;
        case '6': 
            m_pixelMatrix = { 0, 1, 1,
                              1, 0, 0,
                              1, 1, 1,
                              1, 0, 1,
                              1, 1, 1 };
            break;
        case '7': 
            m_pixelMatrix = { 1, 1, 1,
                              0, 0, 1,
                              0, 1, 0,
                              1, 0, 0,
                              1, 0, 0 };
            break;
        case '8': 
            m_pixelMatrix = { 1, 1, 1,
                              1, 0, 1,
                              1, 1, 1,
                              1, 0, 1,
                              1, 1, 1 };
            break;
        case '9': 
            m_pixelMatrix = { 1, 1, 1,
                              1, 0, 1,
                              1, 1, 1,
                              0, 0, 1,
                              1, 1, 0 };
            break;
        default:
            throw std::invalid_argument("ch");
    }
}

CharDisplay::CharDisplay(char ch, double side) 
    : CharDisplay{ch, side, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}} {}


void CharDisplay::changeColor(TripleDouble color) {
    m_color = color;
}

void CharDisplay::changeColor(double red, double green, double blue) {
    changeColor(std::make_tuple(red, green, blue));
}


void CharDisplay::move(TripleDouble pos) {
    m_pos = pos;
}

void CharDisplay::move(double x, double y, double z) {
    move(std::make_tuple(x, y, z));
}

void CharDisplay::draw() {
    auto [red, blue, green] = m_color;

    glColor3f(red, green, blue);
    for (std::size_t i = 0; i < m_pixelMatrix.getHeight(); ++i) {
        for (std::size_t j = 0; j < m_pixelMatrix.getWidth(); ++j) {
            if (m_pixelMatrix.view(i, j)) {
                glBegin(GL_QUADS);
                    glVertex3f(0.0, 0.0, 0.0);
                    glVertex3f(0.0 + m_side, 0.0, 0.0);
                    glVertex3f(0.0 + m_side, 0.0 + m_side, 0.0);
                    glVertex3f(0.0, 0.0 + m_side, 0.0);
                glEnd();
            }
        }
    }
}


void CharDisplay::generate() {
    auto [x, y, z] = m_pos;

    glPushMatrix();
    glTranslatef(x, y, z);   
    draw();
    glPopMatrix();
}
