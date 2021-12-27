#ifndef COMANDS_H
#define COMANDS_H
#include "INCLUDS.h"
#include "Objects.h"


class Comand
{
    virtual void run();
};

class SetCameraAngle: Comand
{
public:
    SetCameraAngle( Camera &cam, std::string &pitch, std::string &yaw);
    void run();
private:
    double angpitch, angyaw;
    Camera &cam;
    bool error;
};

class SetCamera: Comand
{
public:
    SetCamera( Camera &cam, std::string &x, std::string &y, std::string &z);
    void run();
private:
    double x, y, z;
    Camera &cam;
    bool error;
};

class LoadModel: Comand
{
public:
    LoadModel(Flame& f, std::vector <Model> &m, int i):i(i), m(m), f(f){};
    void run();
private:
    int i;
    std::vector <Model> &m;
    Flame &f;
};

class SetLight: Comand
{
public:
    SetLight(std::vector <Light> &light, std::string &x, std::string &y, std::string &z, std::string &Ir, std::string &Ig, std::string &Ib);
    void run();
private:
    double x, y, z;
    int Ir, Ig, Ib;
    std::vector <Light> &light;
    bool error;
};

class AddFire: Comand
{
public:
    AddFire(Model &m, Flame &f, std::string &x, std::string &y, std::string &z);
    void run();
private:
    double x, y, z;
    Flame &f;
    Model &m;
    bool error;
};

#endif // COMANDS_H
