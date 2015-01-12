/*
 * 使用了boost.asio 传输 opencv 获取的webcam 图像，传输的图像经过jpg压缩，传输的字节数大约为34000
 * 是原来 240*320*3 的1/6 速度更加快了。
 *
 * yuliyang@qq.com
 *
 * 2014/10/21
 *
 * @copyright yuliyang@qq.com
 *
 *
 *
 *
 * write端
 *
 */
#include <fstream>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <ctime>
#include <string>
#include <cstdio>
using boost::asio::ip::tcp;
using namespace  std;
using namespace cv;

int main(int args, char *argv[])
{
   // C:\Users\leeyoung\Desktop\test-data\Basketball\img
//    std::string pathToData("C:\\Users\\leeyoung\\Desktop\\imageseq\\lee%4d.jpeg");
    // C:\Users\leeyoung\Desktop\test-data\Basketball\img
    std::string pathToData("C:\\Users\\leeyoung\\Desktop\\imageseq\\lee%4d.jpg");
    cv::VideoCapture cap(pathToData);
    //    VideoCapture cap(0);                        /* open webcam */
    //    if(!cap.isOpened())
    //    {
    //        return -1;
    //    }
    //    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    //    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    Mat frame;

    try
    {
        boost::asio::io_service io_service;
        tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 3200);
        tcp::socket socket(io_service);
        boost::system::error_code ignored_error;
        socket.connect(end_point);
        int count=1;
        while (true)
        {
            cap>>frame;
            if (frame.empty())
                break;
            char c=(char)waitKey(100);
            if (c==27)
            {
                break;
            }
            //(1) jpeg compression
            vector<uchar> buff;//buffer for coding
            vector<int> param = vector<int>(2);
            param[0]=CV_IMWRITE_JPEG_QUALITY;
            param[1]=95;//default(95) 0-100
            imencode(".jpg",frame,buff,param);
//            cout<<"coded file size(jpg)"<<buff.size()<<endl;//fit buff size automatically.
            string headlength(std::to_string(buff.size()));
            headlength.resize(16);
            std::size_t length = boost::asio::write(socket, boost::asio::buffer(headlength), boost::asio::transfer_all(), ignored_error);
//            cout << "length : "<<length<<endl;
            std::size_t lengthbody = boost::asio::write(socket, boost::asio::buffer(string(buff.begin(),buff.end())), boost::asio::transfer_all(), ignored_error);
//            cout << "lengthbody : "<<lengthbody<<endl;
            cout<<"send image--->"<<count <<"finished"<<endl;

            count++;
        }
        cout<<"close socket"<<endl;
        socket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    cout<<"exit"<<endl;

    return 0;
}
