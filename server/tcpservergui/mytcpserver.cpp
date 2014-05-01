#include "mytcpserver.h"
#include <QDebug>

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 3200))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyTcpServer::newConnection()
{

    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()),
                this, SLOT(readSlot()));
}

void MyTcpServer::readSlot()
{

    qint64 blockSize = 230400; //320*240*3
    if (socket->bytesAvailable() < blockSize)//直到读到230400byte数据
        {
            return;
        }
    QByteArray b1=socket->read(230400);
    std::vector<uchar> vectordata(b1.begin(),b1.end());
    cv::Mat data_mat(vectordata,true);
    img= data_mat.reshape(3,240);       /* reshape to 3 channel and 240 rows */
    emit readyRead(); //重新运行readSlot

}


