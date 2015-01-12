#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
#include "networkworker.h"


#include <QThread>
#include <QLabel>
#include <QImage>
#include <QDebug>

using boost::asio::ip::tcp;
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QThread *thread;
    Worker *worker;

private slots:
    void on_pushButton_clicked();

private slots:
    void updateImage(const cv::Mat &img);
    void recvfinished();


private:
    Ui::MainWindow *ui;
    QLabel *msgLabel;
};

#endif // MAINWINDOW_H
