#include "comands.h"
#include "loader.h"
#include "mutexs.h"
std::mutex comand_mtx;
SetCamera::SetCamera(Camera &cam, std::string &x, std::string &y, std::string &z):cam(cam)
{
    error = false;
    try{
    this->x = stod(x);
    this->y = stod(y);
    this->z = stod(z);
    }
    catch(std::invalid_argument)
    {
        error = true;
        std::cout << "Input error";
    };
};

void SetCamera::run()
{
    comand_mtx.lock();
    if (!error)
        cam.setposition(x, y, z);
    comand_mtx.unlock();
};

SetLight::SetLight(std::vector <Light> &light, std::string &x, std::string &y, std::string &z, std::string &Ir, std::string &Ig, std::string &Ib):light(light)
{
    error = false;
try{
    this->x = stod(x);
    this->y = stod(y);
    this->z = stod(z);
    this->Ir = stoi(Ir);
    this->Ig = stoi(Ig);
    this->Ib = stoi(Ib);
}
catch(std::invalid_argument)
{
    error = true;
    std::cout << "Input error";
};
};

void SetLight::run()
{
    comand_mtx.lock();
    if (!error){
        if (light.size() == 0)
            light.resize(1);
        light[0].set(x, y, z);
        light[0].SetI(Ir, Ig, Ib);
    comand_mtx.unlock();
    }
};

SetCameraAngle::SetCameraAngle(Camera &cam, std::string &pitch, std::string &yaw):cam(cam)
{
    error = false;
try{
    this->angpitch = stod(pitch)*M_PI/180;
    this->angyaw = stod(yaw)*M_PI/180;
}
catch(std::invalid_argument)
{
    std::cout << "Input error";
    error = true;
};
};

void SetCameraAngle::run()
{
    comand_mtx.lock();
    if (!error)
        cam.set(angyaw, angpitch);
    comand_mtx.unlock();
};

AddFire::AddFire(Model &m, Flame &f, std::string &x, std::string &y, std::string &z):f(f), m(m)
{
    error = false;
try{
    this->x = stod(x);
    this->y = stod(y);
    this->z = stod(z);
}
catch(std::invalid_argument)
{
    error = true;
    std::cout << "Input error";
};
};

void AddFire::run()
{
    comand_mtx.lock();
    if (!error and m.is_on_model(Point3D(x, y, z)))
        f.add_generate_point(Point3D(x, y, z), m.get_material());
    comand_mtx.unlock();
}

void LoadModel::run()
{
    comand_mtx.lock();
    f.stop();
    if (m.size() == 0)
    {
        m.resize(1);
        std::string fname = ModelName;
        m[0] = Loader().loadmodel(fname);
    }
    Material mat;
    switch(i)
    {
    case 0:{mat = Material(400, 1000, Color(139, 69, 19));break;};
    case 1:{mat = Material(350, 1500, Color(28, 28, 28));break;};
    case 2:{mat = Material(370, 2000, Color(54, 54, 54));break;};
    case 3:{mat = Material(400, 1200, Color(85, 107, 47));break;};
    }
    m[0].set_material(mat);
    comand_mtx.unlock();
}

/*
comands::comands()
{

}
*/
