//lane.cc
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
    
    for(int i = 0; i < 100; i++){
        src = imread(directory + std::to_string(i) + ".jpg" );
        Canny(src,dst,50,200,3);
        cvtColor(dst, color_dst, CV_GRAY2BGR); 

        vector<int> x1,y1,x2,y2;
        vector<Vec4i> lines;
        vector<int> detected;
        HoughLinesP(dst,lines,1,CV_PI/180,80,30,10);
        x1.resize(lines.size());
        y1.resize(lines.size());
        x2.resize(lines.size());
        y2.resize(lines.size());
        for(size_t j=0; j<lines.size();j++ ){
            x1[j] = lines[j][0];
            y1[j] = lines[j][1];
            x2[j] = lines[j][2];
            y2[j] = lines[j][3];
        } 

        for(size_t j=0; j<lines.size();j++){
            if(x1[j] - x2[j] < 100 && x1[j] - x2[j]>-100) continue; //delete vertical lines
            if(y1[j]-y2[j]<40 && y1[j]-y2[j]>-40) continue; //delete horizonal lines
            if(y1[j]<500 || y2[j]<500) continue; //delete lines too high
            if((y1[j]>500 && y2[j]<600)||(y2[j]>500 && y1[j]<600)) continue; //delete mid lines
            //this is a trivial solution but working relatively good :)
            detected.push_back(j);
        }

        for(size_t j=0; j<detected.size();j++){
            int p = detected[j];
            line( color_dst, Point(x1[p],y1[p]),Point(x2[p],y2[p]),Scalar(0,0,255),3,8);
        }
        //line(color_dst,Point(0,0),Point(color_dst.cols,color_dst.rows),Scalar(0,255,0),3,8);
        imshow(std::to_string(i),color_dst);
        waitKey(0);
    }

    return 0;
}