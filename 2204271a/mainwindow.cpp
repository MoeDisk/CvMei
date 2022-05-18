#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QtCore/qmath.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(),this->height());
    QLabel *permanent=new QLabel(this);
    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    permanent->setText(tr("Presentation Draft Version."));
    ui->statusBar1->addPermanentWidget(permanent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::imgShow(){
    ui->viewLabel->setPixmap(QPixmap::fromImage(myQImg.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
}

QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;
        for(int y = 0; y < newImage.height(); y++)
        {
            for(int x = 0; x < newImage.width(); x++)
            {
                oldColor = QColor(image.pixel(x,y));
                int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                newImage.setPixel(x, y, qRgb(average, average, average));
            }
        }
        return newImage;
}

QImage MainWindow::junzhi(QImage image){
    int kernel [3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}};
        int sizeKernel = 3;
        int sumKernel = 9;
        QColor color;
         for(int x = sizeKernel/2;x<image.width() - sizeKernel/2;x++)
        {
           for(int y= sizeKernel/2;y<image.height() - sizeKernel/2;y++)
            {
                int r = 0;
                int g = 0;
                int b = 0;
                for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)
                {
                   for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
                    {
                     color = QColor(image.pixel(x+i,y+j));
                     r+=color.red()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                     g+=color.green()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                     b+=color.blue()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                    }
                }
                r = qBound(0,r/sumKernel,255);
                g = qBound(0,g/sumKernel,255);
                b = qBound(0,b/sumKernel,255);
                image.setPixel(x,y,qRgb( r,g,b));
            }
        }
         return image;
}

QImage MainWindow::fuhe(QImage images){
    QImage image2 =images.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < images.height() - 1; y++)
    {
        for(int x = 0; x < images.width() - 1; x++)
        {
            color0 =   QColor ( images.pixel(x,y));
            color1 =   QColor ( images.pixel(x + 1,y));
            color2 =   QColor ( images.pixel(x,y + 1));
            color3 =   QColor ( images.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            image2.setPixel(x,y,qRgb(a,a,a));
        }
    }
}

QImage MainWindow::AdjustContrast(QImage image, int value)
{
    int pixels = image.width() * image.height();
    unsigned int *data = (unsigned int *)image.bits();
    int red, green, blue, nRed, nGreen, nBlue;
    if (value > 0 && value < 256)
    {
        float param = 1 / (1 - value / 256.0) - 1;
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);
            red = nRed + (nRed - 127) * param;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * param;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * param;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    else
    {
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);
            red = nRed + (nRed - 127) * value / 100.0;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * value / 100.0;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * value / 100.0;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    return image;
}

QImage MainWindow::gamma(QImage image){
    double d=1.2;
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

QImage MainWindow::bianyuan(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < image.height() - 1; y++)
    {
        for(int x = 0; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));
            color1 =   QColor ( image.pixel(x + 1,y));
            color2 =   QColor ( image.pixel(x,y + 1));
            color3 =   QColor ( image.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}

QImage MainWindow::AdjustSaturation(QImage Img, int iSaturateValue)
{
    int red, green, blue, nRed, nGreen, nBlue;
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    float Increment = iSaturateValue/100.0;

    float delta = 0;
    float minVal, maxVal;
    float L, S;
    float alpha;

    for (int i = 0; i < pixels; ++i)
    {
        nRed = qRed(data[i]);
        nGreen = qGreen(data[i]);
        nBlue = qBlue(data[i]);

        minVal = std::min(std::min(nRed, nGreen), nBlue);
        maxVal = std::max(std::max(nRed, nGreen), nBlue);
        delta = (maxVal - minVal) / 255.0;
        L = 0.5*(maxVal + minVal) / 255.0;
        S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

        if (Increment > 0)
        {
            alpha = std::max(S, 1 - Increment);
            alpha = 1.0 / alpha - 1;
            red = nRed + (nRed - L*255.0)*alpha;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - L*255.0)*alpha;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - L*255.0)*alpha;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }
        else
        {
            alpha = Increment;
            red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }

        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }

    return Img;
}

void MainWindow::on_action_Open_triggered()
{
    QString imgPath=QFileDialog::getOpenFileName(this);
    origin_path=imgPath;
    Mat imgData=imread(origin_path.toLatin1().data());
    cvtColor(imgData,imgData,COLOR_BGR2RGB);
    myImg=imgData;
    myQImg=QImage((const unsigned char*)(imgData.data),imgData.cols,imgData.rows,QImage::Format_RGB888);
    imgShow();
}

void MainWindow::on_horizontalSlider_light1_valueChanged(int value)
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    int red, green, blue;
        int pixels = image.width() * image.height();
        unsigned int *data = (unsigned int *)image.bits();
        for (int i = 0; i < pixels; ++i)
        {
            red= qRed(data[i])+ value;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green= qGreen(data[i]) + value;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue= qBlue(data[i]) + value;
            blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
        ui->viewLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
        ui->label_light2->setText(QString::number(value));
    }else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

QImage MainWindow::setRGB(QImage image,int value_r, int value_g, int value_b){
    int r,g,b;
    QColor oldColor;
    int height=image.height();
    int width=image.width();
    for (int i = 0; i < height; ++i)
    {
        for(int j=0;j<width;++j){
            oldColor = QColor(image.pixel(j,i));
            r=oldColor.red()+value_r;
            if(r>255)
                r=255;
            g=oldColor.green()+value_g;
            if(g>255)
                g=255;
            b=oldColor.blue()+value_b;
            if(b>255)
                b=255;
            image.setPixel(j,i, qRgb(r, g, b));
        }
    }
    return image;
}

void MainWindow::on_horizontalSlider_binary1_valueChanged(int value)
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage images=gray(image);
    int height=images.height();
    int width=images.width();
            int bt;
            QColor oldColor;
            for (int i = 0; i < height; ++i)
            {
                for(int j=0;j<width;++j){
                    oldColor = QColor(images.pixel(j,i));
                    bt = oldColor.red();
                if(bt<value){
                    bt=0;
                }else{
                    bt=255;
                }
                images.setPixel(j,i, qRgb(bt, bt, bt));

                }
            }
    ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
    ui->viewLabel->setAlignment(Qt::AlignCenter);
    ui->label_binary2->setText(QString::number(value));
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_horizontalSlider_contrast1_valueChanged(int value)
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage images=AdjustContrast(image,value);
    ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
    ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_horizontalSlider_saturation1_valueChanged(int value)
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage images=AdjustSaturation(image,value);
    ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
    ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_horizontalSlider_red1_valueChanged(int value)
{
    if(ui->viewLabel->pixmap()!=nullptr){
        int value_r=value;
        int value_g=ui->horizontalSlider_green1->value();
        int value_b=ui->horizontalSlider_blue1->value();
        QImage image(origin_path);
        QImage images=setRGB(image,value_r,value_g,value_b);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_horizontalSlider_green1_valueChanged(int value)
{
    if(ui->viewLabel->pixmap()!=nullptr){
        int value_r=ui->horizontalSlider_red1->value();
        int value_g=value;
        int value_b=ui->horizontalSlider_blue1->value();
        QImage image(origin_path);
        QImage images=setRGB(image,value_r,value_g,value_b);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_horizontalSlider_blue1_valueChanged(int value)
{
    if(ui->viewLabel->pixmap()!=nullptr){
        int value_r=ui->horizontalSlider_red1->value();
        int value_g=ui->horizontalSlider_green1->value();
        int value_b=value;
        QImage image(origin_path);
        QImage images=setRGB(image,value_r,value_g,value_b);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_left1_clicked()
{
    if(ui->viewLabel->pixmap()!=nullptr){
        QImage images(ui->viewLabel->pixmap()->toImage());
        QMatrix matrix;
        matrix.rotate(-90.0);
        images= images.transformed(matrix,Qt::FastTransformation);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_right1_clicked()
{
    if(ui->viewLabel->pixmap()!=nullptr){
        QImage images(ui->viewLabel->pixmap()->toImage());
        QMatrix matrix;
        matrix.rotate(90.0);
        images= images.transformed(matrix,Qt::FastTransformation);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_horizontal1_clicked()
{
    if(ui->viewLabel->pixmap()!=nullptr){
        QImage images(ui->viewLabel->pixmap()->toImage());
        images = images.mirrored(true, false);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_vertically1_clicked()
{
    if(ui->viewLabel->pixmap()!=nullptr){
        QImage images(ui->viewLabel->pixmap()->toImage());
        images = images.mirrored(false, true);
        ui->viewLabel->setPixmap(QPixmap::fromImage(images));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_action_H_triggered()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage images=gray(image);
    ui->viewLabel->setPixmap(QPixmap::fromImage(images.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
    ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_action_J_triggered()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
        image=junzhi(image);
             ui->viewLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
             ui->viewLabel->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_action_B_triggered()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage newImage =bianyuan(image);
    ui->viewLabel->setPixmap(QPixmap::fromImage(newImage.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
    ui->viewLabel->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_action_Y_triggered()
{
    if(origin_path!=nullptr){
        QImage images(origin_path);
        QImage image2 =fuhe(images);
        ui->viewLabel->setPixmap(QPixmap::fromImage(image2.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_action_G_triggered()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        image=gamma(image);
        ui->viewLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_reload1_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        ui->viewLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
        ui->viewLabel->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


void MainWindow::on_pushButton_faceregonize1_clicked()
{
    CascadeClassifier face_detector;
        CascadeClassifier eyes_detector;
        string fDetectorPath="/mnt/MoeDisk/OpenCV_Qt_Learn-master/Pic_FaceRecognition_1/haarcascade_frontalface_alt.xml";
        face_detector.load(fDetectorPath);
        string eDetectorPath="/mnt/MoeDisk/OpenCV_Qt_Learn-master/Pic_FaceRecognition_1/haarcascade_eye_tree_eyeglasses.xml";
        eyes_detector.load(eDetectorPath);
        vector<Rect> faces;
        Mat imgSrc=myImg;
        Mat imgGray;
        cvtColor(imgSrc,imgGray,COLOR_RGB2GRAY);
        equalizeHist(imgGray,imgGray);
        face_detector.detectMultiScale(imgGray,faces,1.1,2,0|CASCADE_SCALE_IMAGE,Size(30,30));
        for(int i=0;i<faces.size();i++){
                Point center(faces[i].x+faces[i].width*0.5,faces[i].y+faces[i].height*0.5);
                ellipse(imgSrc,center,Size(faces[i].width*0.5,faces[i].height*0.5),0,0,360,Scalar(255,0,255),4,8,0);
                Mat faceROI=imgGray(faces[i]);
                vector<Rect> eyes;
                eyes_detector.detectMultiScale(faceROI,eyes,1.1,2,0|CASCADE_SCALE_IMAGE,Size(30,30));
        for(int j=0;j<eyes.size();j++){
                    Point center(faces[i].x+eyes[j].x+eyes[j].width*0.5,faces[i].y+eyes[j].y+eyes[j].height*0.5);
                    int radius=cvRound((eyes[j].width+eyes[i].height)*0.25);
                    circle(imgSrc,center,radius,Scalar(255,0,0),4,8,0);
                }
            }
            Mat imgDst=imgSrc;
            myQImg=QImage((const unsigned char*)(imgDst.data),imgDst.cols,imgDst.rows,QImage::Format_RGB888);
            QMessageBox::about(NULL, " ", "Done");
        ui->viewLabel->setPixmap(QPixmap::fromImage(myQImg.scaled(ui->viewLabel->size(),Qt::KeepAspectRatio)));
}

void MainWindow::on_pushButton_Save1_clicked()
{
    QString filename=QFileDialog::getSaveFileName(this, tr("保存图片"),"result0",tr("图片文件(*.png *jpg *.jpeg *.bmp"));
    ui->viewLabel->pixmap()->toImage().save(filename);
}

void MainWindow::on_action_Save_triggered(){
    QString filename=QFileDialog::getSaveFileName(this, tr("保存图片"),"result0",tr("图片文件(*.png *jpg *.jpeg *.bmp"));
    ui->viewLabel->pixmap()->toImage().save(filename);
}

void MainWindow::zoom1(float ang, float sca){
    Point2f srcMatrix[3];
    Point2f dstMatrix[3];
    Mat imgRot(2,3,CV_32FC1);
    Mat imgSrc=myImg;
    Mat imgDst;
    Point centerPoint=Point(imgSrc.cols/2,imgSrc.rows/2);
    imgRot=getRotationMatrix2D(centerPoint,ang,sca);
    warpAffine(imgSrc,imgDst,imgRot,imgSrc.size());
    myQImg=QImage((const unsigned char*)(imgDst.data),imgDst.cols,imgDst.rows,QImage::Format_RGB888);
    imgShow();
}

void MainWindow::on_horizontalSlider_zoom1_sliderMoved(int position){
    zoom1(float(ui->horizontalSlider_zoom1->value()-360),position/100.0);
}

void MainWindow::on_horizontalSlider_zoom1_valueChanged(int value){
    zoom1(float(ui->horizontalSlider_zoom1->value()-360),value/100.0);
}

void MainWindow::on_action_Qt_triggered(){
    QMessageBox::aboutQt(NULL,"关于Qt");
}

void MainWindow::on_action_CvMei_triggered(){
    QMessageBox::about(this,tr("关于"),tr("test"));
}
