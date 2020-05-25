#ifndef CHARDISPLAY_H
#define CHARDISPLAY_H

#include "GL/glut.h"

#include "matrix.h"


class CharDisplay {
private:
    typedef TripleDouble std::tuple<double, double, double>;

    mycontainers::matrix<bool, 3, 5> m_pixelMatrix;
    TripleDouble m_pos{0.0, 0.0, 0.0};
    TripleDouble m_color{0.0, 0.0, 0.0};

    double m_pixelSideLength;

public:
    CharDisplay() = delete;
    CharDisplay(char ch, double pixelSideLength);
    CharDisplay(char ch, TripleDouble pos, TripleDouble color, double pixelSideLength);

    void move(x, y, z);
    void draw();
    void generate();
};



#endif
