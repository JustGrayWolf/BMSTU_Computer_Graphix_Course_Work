#ifndef OBJECTS_H
#define OBJECTS_H
#include "INCLUDS.h"
#include "Primitiv.h"
#include "screen.h"
#include <list>
#include <ctime>

class Loader;

class Material
{
public:
    Material():born_temp(0), c(0), col(Color(0, 0, 0)){};
    Material(int born_temp, int c, Color col):born_temp(born_temp), c(c), col(col){};
    int get_born_temp(){return born_temp;};
    Color &get_color(){return col;};
    int get_born_koef(){return c;};
private:
    int born_temp;
    int c;
    Color col;
};

class Object
{
public:
    Object():position(Point3D(0,0,0)){};
    virtual ~Object(){};
    //virtual bool is_visible();
    //void shift(double dx, double dy, double dz);
    //void set_position(double x, double y, double z);
    //virtual void rotate(double axy, double ayz, double axz);
    //virtual void print();
    //int get_id();
    protected:
    //int id;
    Point3D position;
};

class Camera:Object
{
public:
    Camera();
    Point3D proect(Point3D &P);
    Point3D proect_(Point3D &P);
    void yaw(double a);
    void pitch(double a);
    void set(double ay, double ap);
    void setposition(double x, double y, double z);
    //bool is_visible();
private:
    double sinX, cosX, sinY, cosY, sinZ, cosZ;
    double ayaw, apitch;
};

class Model:Object
{
    friend class Loader;
    friend class Scene;
    friend class Screen;
    friend class MainWindow;
    friend class Flame;
public:
    //bool is_visible();
    void print();
    bool is_on_model(Point3D P);
    Point3D get_near_point_on_model(Point3D P);
    void set_material(Material mat){this->mat = mat;};
    Material get_material()
    {
        return mat;
    }
private:

    std::vector <List> edges;
    std::vector <Point3D> points;
    std::vector <Point3D> normals;
    Material mat;
};

class FirePoint:public Point3D
{
public:
    void Move()
    {
        double a = (double)temp/K0 - 1;
        vy = vy + (a * 0.1 - pow(vy, 2) * 0.1)*0.5;
        y -= vy;
        x += vx;
        z += vz;
        double dx = pow(vx, 2) * 0.1;
        double dz = pow(vz, 2) * 0.1;
        vx -= (vx > dx) ? dx : vx;
        vz -= (vz > dz) ? dz : vz;
    }
    double get_speed()
    {
        return vy;
    }
    void Tempdec()
    {
        temp -= dec;
    }

    void SetTemp(int T)
    {
        temp = T;
        dec = 100;
        double d = ((double)T/K0 - 1)*0.07;
        x += ((double)rand() / RAND_MAX) * FireDist - FireDist / 2;
        y += ((double)rand() / RAND_MAX) * FireDist - FireDist / 2;
        z += ((double)rand() / RAND_MAX) * FireDist - FireDist / 2;
        double ang = ((double)rand() / RAND_MAX) * 2 * M_PI;
        vx = d * sin(ang);
        vz = d * cos(ang);
        vy = d * 2;
    }

    void setdec(int d)
    {
        if (d + 100 < temp)
            dec = pow(temp - d, 2) * 0.008;
        else
            dec = 50;
    }

    int GetT()
    {
        return temp;
    }

private:
    int temp;
    double vx, vz, vy;
    int dec;
};

class GenPoint:public Point3D
{
public:
    FirePoint &generate();
    void SetTemp(int T)
    {
        temp = T;
        start_time = clock();
        spread = false;
    }
    int getTemp()
    {
        return temp;
    }
    bool is_spread()
    {
        return spread;
    }
    void set_spread(){spread = true;};

    void setMaterial(Material &m)
    {
        material = m;
    }

    void up_temp()
    {
        if (material.get_born_koef() > temp)
        {
            if ((clock() - start_time)/CLOCKS_PER_SEC * 50 > 1)
            {
                temp += (double)((double)material.get_born_koef()* ((double)rand()/RAND_MAX) / 20);// ;
                start_time = clock();
            }
        }

    }

private:
    int temp;
    Material material;
    bool spread;
    int start_time;
};

class Flame
{
    friend class Scene;
public:
    void stop(){generate.clear(); fire.clear();};
    void check_genpoints(Model & m);
    void add_generate_point(Point3D P, Material m);
    void refrash_data();
    void generatefire();
private:
    void check_and_add_point(Model &m, double x, double y, double z, GenPoint &F);
std::list <GenPoint> generate;
std::list <FirePoint> fire;
};

class Scene
{
    friend void actionThread(Scene &scene);
    friend class MainWindow;
public:
    Scene()
    {
        Cam.set(0, 0);
        Lights.resize(1);
        Lights[0].set(0, 0, -4);
        Lights[0].SetI(1, 1, 1);
    };
    void proect(std::vector <Point3D> &points);
    void proect_light(std::vector <Light> &L);
    void proect_flame(std::vector <Point3D> &p);
    void add(Model &m);
private:
    Camera Cam;
    std::vector <Light> Lights;
    std::vector <Model> model;
    Flame flame;
};


#endif // OBJECTS_H
