#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTimer>
#include <QPixmap>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "mytcpserver.h"
using namespace cv;
using namespace std;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Mat *showimg;                               /* 可以去掉 */
    MyTcpServer *server;                        /* 服务器端 */

private:
    Ui::Dialog *ui;
    QTimer *timer;                              /* 定时器，更新界面 */
private slots:
    void getFrame(); //实现定时从摄像头取图并显示在label上的功能。

    void on_pushButton_clicked();
};

#endif // DIALOG_H
