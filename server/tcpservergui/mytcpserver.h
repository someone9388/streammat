#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
using namespace std;
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    Mat  img;
signals:
    void readyRead();

public slots:
    void newConnection();
    void readSlot();

private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYTCPSERVER_H

