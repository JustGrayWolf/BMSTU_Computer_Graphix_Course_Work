#include <cmath>
#include "algoritms.h"

int intersection(int x1,int y1,int x2,int y2, int y)
{
    //std::cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << ' ' << y << '\n';
    if (y < ((y1<y2)?y1:y2) or y > ((y1<y2)?y2:y1) or y1 == y2)
        return -1;
    double k = ((double)x1 - x2) / (y1 - y2);
    //std::cout << k << ' ' << (x1 - x2) << ' ' << (y1 - y2) <<" lol\n";
    int res = round(k * y + x2 - k * y2);
    if (res < 0)
        res = 0;
    else if (res > WINWIDTH)
        res = WINWIDTH;
    return res;
}

void fill(std::vector <std::vector <double>> &Zbuf, std::vector <std::vector <Color>> &Cbuf)
{
    for (int i = 0; i < Zbuf.size(); i++)
        for (int j = 0; j < Zbuf[i].size(); j++)
        {
            Zbuf[i][j] = MinZ;
            Cbuf[i][j].set_color(BGRed, BGGreen, BGBlue);
            //std::cout << Cbuf[i][j].getQColor().green() << ' ';
            //Cbuf[i][j].print();
        }
};

void MinMaxX(std::vector <int> &v, int &min, int &max)
{
    min = v[0];
    max = min;
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] < min)
            min = v[i];
        else if (v[i] > max)
            max = v[i];
    }
    if (min < 0)
        min = 0;
    if (max > WINWIDTH)
        max = WINWIDTH;
}

double calculateDeep(Point3D P1, Point3D P2, int x, int y)
{
    double d1 = P1.distXY(x, y), d2 = P2.distXY(x, y);
    return (d1 * P1.getZ() + d2 * P2.getZ())/(d1 + d2);
}

Point3D Normal(List &edge, std::vector <Point3D> &P, int a)
{
    Point3D P1, P2, P3;
    //double x, y, z;
    edge.to_begin();
    int n = edge.get_npoint() + a;
    P1.set( (P[n].getX() - WINWIDTHD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, (P[n].getY() - WINHEIGHTD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, P[n].getZ());
    edge.next();
    n = edge.get_npoint() + a;
    P2.set( (P[n].getX() - WINWIDTHD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, (P[n].getY() - WINHEIGHTD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, P[n].getZ());
    edge.next();
    n = edge.get_npoint() + a;
    P3.set( (P[n].getX() - WINWIDTHD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, (P[n].getY() - WINHEIGHTD2) * ((P[n].getZ() + 1 > EPS) ? (P[n].getZ() + 1) : 0.001) / MKOEF, P[n].getZ());


    double x1 = P1.getX(), x2 = P2.getX(), x3 = P3.getX();
    double y1 = P1.getY(), y2 = P2.getY(), y3 = P3.getY();
    double z1 = P1.getZ(), z2 = P2.getZ(), z3 = P3.getZ();

    double ka = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
    double kb = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
    double kc = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);

    return Point3D(ka, kb, kc);
}

Point3D CalcKoef(List &edge, std::vector <Point3D> &P, int a)
{
    Point3D P1, P2, P3;
    //double x, y, z;
    edge.to_begin();
    P1 = P[edge.get_npoint() + a];
    edge.next();
    P2 = P[edge.get_npoint() + a];
    edge.next();
    P3 = P[edge.get_npoint() + a];
    double x1 = P1.getX(), x2 = P2.getX(), x3 = P3.getX();
    double y1 = P1.getY(), y2 = P2.getY(), y3 = P3.getY();
    double z1 = P1.getZ(), z2 = P2.getZ(), z3 = P3.getZ();

    double ka = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
    double kb = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
    double kc = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);
    double kd = x1*(y2*z3 - y3*z2) + x2*(y3*z1 - y1*z3) + x3*(y1*z2 - y2*z1);
/*
    std::cout << x1 << ' ' << y1 << ' ' << z1 << '\n';
    std::cout << x2 << ' ' << y2 << ' ' << z2 << '\n';
    std::cout << x3 << ' ' << y3 << ' ' << z3 << '\n';
    std::cout << ka << ' ' << kb << ' ' << kc << ' ' << -kd << '\n';
    */
    if (abs(kc) < EPS)
    {
        return Point3D(0, 0, 0);
    }
    else
    {
        //std::cout << - x1*ka/kc - y1*kb/kc +kd/kc << ' ' << z1 << '\n';
        return Point3D(-ka/kc, -kb/kc, +kd/kc);
    }
    /*
    if (P1.getY() == P2.getY())
    {
        if (P2.getY() == P3.getY())
            return Point3D(0, 0, 0);
        Point3D P = P2;
        P2 = P3;
        P3 = P;
    }

    double Z01 = P1.getZ() - P2.getZ();
    double Z02 = P3.getZ() - P2.getZ();
    double k = (P3.getY() - P2.getY())/(P1.getY() - P2.getY());
    double Z03 = Z02 - Z01 * k;

    x = Z03/((P3.getX() - P2.getX()) - (P1.getX() - P2.getX() * k));
    y = (Z01 - (P1.getX() - P2.getX()) * x)/(P1.getY() - P2.getY());
    z = P1.getZ() - P1.getX() * x - P2.getY() * y;
*/


}

void MinMaxY(List &edge, std::vector <Point3D> &P, int &min, int &max, int a = 0)
{
    edge.to_end();
    int cur;
    min = round(P[edge.get_npoint() + a].getY());
    max = min;
    for (edge.to_begin();!edge.is_end();edge.next())
    {
        cur = round(P[edge.get_npoint() + a].getY());
        if (cur < min)
            min = cur;
        else if (cur > max)
            max = cur;
    }

    if (min < 0)
        min = 0;
    if (max > WINHEIGHT)
        max = WINHEIGHT;


}

void Screen::zbuffer(std::vector <Model> &m)
{
    std::vector <List> edges = m[0].edges;
    int addnum = -1;
    int ystart, yend;
    int xfirst, xlast;
    int buf;
    int intersect;
    std::vector <int> intersections;
    Point3D K, N;
    double CalcZ;
    fill(Zbuf, Cbuf);
    for(int l = 0; l < edges.size(); l++)
    {
        //if (l == 5)
            //continue;
    K = CalcKoef(edges[l], P, addnum);
    N = Normal(edges[l], P, addnum);
    MinMaxY(edges[l], P, ystart, yend, addnum);
    //std::cout << 'y' << ystart << ' ' << yend << '\n';
    for (int y = ystart; y <= yend; y++)
    {
        edges[l].to_end();
        buf = edges[l].get_npoint() + addnum;
        for (edges[l].to_begin();!edges[l].is_end(); edges[l].next())
        {
            intersect = intersection(P[buf].getX(),P[buf].getY(),P[edges[l].get_npoint()+addnum].getX(),P[edges[l].get_npoint()+addnum].getY(), y);
            buf = edges[l].get_npoint() + addnum;
            if (intersect != -1)
            {
                intersections.push_back(intersect);
                //std::cout << intersect << ' ';
            }

        }
        intersect = intersection(P[buf].getX(),P[buf].getY(),P[edges[l].get_npoint()+addnum].getX(),P[edges[l].get_npoint()+addnum].getY(), y);
        if (intersect != -1)
        {
            intersections.push_back(intersect);
            //std::cout << intersect << '\n';
        }

        if (intersections.size() < 2)
            continue;

        MinMaxX(intersections, xfirst, xlast);
        intersections.clear();
        //std::cout << 'x' << xfirst << ' ' << xlast << '\n';
        for (int x = xfirst; x <= xlast; x++)
        {
            CalcZ = x*K.getX() + y*K.getY() + K.getZ();
            if (Zbuf[x][y] > CalcZ and CalcZ > 0)
            {
                Zbuf[x][y] = CalcZ;
               // Cbuf[x][y].set_color(255, 255 - (l*10 % 256), 255);
                Point3D P0((x - WINWIDTHD2) * ((CalcZ + 1 > EPS) ? (CalcZ + 1) : 0.001) / MKOEF, (y - WINHEIGHTD2) * ((CalcZ + 1 > EPS) ? (CalcZ + 1) : 0.001) / MKOEF, CalcZ);
                Cbuf[x][y] = pixelcolor(m[0].mat.get_color(), P0, N, Lights);
            }

        }
    }
    }
}

double FongVecCos(Point3D &P1, Point3D &N, Point3D &P2)
{
    double v1x = P1.getX() - P2.getX();
    double v1y = P1.getY() - P2.getY();
    double v1z = P1.getZ() - P2.getZ();
    //if (!v1x and !v1y and !v1z)
        //return 1;
    double v2x = N.getX();
    double v2y = N.getY();
    double v2z = N.getZ();

    double k = v2x * v1x + v2y * v1y + v2z * v1z;
    double v3x = v1x - v2x * k * 2.0f;
    double v3y = v1y - v2y * k * 2.0f;
    double v3z = v1z - v2z * k * 2.0f;

    double v4x = -P1.getX();
    double v4y = -P1.getY();
    double v4z = -P1.getZ();
    double sqrd = (v3x * v3x + v3y * v3y + v3z * v3z) * (v4x * v4x + v4y * v4y + v4z * v4z);
    if (sqrd > EPS)
        return (v3x * v4x + v3y * v4y + v3z * v4z)/sqrt(sqrd);
    else
        return 1;
}

double VecCos(Point3D &P1, Point3D &N, Point3D &P2)
{
   // P1.print();
    //P2.print();
double v1x = P2.getX() - P1.getX();
double v1y = P2.getY() - P1.getY();
double v1z = P2.getZ() - P1.getZ();
//if (!v1x and !v1y and !v1z)
    //return 1;
double v2x = N.getX();
double v2y = N.getY();
double v2z = N.getZ();
//std::cout << v1x << ' ' << v1y << ' ' << v1z << '\n';
double sqrd = (v1x * v1x + v1y * v1y + v1z * v1z) * (v2x * v2x + v2y * v2y + v2z * v2z);
if (sqrd > EPS)
    return (v1x * v2x + v1y * v2y + v1z * v2z)/sqrt(sqrd);
else
    return 1;
}

Color pixelcolor(Color &PixCol, Point3D &P, Point3D &N, std::vector <Light> &Light)
{

    //Light[0].print();
    //std::cout<<'\n';
    //P.print();
    //std::cout<<'\n';
    int r = PixCol.GetR(), g = PixCol.GetG(), b = PixCol.GetB();
    double k1 = 0.2, k2 = 5, k3 = 100;
    int rr = k1 * r, rg = k1 * g, rb = k1 * b;
    int Pn = 2;
    for (int i = 0; i < Light.size(); i++)
    {

    Point3D LP = Light[i].getPoint();
    if (LP.getZ() > P.getZ()) continue;
    double CosPn = pow(FongVecCos(P, N, LP), Pn), CosLI = abs(VecCos(P, N, LP));
    //if (CosLI <= 0) continue;
    //std::cout << CosLI << ' ' << CosPn << ' ' <<  LP.getZ() << '\n';
    rr += (k2 * CosLI * (double)r + k3 * CosPn ) * Light[i].GetIr() / (P.sqrdistP(LP) + 1);
    rg += (k2 * CosLI * (double)g + k3 * CosPn ) * Light[i].GetIg() / (P.sqrdistP(LP) + 1);
    rb += (k2 * CosLI * (double)b + k3 * CosPn ) * Light[i].GetIb() / (P.sqrdistP(LP) + 1);
    }
    rr = (rr > 255) ? 255 : rr;
    rg = (rg > 255) ? 255 : rg;
    rb = (rb > 255) ? 255 : rb;
    return Color(rr, rg, rb);
};
