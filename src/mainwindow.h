#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Objects.h"
#include "screen.h"
#include <thread>
#include "comands.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend void actionThread(Scene &scene, Screen &screen);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    Screen *screen;
    Scene *scene;
    std::thread *action;
    std::string lx, ly, lz, ay, ap, cx, cy, cz, fx, fy, fz, ir, ig, ib;
private slots:
void on_PushB_clicked();
void on_SetCam_clicked();
void on_SetCamAng_clicked();
void on_SetL_clicked();
void on_AddFire_clicked();
void on_LightX_textChanged(const QString & text);
void on_LightY_textChanged(const QString & text);
void on_LightZ_textChanged(const QString & text);
void on_CameraX_textChanged(const QString & text);
void on_CameraY_textChanged(const QString & text);
void on_CameraZ_textChanged(const QString & text);
void on_CameraYaw_textChanged(const QString & text);
void on_CameraPitch_textChanged(const QString & text);
void on_LightIr_textChanged(const QString & text);
void on_LightIg_textChanged(const QString & text);
void on_LightIb_textChanged(const QString & text);
void on_FireX_textChanged(const QString & text);
void on_FireY_textChanged(const QString & text);
void on_FireZ_textChanged(const QString & text);

};

struct thr
{
    thr(Scene &scene,Screen &screen,MainWindow &m,std::vector <Point3D> &FlameP,Flame &flame,std::vector <Point3D> &Points, std::vector <Light> &light, std::vector <Model> &model):
        scene(scene),
        screen(screen),
        m(m),
        FlameP(FlameP),
        flame(flame),
        Points(Points),
        light(light),
        model(model)
    {};
    Scene &scene;
    Screen &screen;
    MainWindow &m;
    std::vector <Point3D> &FlameP;
    Flame &flame;
    std::vector <Point3D> &Points;
    std::vector <Light> &light;
    std::vector <Model> &model;
};
#endif // MAINWINDOW_H
