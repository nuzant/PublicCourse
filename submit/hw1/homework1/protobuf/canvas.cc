// Copyright @2018 Pony AI Inc. All rights reserved.

#include "homework1/protobuf/canvas.h"

#include <cmath>
#include <iostream>
#include <glog/logging.h>

namespace homework1 {

using homework1::geometry::Point3D;

void Canvas::Draw() const {
  for (const auto& p : polygon_.point()) {
    std::cout << "Point:" << p.x() << ", " << p.y() << ", " << p.z() << std::endl;
  }
}

void Canvas::AddPoint(double x, double y, double z) {
  Point3D point;
  point.set_x(x);
  point.set_y(y);
  point.set_z(z);
  AddPoint(point);
}

void Canvas::AddPoint(const Point3D& p) {
  auto* point = polygon_.add_point();
  point->CopyFrom(p);
}

const Point3D& Canvas::GetPoint(int index) const {
  return polygon_.point(index);
}

const double Canvas::ComputeLength(const Point3D& p1, const Point3D& p2){
  double dx = p1.x() - p2.x();
  double dy = p1.y() - p2.y();
  double dz = p1.z() - p2.z();
  return sqrt(dx*dx+dy*dy+dz*dz);
}

const double Canvas::ComputePolyline(){
  int n = polygon_.point_size();
  double result = 0;
  if(n > 1){
    for(int i=0;i<n-1;i++){
      result += ComputeLength(polygon_.point(i),polygon_.point(i+1));
    }
    return result;
  } else {
    return result;
  }
}

void Canvas::ParseFromString(const std::string& serialzation) {
  polygon_.ParseFromString(serialzation);
}

const std::string Canvas::SerializeToString() const {
  std::string serialzation;
  CHECK(polygon_.SerializeToString(&serialzation)) << "Canvas serialization failed.";
  return serialzation;
}

}  // namespace homework1
