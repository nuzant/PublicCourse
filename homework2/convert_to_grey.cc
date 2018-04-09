// Copyright @2018 Pony AI Inc. All rights reserved.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

void convert(const Mat &src, Mat &dst){
    dst = Mat (src.rows, src.cols, CV_8UC1, Scalar(0,0,0));

    for(int i = 0 ; i < src.rows; i++){
        for(int j = 0; j < src.cols; j ++){
            uchar b = src.at<Vec3b>(i,j)[0];
            uchar g = src.at<Vec3b>(i,j)[1];
            uchar r = src.at<Vec3b>(i,j)[2];

            dst.at<uchar>(i,j) = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }
}

int main() {
  cv::Mat image;
  // ATTENTION!!! : please use absolute path for reading the data file.
  image = imread("/home/nuz/workspace/PublicCourse/homework2/sample_data/GigECameraDeviceWideAngle/0.jpg", CV_LOAD_IMAGE_COLOR);
  cv::Mat grey;

  grey.release();
  convert(image,grey);

  namedWindow("greyscale");
  imshow("greyscale", grey);
  waitKey(0);
  return 0;
}