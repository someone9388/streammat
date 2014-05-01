/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *	  	
 *    Description:  stream opencv Mat frame to server by tcp with boost asio
 *
 *
 *        Version:  1.0
 *        Created:  2014/4/29 11:40:20
 *         Author:  yuliyang
 *
 *		     Mail:  wzyuliyang911@gmail.com
 *			 Blog:  http://www.cnblogs.com/yuliyang
 *
 * =====================================================================================
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>  
#include <ctime>
#include <string>
using boost::asio::ip::tcp;
using namespace  std;
using namespace cv;

int main()
{
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector()); /* 直接调用opencv里的HOG特征检测 */
	VideoCapture cap(0);                        /* open webcam */
	if(!cap.isOpened())  
	{  
		return -1;  
	}  

	
	Mat frame;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);      /* set width */
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);     /* set height */

	try
	{
		boost::asio::io_service io_service;
		tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 3200);

		tcp::socket socket(io_service);
		socket.connect(end_point);
		boost::system::error_code ignored_error;
		
		while (true)
		{
			vector<Rect> found, found_filtered;
			size_t i, j;
			cap>>frame;
			hog.detectMultiScale(frame, found, 0, Size(4,4), Size(0,0), 1.05, 2);
			//for (i=0; i<found.size(); i++)          /* 实际运用时候可以注视掉框出行人的步骤，来加快速度 */
			//{
			//	Rect r = found[i];
			//	for (j=0; j<found.size(); j++)
			//		if (j!=i && (r & found[j])==r)
			//			break;
			//	if (j==found.size())
			//		found_filtered.push_back(r);
			//}
			//for (i=0; i<found_filtered.size(); i++)
			//{
			//	Rect r = found_filtered[i];
			//	r.x += cvRound(r.width*0.1);
			//	r.width = cvRound(r.width*0.8);
			//	r.y += cvRound(r.height*0.06);
			//	r.height = cvRound(r.height*0.9);
			//	rectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 1);
			//}
			
			imshow("client",frame);   
		
			char c=(char)waitKey(100);
			if (c==27)
			{
				break;
			}

			if (found.size()>=1) {

				frame = (frame.reshape(0,1)); // to make it continuous
				std::string message((char *)frame.data,230400); /* the size of mat data is 320*240*3 */
				socket.write_some(boost::asio::buffer(message), ignored_error);
				cout<<"send image finished"<<endl;


			}
			else
			{
				cout<<"no image to send "<<endl;
			}

		
		}

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

		return 0;
}
