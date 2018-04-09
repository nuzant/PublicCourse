// Copyright @2018 Pony AI Inc. All rights reserved.

#include <iostream>
#include <cmath>
#include <fstream>

#include "homework2/pointcloud.h"

int main(){
    std::ofstream fout("/home/nuzant/PublicCourse/homework2/histogram/plotdata.txt");

    const PointCloud pointcloud = ReadPointCloudFromTextFile(
        "/home/nuz/workspace/PublicCourse/homework2/sample_data/VelodyneDevice32c/0.txt");
    std::vector<double> range;
    std::vector<double> height;

    fout << pointcloud.points.size() << std::endl;
  
    range.resize(pointcloud.points.size());
    height.resize(pointcloud.points.size());
    for(int i = 0; i<pointcloud.points.size(); i++){
        range[i] = sqrt(pointcloud.points[i].x() * pointcloud.points[i].x() +
                        pointcloud.points[i].y() * pointcloud.points[i].y() +
                        pointcloud.points[i].z() * pointcloud.points[i].z());
        height[i] = pointcloud.points[i].z();
        //fout data
        fout << range[i] << std::endl << height[i] << std::endl;
    }
    std::cout<< "data out" << std::endl;
  
    return 0;
}