/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of simple-qt-thread-example, a simple example to demonstrate how to use threads.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-simple-and-stable-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "networkworker.h"
#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
    _working =false;
    _abort = false;
}

void Worker::requestWork()
{
    mutex.lock();
    _working = true;
    _abort = false;
    qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();
    mutex.unlock();
    emit workRequested();
}

void Worker::abort()
{
    mutex.lock();
    if (_working) {
        _abort = true;
        qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    }
    mutex.unlock();
}

void Worker::doWork()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();
    Mat  pngimage;
    try{
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3200));
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        boost::system::error_code error;
        while(true)
        {
            mutex.lock();
            bool abort = _abort;
            mutex.unlock();
            if (abort) {
                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                break;
            }
            //do the main work here
            boost::array< char, 16 > header;
            std::size_t length = boost::asio::read(socket, boost::asio::buffer(header), boost::asio::transfer_all(), error);
            cout<<"length : "<< length<<endl;
            if(length == 16)
            {

                std::vector<uchar> body(atoi((string(header.begin(),header.end())).c_str()));
                std::size_t lengthbody = boost::asio::read(socket, boost::asio::buffer(body), boost::asio::transfer_all(), error);
                pngimage = imdecode(Mat(body),CV_LOAD_IMAGE_COLOR);
                Mat tempimg;
                pngimage.copyTo (tempimg);

                /*
                 * hard process
                 *
                 */
//                QEventLoop loop;
//                QTimer::singleShot(100, &loop, SLOT(quit()));
//                loop.exec();



                emit imageChanged (tempimg);

            }else{

                cout<<"here go length==0"<<endl;
                cout<<"nothing to read..."<<endl;
                break;

            }


        }
        cout<<"ui socket close"<<endl;
        socket.close();
        io_service.stop ();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }


    mutex.lock();
    _working = false;
    mutex.unlock();
    qDebug()<<"Worker *** process finished in Thread "<<thread()->currentThreadId();
    emit finished();
}



