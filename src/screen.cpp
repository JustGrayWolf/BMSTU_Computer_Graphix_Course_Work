#include "screen.h"
#include "Primitiv.h"
#include "loader.h"
#include <math.h>

Screen::Screen()
{
    Cbuf.resize(WINWIDTH + 1);
    for(int i = 0; i < WINWIDTH + 1; i++)
        Cbuf[i].resize(WINHEIGHT + 1);
    Zbuf.resize(WINWIDTH + 1);
    for(int i = 0; i < WINWIDTH + 1; i++)
        Zbuf[i].resize(WINHEIGHT + 1);
    std::string s = "./fire0.bmp";
    flame_mask = Loader().loadpng(s);
};


Color& Color::operator+=(Color &c)
{
    if (c.a == 0)
    {
        this->b += c.GetB();
        this->g += c.GetG();
        this->r += c.GetR();
    }
    else
    {
        //std::cout << 'c' << this->GetR() << this->GetG() << this->GetB() << '\n';
        this->b += round(((double)(c.GetB() - this->GetB()) * c.GetA()) /256);
        this->g += round(((double)(c.GetG() - this->GetG()) * c.GetA()) /256);
        this->r += round(((double)(c.GetR() - this->GetR()) * c.GetA()) /256);
        //std::cout << 'c' << this->GetR() << this->GetG() << this->GetB() << '\n';
    }
    return *this;
}

void Screen::flame_visualise()
{
    double x, y, z;
    double e;
    for (Point3D P : Flame)
    {
        x = P.getX();
        y = P.getY();
        z = P.getZ();
        double K = z*FKOEF;
        int maxx = round(flame_mask.size()/K);
        int maxy;
        if (maxx > 0)
            maxy = round(flame_mask[0].size()/K);
        int maxxd2 = maxx / 2;
        if (x > -maxx and x < WINWIDTH + maxx and y > -maxy and y < WINHEIGHT + maxy and abs(z) > 0.5)
            for(int i = 0; i < maxx and (x+i-maxxd2)>0 and (x+i-maxxd2)<WINWIDTH; i++)
            {
               int maxyd2 = maxy / 2;
               for(int j = 0; j < maxy and (y+j-maxyd2)>0 and (y+j-maxyd2)<WINWIDTH; j++)
               {
                   if (Zbuf[x+i-maxxd2][y+j-maxyd2] >= z)
                        Cbuf[x+i-maxxd2][y+j-maxyd2] += flame_mask[i*K][j*K];
               }
            }

    }
}


