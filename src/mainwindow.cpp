#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loader.h"
#include <QPainter>
#include "screen.h"
#include "algoritms.h"
#include <unistd.h>
#include "mutexs.h"
std::mutex draw_mtx;

#include <QRegExpValidator>



MainWindow::~MainWindow()
{
    action->~thread();
    delete ui;
}



unsigned int PRNG(int i)
{
    static unsigned int seed = 4541;

    seed = (8253729 * seed + 2396403);

    return seed % i;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    draw_mtx.lock();
    QPainter painter;
    painter.begin(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    QPoint point;

    for (int x = 0; x < WINWIDTH; x++, point.setX(x))
    {
        for (int y = 0; y < WINHEIGHT;y++, point.setY(y))
        {
            painter.setPen(screen->GetColor(x, y)/*QColor(PRNG(255), PRNG(255), PRNG(255))*/);
            /*std::cout << screen->GetColor(x, y).red();
            std::cout << ' ';
            std::cout << screen->GetColor(x, y).blue();
            std::cout << ' ';
            std::cout << screen->GetColor(x, y).green();
            std::cout << ' ';*/
            painter.drawPoints(&point, 1);
        }
        //std::cout << '\n';
    }

    painter.setPen(Qt::black);
    if (scene->model.size() > 0 and ISEDGE)
    {
        //scene->model[0].print();
    for (int i = 0; i < scene->model[0].edges.size(); i++)
    {
    List edge = scene->model[0].edges[i];
    edge.to_end();
    int cur = edge.get_npoint() - 1;
    for (edge.to_begin(); !edge.is_end(); edge.next())
    {
        //std::cout << cur << ' ';
        painter.drawLine(screen->P[cur].getX(), screen->P[cur].getY(), screen->P[edge.get_npoint() - 1].getX(), screen->P[edge.get_npoint() - 1].getY());
        cur = edge.get_npoint() - 1;
    }
    //std::cout << cur << '\n';
    painter.drawLine(screen->P[cur].getX(), screen->P[cur].getY(), screen->P[edge.get_npoint() - 1].getX(), screen->P[edge.get_npoint() - 1].getY());
    }
}
    painter.end();
    draw_mtx.unlock();
}

void actionThread(thr data)
{
    try{
    while(true)
    {
    if (data.model.size() > 0)
    {
    comand_mtx.lock();
    data.scene.proect(data.Points);
    data.scene.proect_light(data.light);
    data.scene.proect_flame(data.FlameP);
    data.flame.refrash_data();
    data.flame.check_genpoints(data.model[0]);
    comand_mtx.unlock();
    draw_mtx.lock();
    data.screen.zbuffer(data.model);
    data.screen.flame_visualise();
    draw_mtx.unlock();
    data.m.update();
    }
    }
    }
    catch(std::exception& e)
    {

    }
}

void MainWindow::on_PushB_clicked()
{
    LoadModel loadcom(scene->flame, scene->model, ui->comboBox->currentIndex());
    loadcom.run();

    //std::string s = "./coub.obj";
    //scene->add(Loader().loadmodel(s));
    //for (int i = 0; i < 300; i++)
    //while (true)
    {
    //scene->Cam.yaw((double)(M_PI));
    //scene->Cam.pitch((double)(M_PI/3));
    //scene->Lights[0].Shift(0, 1, 0);

    //data.scene.proect_flame(data.FlameP);
    //data.flame.refrash_data();
    //data.screen.flame_visualise();
    //data.m.repaint();
    //sleep(50);
    }
    //std::cout << "hello";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cx = "0";cy = "0";cz = "-4";ay = "0";ap = "0";
    ir = "1"; ig = "1"; ib = "1"; lx = "0"; ly = "0"; lz = "-4";
    ui->setupUi(this);
    screen = new Screen;
    scene = new Scene;
    std::string coub = ModelName;
    scene->add(Loader().loadmodel(coub));
    scene->model[0].mat = Material(400, 1000, Color(139, 69, 19));
    //scene->model[0].is_on_model(Point3D(0, 0, 0));
    thr data = thr(*scene, *screen, *this, screen->Flame, scene->flame, screen->P, screen->Lights, scene->model);
    action = new std::thread(actionThread, data);
}

void MainWindow::on_SetCam_clicked(){SetCamera com(scene->Cam, cx, cy, cz); com.run();};
void MainWindow::on_SetCamAng_clicked(){SetCameraAngle com(scene->Cam, ap, ay); com.run();};
void MainWindow::on_SetL_clicked(){SetLight com(scene->Lights, lx, ly, lz, ir, ig, ib); com.run();};
void MainWindow::on_AddFire_clicked(){AddFire com(scene->model[0], scene->flame, fx, fy, fz); com.run();};
void MainWindow::on_LightX_textChanged(const QString & text){lx = text.toStdString();};
void MainWindow::on_LightY_textChanged(const QString & text){ly = text.toStdString();};
void MainWindow::on_LightZ_textChanged(const QString & text){lz = text.toStdString();};
void MainWindow::on_CameraX_textChanged(const QString & text){cx = text.toStdString();};
void MainWindow::on_CameraY_textChanged(const QString & text){cy = text.toStdString();};
void MainWindow::on_CameraZ_textChanged(const QString & text){cz = text.toStdString();};
void MainWindow::on_CameraYaw_textChanged(const QString & text){ay = text.toStdString();};
void MainWindow::on_CameraPitch_textChanged(const QString & text){ap = text.toStdString();};
void MainWindow::on_LightIr_textChanged(const QString & text){ir = text.toStdString();};
void MainWindow::on_LightIg_textChanged(const QString & text){ig = text.toStdString();};
void MainWindow::on_LightIb_textChanged(const QString & text){ib = text.toStdString();};
void MainWindow::on_FireX_textChanged(const QString & text){fx = text.toStdString();};
void MainWindow::on_FireY_textChanged(const QString & text){fy = text.toStdString();};
void MainWindow::on_FireZ_textChanged(const QString & text){fz = text.toStdString();};
