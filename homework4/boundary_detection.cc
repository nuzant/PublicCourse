//boundary_detection.cc
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace cv;

int main(){
    //use absolute directory!
    std::string directory = "/home/nuzant/PublicCourse/homework2/pony_data/GigECameraDeviceWideAngle/";
    cv::Mat src,dst,color_dst;
    
    for(int i = 0; i < 10; i++){
        src = imread(directory + std::to_string(i) + ".jpg" );
        Canny(src,dst,50,200,3);
        cvtColor(dst, color_dst, CV_GRAY2BGR); 

        vector<Vec4i> lines;
        HoughLinesP(dst,lines,1,CV_PI/180,80,30,10);

        for(size_t j=0; j<lines.size();j++){
            line( color_dst, Point(lines[j][0],lines[j][1]),Point(lines[j][2],lines[j][3]),Scalar(0,0,255),3,8);
        }

        //line(color_dst,Point(0,0),Point(color_dst.cols,color_dst.rows),Scalar(0,255,0),3,8);
        imshow(std::to_string(i),color_dst);
        waitKey(0);
    }

    return 0;
}