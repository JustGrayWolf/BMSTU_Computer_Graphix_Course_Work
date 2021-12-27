#include "INCLUDS.h"
#include "Objects.h"
#include <list>
//bool Model::is_visible()
//{return true;};

//bool Camera::is_visible()
//{return false;};

Camera::Camera()
{
    position.Shift(0, 0, -4);
}

void Camera::setposition(double x, double y, double z)
{
    position.set(x, y, z);
};

void Camera::set(double ay, double ap)
{
    ayaw = ay;
    cosY = cos(ayaw);
    sinY = sin(ayaw);

    apitch = ap;
    sinX = sin(apitch);
    cosX = cos(apitch);
    sinZ = 0;
    cosZ = 1;
}

void Camera::yaw(double a)
{
    ayaw += a;
    cosY = cos(ayaw);
    sinY = sin(ayaw);
    pitch(0);
}

void Camera::pitch(double a)
{
    apitch += a;
    sinX = sin(apitch);
    cosX = cos(apitch);
    sinZ = 0;
    cosZ = 1;
}

Point3D Camera::proect(Point3D &P)
{
    double sx = P.getX() - position.getX();
    double sy = P.getY() - position.getY();
    double sz = P.getZ() - position.getZ();
    double x = sx * cosY + sz * sinY;
    double y = sy;
    double z = sz * cosY - sx * sinY;
    sx = x;
    sy = y * cosX + z * sinX;
    sz = z * cosX - y * sinX;
    x = sx * cosZ - sy * sinZ;
    y = sy * cosZ - sx * sinZ;
    if (sz + 1 > EPS)
    {
        x = x * MKOEF / (sz + 1)  + WINWIDTHD2;
        y = y * MKOEF / (sz + 1) + WINHEIGHTD2;
    }
    else
    {
        x = x * MKOEF * 1000 + WINWIDTHD2;
        y = y * MKOEF * 1000 + WINHEIGHTD2;
    }
    //std::cout << x << ' ' << y << ' ';
    return Point3D(x , y , sz);
}

Point3D::Point3D(double x, double y, double z){this->x=x;this->y=y;this->z=z;};

void Model::print()
{
    std::cout << "\nModel:\n    Points(" << points.size() << "):\n";
    for (int i = 0; i < points.size(); i++)
    {
        std::cout << "        ";
        points.at(i).print();
        std::cout << '\n';
    }

    std::cout << "\n\n    Normals(" << normals.size() << "):\n";
    for (int i = 0; i < normals.size(); i++)
    {
        std::cout << "        ";
        normals.at(i).print();
        std::cout << '\n';
    }

    std::cout << "    Edges(" << edges.size() << "):\n";

    for (int i = 0; i < edges.size(); i++)
    {
        std::cout << "        ";

        for (edges.at(i).to_begin(); !edges.at(i).is_end();edges.at(i).next())
        {
            std::cout << "np = " << edges.at(i).get_npoint() << " nn = " << edges.at(i).get_nnorm() << "; ";
        }
        std::cout << "np = " << edges.at(i).get_npoint() << " nn = " << edges.at(i).get_nnorm() << "; ";
        std::cout << '\n';
    }

}

void Scene::proect(std::vector <Point3D> &points)
{
    int size = 0, l = 0;
    for (int i = 0; i < model.size(); i++)
        size += model[i].points.size();
    if (points.size() != size)
        points.resize(size);
    // std::cout << size;
    for (int i = 0; i < model.size(); i++)
        for (int j = 0; j < model[i].points.size(); j++)
        {
            points[l] = Cam.proect(model[i].points[j]);
            //points[l].print();
            //std::cout << '\n';
            l++;
        }

}

void Scene::proect_light(std::vector <Light> &L)
{
    if (L.size() != Lights.size())
        L.resize(Lights.size());
    Point3D P;
    // std::cout << size;
    for (int i = 0; i < L.size(); i++)
    {
            P = Lights[i].getPoint();
            L[i] = Lights[i];
            L[i].set(Cam.proect_(P));
            //Cam.proect(P).print();
            //L[i].print();
            //std::cout << '\n';
    }

}

void Scene::add(Model &m)
{
    model.resize(model.size() + 1);
    model[model.size() - 1] = m;
}

void Scene::proect_flame(std::vector <Point3D> &p)
{
    if (p.size() != flame.fire.size())
        p.resize(flame.fire.size());
    // std::cout << size;
    int l = 0;
        for (FirePoint F: flame.fire)
        {
            p[l] = Cam.proect(F);
            l++;
            //points[l].print();
            //std::cout << '\n';
        }
};

void Flame::add_generate_point(Point3D P, Material m)
{
    GenPoint *G = new GenPoint;
    G->set(P);
    G->setMaterial(m);
    G->SetTemp(m.get_born_temp());
    generate.push_front(*G);
};

void Flame::refrash_data()
{
    if (fire.size() == 0)
        return;
    int envtemp = MinTemp;
    long long sr = 0;
    int ch = 0;
    double dist = 0;
    std::list <FirePoint> :: iterator it = fire.begin();
    for (;it!=fire.end();it++)
    {
        sr = 0;
        ch = 0;
        for (FirePoint &F: fire)
        {
            dist = F.sqrdistP(*it);
            if (dist > EPS and dist < 0.01)
            {
                sr += F.GetT();
                ch++;
                if (ch == 10)
                    break;
            }
        }
        for (int i = ch; i < 10; i++)
        {
            sr += MinTemp;
            ch++;
        }
        it->setdec(sr/ch);
        it->Tempdec();
        if (it->GetT() < MinTemp or it->get_speed() < EPS)
        {
            fire.erase(it);

            if (!fire.size() or it == fire.end())
                break;
        }
        else
            it->Move();
    }
    for (int i = 0; i < (5 - (fire.size()/generate.size())) ; i++)
    {
        generatefire();
    }
    //iterator iter = fire.begin();
};

void Flame::generatefire()
{
    for (GenPoint P : generate)
    {
        fire.push_front(P.generate());
    }
};

FirePoint & GenPoint::generate()
{
    FirePoint *res = new FirePoint;
    res->set(x, y, z);
    res->SetTemp(temp * 0.8 + (double)rand()/RAND_MAX * temp * 0.2);
    return *res;
}

Point3D Camera::proect_(Point3D &P)
{
    double sx = P.getX() - position.getX();
    double sy = P.getY() - position.getY();
    double sz = P.getZ() - position.getZ();
    double x = sx * cosY + sz * sinY;
    double y = sy;
    double z = sz * cosY - sx * sinY;
    sx = x;
    sy = y * cosX + z * sinX;
    sz = z * cosX - y * sinX;
    x = sx * cosZ - sy * sinZ;
    y = sy * cosZ - sx * sinZ;
    //std::cout << x << ' ' << y << ' ';
    return Point3D(x, y, sz);
};

void Flame::check_and_add_point(Model &m, double x, double y, double z, GenPoint &F)
{
    Point3D P = F;
    bool b = false;
    P.Shift(x, y, z);
    Point3D P0 = m.get_near_point_on_model(P);

    if (sqrt(P0.sqrdistP(F)) > (FireDist / 3))
    {
        for (GenPoint G : generate)
        {
            if (sqrt(P0.sqrdistP(G)) < FireDist / 3)
                {
                    b = true;
                    break;
                }
        }
    }
    else
        b = true;
    if (!b)
    {
        add_generate_point(P0, m.mat);
    }
}

void Flame::check_genpoints(Model & m)
{
    Point3D P, K, P0;
    generatefire();
    for(GenPoint &F : generate)
    {
        F.up_temp();
        if (F.getTemp() > m.mat.get_born_temp() * 1.5)
            if (!F.is_spread())
            {
                check_and_add_point(m, -FireDist, 0, 0, F);
                check_and_add_point(m, FireDist, 0, 0, F);
                check_and_add_point(m, 0, -FireDist, 0, F);
                check_and_add_point(m, 0, FireDist, 0, F);
                check_and_add_point(m, 0, 0, -FireDist, F);
                check_and_add_point(m, 0, 0, FireDist, F);
                F.set_spread();
            }

    }
};

void MinMax_Y(List &edge, std::vector <Point3D> &P, double &min, double &max)
{
    edge.to_end();
    int cur;
    min = round(P[edge.get_npoint() - 1].getY());
    max = min;
    for (edge.to_begin();!edge.is_end();edge.next())
    {
        cur = round(P[edge.get_npoint() - 1].getY());
        if (cur < min)
            min = cur;
        else if (cur > max)
            max = cur;
    }
}

void MinMax_X(List &edge, std::vector <Point3D> &P, double &min, double &max)
{
    edge.to_end();
    int cur;
    min = round(P[edge.get_npoint() - 1].getX());
    max = min;
    for (edge.to_begin();!edge.is_end();edge.next())
    {
        cur = round(P[edge.get_npoint() - 1].getX());
        if (cur < min)
            min = cur;
        else if (cur > max)
            max = cur;
    }
}

void MinMax_Z(List &edge, std::vector <Point3D> &P, double &min, double &max)
{
    edge.to_end();
    int cur;
    min = round(P[edge.get_npoint() - 1].getZ());
    max = min;
    for (edge.to_begin();!edge.is_end();edge.next())
    {
        cur = round(P[edge.get_npoint() - 1].getZ());
        if (cur < min)
            min = cur;
        else if (cur > max)
            max = cur;
    }
}

void CoefCalc(List &edge, std::vector <Point3D> &P, double &ka, double &kb, double &kc, double &kd)
{
    Point3D P1, P2, P3;
    //double x, y, z;
    edge.to_begin();
    P1 = P[edge.get_npoint() - 1];
    edge.next();
    P2 = P[edge.get_npoint() - 1];
    edge.next();
    P3 = P[edge.get_npoint() - 1];
    double x1 = P1.getX(), x2 = P2.getX(), x3 = P3.getX();
    double y1 = P1.getY(), y2 = P2.getY(), y3 = P3.getY();
    double z1 = P1.getZ(), z2 = P2.getZ(), z3 = P3.getZ();

    ka = -(y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2));
    kb = -(z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2));
    kc = -(x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2));
    kd = (x1*(y2*z3 - y3*z2) + x2*(y3*z1 - y1*z3) + x3*(y1*z2 - y2*z1));
};

bool Model::is_on_model(Point3D P)
{
    bool res = false;
    double minX, maxX, minY, maxY, minZ, maxZ;
    double ka, kb, kc, kd;
    for(int l = 0; l < edges.size(); l++)
    {
        MinMax_X(edges[l], points, minX, maxX);
        MinMax_Y(edges[l], points, minY, maxY);
        MinMax_Z(edges[l], points, minZ, maxZ);
        //std::cout << minX << ' ' << maxX << ' ' << minY << ' ' << maxY << ' ' << minZ << ' ' << maxZ << '\n';
        if (minZ-EPS < P.getZ() and P.getZ() < maxZ+EPS and minY-EPS < P.getY() and P.getY() < maxY+EPS and minX-EPS < P.getX() and P.getX() < maxX+EPS)
        {
            CoefCalc(edges[l], points, ka, kb, kc, kd);
            if (abs(ka * P.getX() + kb * P.getY() + kc * P.getZ() + kd) < EPS)
            {
                res = true;
                break;
            }
            //std::cout << ka << ' ' << kb << ' ' << kc << ' ' << kd << ' ' << abs(ka * P.getX() + kb * P.getY() + kc * P.getZ() + kd) << '\n';
        }
    }
    return res;
};

Point3D Model::get_near_point_on_model(Point3D P)
{
    Point3D res, cur;
    double resl = -1, curl;
    double d;
    double curx, cury, curz;
    double minX, maxX, minY, maxY, minZ, maxZ;
    double ka, kb, kc, kd;
    for(int l = 0; l < edges.size(); l++)
    {
        MinMax_X(edges[l], points, minX, maxX);
        MinMax_Y(edges[l], points, minY, maxY);
        MinMax_Z(edges[l], points, minZ, maxZ);
        //std::cout << minX << ' ' << maxX << ' ' << minY << ' ' << maxY << ' ' << minZ << ' ' << maxZ << '\n';
            CoefCalc(edges[l], points, ka, kb, kc, kd);
            d = (ka * P.getX() + kb * P.getY() + kc * P.getZ() + kd)/(pow(ka, 2) + pow(kb, 2) + pow(kc, 2));
            curx = P.getX() + ka * d;
            cury = P.getY() + kb * d;
            curz = P.getZ() + kc * d;
            if (curx < minX)
                curx = minX;
            else if (curx > maxX)
                curx = maxX;
            if (cury < minY)
                cury = minY;
            else if (cury > maxY)
                cury = maxY;
            if (curz < minZ)
                curz = minZ;
            else if (curz > maxZ)
                 curz = maxZ;
            cur.set(curx, cury, curz);
            curl = P.sqrdistP(cur);
            if (curl < resl or resl < 0)
            {
                res = cur;
                resl = curl;
            }
     }
    return res;
};
//int Object::get_id(){return id;};

//void Object::shift(double dx, double dy, double dz){};

//void Object::set_position(double x, double y, double z){};
