#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include <QString>
#include <QMessageBox>
#include <QScreen>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void imgShow();
    void zoom1(float angle,float scale);
    QString imgPath;
    QImage gray(QImage image);
    QImage AdjustContrast(QImage image, int value);
    QImage AdjustSaturation(QImage image, int value);
    QImage setRGB(QImage image,int value_r,int value_g,int value_b);
    QImage fuhe(QImage images);
    QImage gamma(QImage image);
    QImage bianyuan(QImage image);
    QImage junzhi(QImage image);
    Mat masaike(Mat image);

private slots:
    void on_action_Open_triggered();

    void on_horizontalSlider_light1_valueChanged(int value);

    void on_horizontalSlider_binary1_valueChanged(int value);

    void on_horizontalSlider_contrast1_valueChanged(int value);

    void on_horizontalSlider_saturation1_valueChanged(int value);

    void on_horizontalSlider_red1_valueChanged(int value);

    void on_horizontalSlider_green1_valueChanged(int value);

    void on_horizontalSlider_blue1_valueChanged(int value);

    void on_pushButton_left1_clicked();

    void on_pushButton_right1_clicked();

    void on_pushButton_horizontal1_clicked();

    void on_pushButton_vertically1_clicked();

    void on_action_H_triggered();

    void on_action_J_triggered();

    void on_action_B_triggered();

    void on_action_Y_triggered();

    void on_action_G_triggered();

    void on_pushButton_reload1_clicked();

    void on_pushButton_Save1_clicked();

    void on_pushButton_faceregonize1_clicked();

    void on_horizontalSlider_zoom1_valueChanged(int value);

    void on_horizontalSlider_zoom1_sliderMoved(int position);

    void on_action_Save_triggered();

    void on_action_Qt_triggered();

    void on_action_CvMei_triggered();

private:
    Ui::MainWindow *ui;
    Mat myImg;
    QImage image;
    QImage myQImg;
    QString origin_path;
};
#endif // MAINWINDOW_H
