//perception_obstacles.cc

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "common/proto/perception.pb.h"
#include "common/proto/object_labeling_3d.pb.h"
#include "common/proto/perception.pb.h"

#include "common/utils/file/file.h"
#include "common/utils/file/path.h"

#include "homework2/pointcloud.h"

#include <iostream>

const std::string prefix_output_dir = "/home/nuz/workspace/PublicCourse/homework4/output/";
const std::string prefix_label_dir = "/home/nuz/workspace/PublicCourse/homework4/sample/label/VelodyneDevice32c/";
const std::string prefix_pointcloud_dir = "/home/nuz/workspace/PublicCourse/homework4/sample/select/VelodyneDevice32c/";

double cross(const Eigen::Vector3d &a, const Eigen::Vector3d &b){
    return a.x()*b.y()-b.x()*a.y();
}

interface::geometry::Point3D eigen_to_proto(Eigen::Vector3d &p){
    interface::geometry::Point3D p3d;
    p3d.set_x(p.x());
    p3d.set_y(p.y());
    p3d.set_z(p.z());
    return p3d;
}

Eigen::Vector3d proto_to_eigen(const interface::geometry::Point3D &p) {
    return Eigen::Vector3d(p.x(), p.y(), p.z());
}

bool point_in_obstacle(const Eigen::Vector3d &p, const interface::geometry::Polygon &polygon
    , double height){
    CHECK(polygon.point_size() >= 3);

    double minz = 100000;
    double maxz = 0;

    auto first = proto_to_eigen(polygon.point(0));
    auto last = first;
    
    for (int i = 1; i < polygon.point_size(); i++) {
        auto it = proto_to_eigen(polygon.point(i));
        if (cross(last - p, it - p) < 0) return false;
        //cout << v << endl;
        last = it;

        if(it.z()<minz) minz = it.z();
    }
    if (cross(last - p, first - p) < 0) return false;

    maxz = minz + height;
    return p.z() >= minz && p.z() <= maxz;
}

int main(){
    for(int i = 0 ; i < 21 ; i++){
        interface::object_labeling::ObjectLabels object_labels;
        CHECK(file::ReadFileToProto(prefix_label_dir + std::to_string(i)+ ".label",&object_labels));
        PointCloud pointcloud = ReadPointCloudFromTextFile(prefix_pointcloud_dir + std::to_string(i)+ ".txt");
        interface::perception::PerceptionObstacles perception_obstacles;
        for(int j = 0 ; j < object_labels.object_size(); j ++){
            interface::perception::PerceptionObstacle o;
            interface::object_labeling::ObjectLabel label = object_labels.object(j);
            o.set_id(label.id());
            o.set_heading(label.heading());
            o.set_height(label.height());
            o.set_type(label.type());

            auto &poly = label.polygon();
            for(int k = 0 ; k < poly.point_size();k++){
                auto *p = o.add_polygon_point();
                p -> CopyFrom(poly.point(k));
            }

            for(auto eigenp: pointcloud.points){
                eigenp = pointcloud.rotation * eigenp + pointcloud.translation;
                auto p = eigen_to_proto(eigenp);
                if(point_in_obstacle(eigenp,poly,label.height())){
                    auto *obj_point = o.add_object_points();
                    obj_point -> CopyFrom(p);
                }
            }
            //std::cout<<o.object_points_size()<<std::endl;
            auto *obstacle = perception_obstacles.add_obstacle();
            obstacle->CopyFrom(o);
        }
        CHECK(file::WriteProtoToFile(perception_obstacles,prefix_output_dir+std::to_string(i)+".perception"));
    }
}