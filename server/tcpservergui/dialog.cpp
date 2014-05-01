#include "dialog.h"
#include "ui_dialog.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QDebug>

using namespace cv;
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

//    QImage init("C:/Users/leeyoung/Desktop/tcpservergui/img/tinyproxy.jpg");
    ui->setupUi(this);
//    ui->label_3->setPixmap(QPixmap::fromImage(init));
    setWindowTitle("服务器端");
    ui->comboBox->addItem ("算法1");
    ui->comboBox->addItem ("算法2");
    ui->comboBox->addItem ("算法3");
    timer = new QTimer(this);
    timer->start(10); //1000为1秒，10毫秒去取一帧
    connect(timer,SIGNAL(timeout()),this,SLOT(getFrame())); //超时就去取
    server = new MyTcpServer();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getFrame(){
    QImage image= QImage((uchar*) server->img.data, server->img.cols, server->img.rows, server->img.step, QImage::Format_RGB888).rgbSwapped();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void Dialog::on_pushButton_clicked()
{
    QMessageBox::information (this,"title",ui->comboBox->currentText ());//获取当前选择的字符串
}
