// Copyright @2018 Pony AI Inc. All rights reserved.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;

void distort(const Mat &src, Mat &dst,const double k1,const double k2){
  dst = Mat(src.rows,src.cols, CV_8UC3,Scalar(0,0,0));
  
  double x1,y1,x2,y2,cx,cy,fx,fy,r,scale;
  int u,v;
  cx = src.rows/2;
  cy = src.cols/2;
  fx = 600;
  fy = 600;
  scale = 0.7;

  for(int i = 0 ;i < src.rows; i++){
    for(int j = 0;j < src.cols; j++){
      x1 = (i - cx)/fx;
      y1 = (j - cy)/fy;
      r = x1*x1+y1*y1;

      x2 = x1*(1+r*k1+r*r*k2);
      y2 = y1*(1+r*k1+r*r*k2);

      u = x2*fx*scale +cx;
      v = y2*fx*scale +cy;

      //std::cout<<src.rows<<" "<<src.cols<<std::endl;
      //std::cout<<i<<" "<<j<<std::endl;
      //std::cout<<u<<" "<<v<<std::endl;
      if(u<src.rows && v<src.cols && u>=0 && v>=0)
        dst.at<Vec3b>(u,v) = src.at<Vec3b>(i,j);
    }
  }
}

int main() {
  cv::Mat image;
  // ATTENTION!!! : please use absolute path for reading the data file.
  image = imread("/home/nuzant/PublicCourse/homework3/chessboard/chessboard_undistorted.png", CV_LOAD_IMAGE_COLOR);

  cv::Mat dst;
  dst.release();
  distort(image,dst,0,0);

  cv::Mat dst1;
  dst1.release();
  distort(image,dst1,0.1,0.1);
  
  cv::Mat dst2;
  dst2.release();
  distort(image,dst2,-0.1,-0.1);

  imshow("undistorted",dst);
  waitKey(0);
  imshow("distorted(k positive)", dst1);
  waitKey(0);
  imshow("distorted(k negative)", dst2);
  waitKey(0);
  return 0;
}