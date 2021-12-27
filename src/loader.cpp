#include "Loader.h"
void ReadPoint(std::ifstream &f, int &cur, std::vector <Point3D> &parr)
{
    double x, y, z;
    f >> x >> y >> z;
    parr.at(cur) = Point3D(x, y, z);
    cur++;
}

int ReadInt(std::ifstream &f, bool &end)
{
    if (end)
        return 0;
    char c;
    int res = 0;
    c = f.get();
    while(!f.eof() and !f.fail())
    {
        if ('0' <= c and c <= '9')
        {
            res *= 10;
            res += c - '0';
        }
        else
            if (c != ' ' or res != 0)
            {
                if (c == '\n')
                    end = true;
                break;
            }
        c = f.get();
    }
    return res;
}

void ReadEdge(std::ifstream &f, int &cur, std::vector <List> &edge)
{
    int n, n1, n2;
    bool end = false;
    //std::cout << ']';
    //std::cout << n << ' ' << n1 << c;
    while((n = ReadInt(f, end)) and (n1 = ReadInt(f, end)) and (n2 = ReadInt(f, end)))
    {
        //std::cout << n << ' ' << n1 << ' ' << n2 << '\n';
        //std::cout << (bool)(f >> n >> c >> n1 >> c >> n2);
        edge.at(cur).add(n, n1, n2);
        if (end)
            break;
    }
    cur++;
}

int countPoints(std::string &name)
{
    char cf = '\n', cs = '\n', ct = '\n';
    int count = 0;
    std::ifstream file(name);
    while (!(file.eof() or file.fail()))
    {
       cf = file.get();
       if(cf == ' ' and cs == 'v' and ct == '\n')
           count++;
       ct = cs;
       cs = cf;
    }
    file.close();
    return count;
}

int countNormals(std::string &name)
{
    char cf = '\n', cs = '\n', ct = '\n';
    int count = 0;
    std::ifstream file(name);
    while (!(file.eof() or file.fail()))
    {
       cf = file.get();
       if(cf == 'n' and cs == 'v' and ct == '\n')
           count++;
       ct = cs;
       cs = cf;
    }
    file.close();
    return count;
}

int countEdges(std::string &name)
{
    char cf, cs = ' ';
    int count = 0;
    std::ifstream file(name);
    while (!(file.eof() or file.fail()))
    {
       cf = file.get();
       if(cf == 'f' and cs == '\n')
           count++;
       cs = cf;
    }
    file.close();
    return count;
}

void ReadNormal(std::ifstream &f, int &cur, std::vector <Point3D> Vec)
{
    double x, y, z;
    f >> x >> y >> z;
    //std::cout << x << y << z << ' ';
    Vec.at(cur) = Point3D(x, y, z);
    cur++;
};

Model &Loader::loadmodel(std::string &name)
{

    int pnum = countPoints(name);
    int ednum = countEdges(name);
    int nnum = countNormals(name);
    //std::cout << pnum << ' ' << ednum << ' ' << nnum << '\n';
    std::ifstream modelfile(name);
    char c, cs = '\n';
    Model *model = new Model();
    model->points.resize(pnum);
    model->edges.resize(ednum);
    model->normals.resize(nnum);
    model->mat = Material(500, 700, Color(255, 0, 255));

    int curp = 0, cured = 0, curn = 0;
    while (!(modelfile.eof() or modelfile.fail()))
    {
        c = modelfile.get();
        if (cs == '\n')
        {
        if(c == 'v')
        {
            cs = c;
            if ((c = modelfile.get()) == 'n')
            {
                //std::cout << '/';
                ReadNormal(modelfile, curn, model->normals);
            }
            else if(c == ' ')
            {
                //std::cout << '-';
                ReadPoint(modelfile, curp, model->points);
            }
        }
        else if(c == 'f')
        {
            //std::cout << '+';
            ReadEdge(modelfile, cured, model->edges);
            c = '\n';
        }
        }
        cs = c;
    }
    modelfile.close();
    return *model;

}

std::vector <std::vector <Color>>& Loader::loadpng(std::string &name)
{

    std::vector <std::vector <Color>> *pic = new std::vector <std::vector <Color>>;

    char c;
    char r, g, b;
    int a, ir, ib, ig;
    std::vector <Color> pal;
    pal.resize(256);
    std::ifstream png(name,std::ios::binary|std::ios::in);
    //пропускаем заголовок
    for (int i = 0; i < 18; i++)
        png.read(&c, sizeof(c));

    int sizex = 0, sizey = 0;

        png.read((char*)&sizex, 4);
        png.read((char*)&sizey, 4);
    for (int i = 0; i < 28; i++)
        png.read(&c, sizeof(c));

    (*pic).resize(sizex);
    for (int j = 0; j < sizex; j++)
        (*pic)[j].resize(sizey);

    for (int i = sizey - 1; i >= 0; i--)
    {

        for (int j = 0; j < sizex; j++)
        {

        png.read((char*)&b, 1);
        png.read((char*)&g, 1);
        png.read((char*)&r, 1);
        ir = ((int)r<0)?256+(int)r:(int)r;
        ig = ((int)g<0)?256+(int)g:(int)g;
        ib = ((int)b<0)?256+(int)b:(int)b;
        a = (ir + ib + ig)/12 + 1;
         (*pic)[j][i].set_color(a, ir, ig, ib);

        }
        for (int i = 0; ((sizex*3)%4) and i < (4 - (sizex*3)%4);i++)
            png.read((char*)&c, 1);
    }

    //std::cout << sizex << ' ' << sizey << '\n';
    png.close();
    return *pic;
};
