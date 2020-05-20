#include <iostream>
#include <vector>
#include <GL/glut.h>

#include "SOIL.h"

class Cube
{
    private:
        std::vector<double> cube;
        double texture_position[12] = {
                                0.0, 0.0,
                                1.0, 0.0, 
                                1.0, 1.0,
                                0.0, 1.0,
                            };
        // double texture_position;

    public:
    double _x, _y, _z, _len; // apply len later
    GLint texture_name;

    Cube()
    {
        _x = 0;
        _y = 0;
        _z = 0;

        cube = {       
            // front
            0.0, 0.0, 1.0,
            0.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 0.0, 1.0,

            // back
            0.0, 0.0, 0,
            0.0, 1.0, 0,
            1.0, 1.0, 0,
            1.0, 0.0, 0,

            // 1.0 left
            0.0, 1.0, 1.0,
            0.0, 1.0, 0,
            0.0, 0.0, 0,
            0.0, 0.0, 1.0,

            // 1.0 right
            1.0, 1.0, 1.0,
            1.0, 1.0, 0,
            1.0, 0.0, 0,
            1.0, 0.0, 1.0,

            // bottom
            0.0, 0.0, 1.0,
            0.0, 0.0, 0,
            1.0, 0.0, 0,
            1.0, 0.0, 1.0,
            
            // top
            0.0, 1.0, 1.0,
            0.0, 1.0, 0,
            1.0, 1.0, 0,
            1.0, 1.0, 1.0,
        };
    }

    Cube(GLint texture, double x, double y, double z, double side)
    {
        texture_name = texture;
        _x = x;
        _y = y;
        _z = z;
        // _len = length;
        cube = {       
            // front
            0.0, 0.0, side,
            0.0, side, side,
            side, side, side,
            side, 0.0, side,

            // back
            0.0, 0.0, 0,
            0.0, side, 0,
            side, side, 0,
            side, 0.0, 0,

            // side left
            0.0, side, side,
            0.0, side, 0,
            0.0, 0.0, 0,
            0.0, 0.0, side,

            // side right
            side, side, side,
            side, side, 0,
            side, 0.0, 0,
            side, 0.0, side,

            // bottom
            0.0, 0.0, side,
            0.0, 0.0, 0,
            side, 0.0, 0,
            side, 0.0, side,
            
            // top
            0.0, side, side,
            0.0, side, 0,
            side, side, 0,
            side, side, side,
        };
    }

    void set_all(GLint texture, double x, double y, double z)
    {
        texture_name = texture;
        _x = x;
        _y = y;
        _z = z;
        // _len = length;
    }

    void move_piece(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }

    void resize(double length)
    {
        _len = length;
    }


    void enable_texture()
    {
        glBindTexture(GL_TEXTURE_2D, texture_name);
        glEnable(GL_TEXTURE_2D);
    }


    void generate()
    {
        glPushMatrix();
        glTranslatef(_x , _y, _z);
        enable_texture();
        for(int i = 0; i < 72; i += 12)
        {
            int check = 0;
            glBegin(GL_QUADS);
                for(int j = i; j < i + 12; j += 3)
                {
                    glTexCoord2f(texture_position[check], texture_position[check+1]); glVertex3f(cube[j], cube[j + 1], cube[j + 2]);
                    check += 2;
                }
            glEnd();
        }
        glPopMatrix();
    }
    void info()
    {
        std::cout <<"coords: x: "<< _x << " y: " << _y << " z: " << _z << " texture: " << texture_name << "\n";
    }
};