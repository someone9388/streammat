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


/*
 * the data recived is about 20K bytes for 320*240 color image each frame
 *
 *
 */

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>


#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;
using namespace cv;
using namespace std;



class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    /**
     * @brief Requests the process to start
     *
     * It is thread safe as it uses #mutex to protect access to #_working variable.
     */
    void requestWork();
    /**
     * @brief Requests the process to abort
     *
     * It is thread safe as it uses #mutex to protect access to #_abort variable.
     */
    void abort();


public:
    /**
     * @brief Process is aborted when @em true
     */
    bool _abort;
    /**
     * @brief @em true when Worker is doing work
     */
    bool _working;
    /**
     * @brief Protects access to #_abort
     */
    QMutex mutex;


    QString filename;
    int width;
    int height;

signals:

    void running();
    /**
     * @brief This signal is emitted when the Worker request to Work
     * @sa requestWork()
     */
    void workRequested();
    /**
     * @brief This signal is emitted when counted value is changed (every sec)
     */
    void valueChanged(const QString &value);
    /**
     * @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
     */
    void finished();
    void imageChanged(const cv::Mat &img);

public slots:
    /**
     * @brief Does something
     *
     */
    virtual void doWork();
};

#endif // WORKER_H
