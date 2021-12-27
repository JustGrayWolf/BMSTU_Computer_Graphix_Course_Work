#ifndef SCREEN_H
#define SCREEN_H
#include "INCLUDS.h"
#include "Primitiv.h"
//#include "Objects.h"
#include <QMainWindow>

class Color
{
public:
    Color():r(0), g(0), b(0)
    {};
    Color(char r, char g, char b):r(r), g(g), b(b){};

    Color(char a, char r, char g, char b):a(a), r(r), g(g), b(b){};

    void set_color(char a, char r, char g, char b){this->a = a;this->r = r;this->g = g;this->b = b;};

    void set_color(char r, char g, char b){this->r = r;this->g = g;this->b = b;};

    QColor getQColor()
    {
        int qr = ((int)r < 0) ? 256 + (int)r : (int)r, qb = ((int)b < 0) ? 256 + (int)b : (int)b, qg = ((int)g < 0) ? 256 + (int)g : (int)g;

        return QColor(qr, qg, qb);
    };

    int GetR(){return ((int)r < 0) ? 256 + (int)r : (int)r;};
    int GetG(){return ((int)g < 0) ? 256 + (int)g : (int)g;};
    int GetB(){return ((int)b < 0) ? 256 + (int)b : (int)b;};
    int GetA(){return ((int)a < 0) ? 256 + (int)a : (int)a;};

    Color &operator+= (Color &c);

    void print()
    {
        std::cout << (int)r << ' ' << (int)g << ' ' << (int)b << ' ';
    }

private:
    char a;
    char r;
    char g;
    char b;
};

class Model;

class Screen
{
    friend class MainWindow;
public:
    Screen();
    void zbuffer(std::vector <Model>  &m);
    void flame_visualise();
    QColor GetColor(int x, int y){return Cbuf[x][y].getQColor();};
    //void draw();
private:
std::vector <Point3D> P;
std::vector <Point3D> Flame;
std::vector <Light> Lights;
std::vector <std::vector <double>> Zbuf;
std::vector <std::vector <Color>> flame_mask;
std::vector <std::vector <Color>> Cbuf;
};

#endif // SCREEN_H
