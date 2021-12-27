#ifndef PRIMITIV_H
#define PRIMITIV_H
#include <math.h>

    class Point3D
    {
    public:
        Point3D():x(0),y(0),z(0){};
        Point3D(double x, double y, double z);
        Point3D getPoint(){return *this;};
        void set(double x, double y, double z){this->x = x;this->y = y; this->z = z;};
        void set(Point3D P){x=P.x;y=P.y;z=P.z;};
        void print();
        double distXY(double x0, double y0){return sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));};
        double sqrdistP (Point3D &P){return (pow(P.getX() - x, 2) + pow(P.getY() - y, 2) + pow(P.getZ() - z, 2));};
        double getZ(){return z;};
        double getY(){return y;};
        double getX(){return x;};
        void Shift(double dx, double dy, double dz){x+=dx;y+=dy;z+=dz;};

    protected:
        double x, y, z;
    };


    class Light:public Point3D
    {
    public:
        int GetIr(){return Ir;};
        int GetIg(){return Ig;};
        int GetIb(){return Ib;};
        void SetI(int r, int g, int b){Ir = r; Ig = g; Ib = b;};
    private:
        int Ir, Ig, Ib;
    };

    struct elem
    {
        int npoint;
        int nnorm;
        int ntext;
        elem *nextel;
    };


    class List
    {
    public:
        List():head(nullptr), tell(nullptr), cur(nullptr){};
        int get_npoint();
        int get_nnorm();
        int get_ntext();
        void add(int, int, int);
        bool is_end();
        bool is_begin();
        void to_end();
        void to_begin();
        const List operator++();
        void next();
    private:
        elem *head;
        elem *cur;
        elem *tell;
    };

#endif // PRIMITIV_H
