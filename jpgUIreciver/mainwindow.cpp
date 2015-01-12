#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thread = new QThread();
    worker = new Worker();
    worker->moveToThread(thread);

    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(worker, SIGNAL(imageChanged(const cv::Mat &)), this, SLOT(updateImage(const cv::Mat &)));

    connect(worker, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), worker, SLOT(doWork()));

    /*
     * update label for running statue
     */

    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    connect(worker, SIGNAL(finished()), this, SLOT(recvfinished()));


    /*
     * update label for finished status
     */


}

MainWindow::~MainWindow()
{

    worker->abort();
    thread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete worker;
    delete thread;

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    worker->abort();
    thread->wait(); // If the thread is not running, this will immediately return.
    worker->requestWork();

}

void MainWindow::updateImage(const Mat &img)
{

    /*
     * just show the image
     */
    QImage labelimage= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    ui->label_show->setPixmap(QPixmap::fromImage(labelimage));
    ui->label_show->repaint ();

    /*
     * do something to the mat
     * HOG people detect or Haar face detect
     */

}


/*
 * show statusbar message
 */
void MainWindow::recvfinished()
{
    //    QLabel *message;
    //    cout<<"finished...";
    //    qDebug()<<"finished...............................";
//    msgLabel = new QLabel;
//    msgLabel->setMinimumSize (msgLabel->sizeHint ());
//    msgLabel->setAlignment (Qt::AlignCenter);
//    msgLabel->setText ("finised");
//    ui->statusBar->addWidget (msgLabel);

}


