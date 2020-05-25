#ifndef CHARDISPLAY_H
#define CHARDISPLAY_H

#include <tuple>
#include <stdexcept>

#include "GL/glut.h"

#include "matrix.h"


class CharDisplay {
public:
    typedef std::tuple<double, double, double> TripleDouble;

private:

    mycontainers::Matrix<bool, 3, 5> m_pixelMatrix;
    double m_side;

    TripleDouble m_pos{0.0, 0.0, 0.0};
    TripleDouble m_color{0.0, 0.0, 0.0};

public:
    CharDisplay() = delete;
    CharDisplay(char ch, double side, TripleDouble pos, TripleDouble color);
    CharDisplay(char ch, double side);

    void changeColor(TripleDouble color);
    void changeColor(double red, double green, double blue);

    void move(TripleDouble pos);
    void move(double x, double y, double z);

    void draw();
    void generate();
};



#endif
