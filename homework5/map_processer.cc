//map_processer.cc

#include "homework5/map/map_lib.h"
#include "common/utils/file/file.h"
#include <cmath>

double dist(interface::geometry::Point3D p1, interface::geometry::Point3D p2){
    double dx = p1.x()-p2.x();
    double dy = p1.y()-p2.y();
    return sqrt(dx*dx+dy*dy);
}

int main(){
    homework5::map::MapLib maplib = homework5::map::MapLib();
    interface::map::Map map_data = maplib.map_proto();

    for(int i = 0 ; i < map_data.lane_size(); i++){
        auto *lane = map_data.mutable_lane(i);
        const double MAX = 1e-5;
        interface::geometry::Point3D start1 = lane->central_line().point(0);
        interface::geometry::Point3D end1 = lane->central_line().point(lane->central_line().point_size()-1); 
        for(auto &other: map_data.lane()){
            //if(lane == other) continue;
            interface::geometry::Point3D start2 = other.central_line().point(0);
            interface::geometry::Point3D end2 = other.central_line().point(other.central_line().point_size()-1);
            if(dist(start1,end2)<MAX){
                auto *pre_id = lane -> add_predecessor();
                pre_id -> CopyFrom(other.id());
            }
            if(dist(start2,end1)<MAX){
                auto *suc_id = lane -> add_successor();
                suc_id -> CopyFrom(other.id());
            }
        }
    }
    CHECK(file::WriteProtoToFile(map_data, "/home/nuz/workspace/PublicCourse/homework5/map/processed_map_proto.txt"));
}