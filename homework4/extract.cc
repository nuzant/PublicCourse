//extract.cc

#include <unordered_map>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <QtWidgets/QApplication>

#include "homework4/pointcloud_viewer.h"

DEFINE_string(pony_data_dir, "", "The path of pony data.");

int main(){
    interface::perception::PerceptionObstacles pos;
    interface::perception::PerceptionObstacle po;
    
    PointCloudViewer::Options options;
    PointCloudViewer viewer(options, nullptr, FLAGS_pony_data_dir);

}

