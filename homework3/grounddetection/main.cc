// Copyright @2018 Pony AI Inc. All rights reserved.

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/flann/defines.h>
#include <opencv2/flann/config.h>

#include "homework3/grounddetection/pointcloud.h"

int main() {
  int framenum = 200;
  double maxx = 0.0;
  double maxy = 0.0;
  double minx = 999.0;
  double miny = 999.0;
  std::vector<Eigen::Vector3d> accumulated;
  // ATTENTION!!! : please use absolute path for reading the data file.
  for(int i = 0; i < framenum; i ++){ //accumulating points
    PointCloud pointcloud = ReadPointCloudFromTextFile(
      "/home/nuz/workspace/PublicCourse/homework2/pony_data/VelodyneDevice32c/"+std::to_string(i)+".txt");
/*    std::cout << "Total points read: " << pointcloud.points.size() << std::endl;
    std::cout << "Rotation: " << std::endl;
    std::cout << pointcloud.rotation << std::endl;
    std::cout << "Translation: " << std::endl;
    std::cout << pointcloud.translation.transpose() << std::endl; */
    for(int j = 0; j < pointcloud.points.size(); j++){
      Eigen::Vector3d p;
      p = (pointcloud.rotation * pointcloud.points[j]) + pointcloud.translation;
      //std::cout <<pointcloud.points[j].transpose()<<"//"<< p.transpose() << std::endl;
      if(p.x() < minx) minx = p.x();
      if(p.x() > maxx) maxx = p.x();
      if(p.y() < miny) miny = p.y();
      if(p.y() > maxy) maxy = p.y();
      accumulated.emplace_back(p);
    }
  }

  int cols = (maxx - minx)/0.5+10;
  int rows = (maxy - miny)/0.5+10;

  int minz =255;
  cv::Mat mat;
  //turn 3d points into pixels, find lowest z of every pixel
  mat = cv::Mat(rows, cols, CV_64FC1, cv::Scalar(255,255,255));
  for(int i = 0; i < accumulated.size(); i++){
    int vx = (accumulated[i].x() - minx) / 0.5 ;
    int vy = (accumulated[i].y() - miny) / 0.5 ;
    //std::cout<< accumulated[i].z()<<std::endl;
    if(accumulated[i].z() < mat.at<double>(vy,vx)) {
      mat.at<double>(vy,vx) = accumulated[i].z();
      if(accumulated[i].z()<minz && accumulated[i].z()>-30) minz = accumulated[i].z();
    }
  }

  //find ground points
  std::vector<cv::Point3d> gnd;
  double threshold = 3;
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols ; j++){
      cv::Point3d p2;
      p2 = cv::Point3d(i,j,mat.at<double>(i,j));
      if(mat.at<double>(i,j) < minz + threshold) gnd.emplace_back(p2);
      //if(mat.at<double>(i,j)!= 255) std::cout<< p2 << std::endl;
    }
  }
  std::cout<<minz<<std::endl;

  //project 3d points to 2d image
  cv::Mat image;
  std::vector<cv::Point2d> pt;
  cv::projectPoints(gnd, cv::Vec3d(0,0,0),cv::Vec3d(0,0,0),cv::Matx33f(30,0,700,0,30,1000,0,0,1),cv::Vec4d(0,0,0,0),pt);

  image = cv::Mat(1000,1000,CV_64FC3,cv::Scalar(0,0,0));
  for(int i = 0 ; i < pt.size(); i++){
    int u = (int)pt[i].x;
    int v = (int)pt[i].y;
    //std::cout<<u<< " "<< v<<std::endl;
    image.at<cv::Vec3d>(u,v) = cv::Vec3d(0,0,255.0);
    //image.at<double>
  }

  imshow("ground",image);
  cv::waitKey(0);
  return 0;
}
