//route.h - finding route of current simulation

#include "common/utils/file/file.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "homework6/map/map_lib.h"
#include "common/proto/route.pb.h"
#include <string>
#include <vector>

namespace findroute{

class FindRoute {
public:
FindRoute(interface::geometry::Point3D start, interface::geometry::Point3D end){
    interface::map::Map map = map_lib_.map_proto();
    //map_process
    map_process(map);
    //find_route 
    //std::cout<<map.lane_size()<<std::endl;
    int index_sl,index_sp,index_el,index_ep;
    double mindist = 999;
    //find first route point
    for(int j = 0 ; j < map.lane_size(); j++){
        for(int k = 0; k < map.lane(j).central_line().point_size(); k++){
            interface::geometry::Point3D point = map.lane(j).central_line().point(k);
            if(dist(start,point)<mindist) {
                //std::cout<<dist(start,point)<<std::endl;
                //std::cout<<start.x()<<" "<<start.y() <<std::endl;
                //std::cout<<point.x()<<" "<<point.y() <<std::endl;
                index_sl = j;
                index_sp = k;
                mindist = dist(start,point);
            }
        }
    }
    //std::cout<<"111"<<std::endl;
    //find last route point
    for(int j = 0 ; j < map.lane_size(); j++){
        for(int k = 0; k < map.lane(j).central_line().point_size(); k++){
            interface::geometry::Point3D point = map.lane(j).central_line().point(k);
            if(dist(end,point)<4) {
                index_el = j;
                index_ep = k;
            }
        }
    }
    //std::cout<<"111"<<std::endl;
    //find a route from startline to endline
    std::vector<interface::map::Lane> route_lanes;
    std::vector<interface::map::Id> route_ids;
    std::vector<int> father;
    std::string endId = map.lane(index_el).id().id();
    int father_index = 0;
    father.clear();
    father.push_back(father_index);
    route_ids.clear();
    route_ids.push_back(map.lane(index_sl).id());
    //std::cout<<"vsize"<<route_ids.size()<<std::endl;
    while(true){
        //std::cout<<"fa"<<father_index<<std::endl;
        bool done = false;
        interface::map::Lane cur = obtain_lane(route_ids[father_index].id(),map);
        //std::cout<<cur.id().id()<<std::endl;
        //std::cout<<cur.successor_size()<<std::endl;
        for(int j = 0; j< cur.successor_size();j++){
            route_ids.push_back(cur.successor(j));
            father.push_back(father_index);
            if(cur.successor(j).id() == endId) {
                done = true;
                break;
            }
        }
        if(done) break;
        father_index++;
        //std::cout<<father_index<<std::endl;
        //std::cout<<"vsize"<<route_ids.size()<<std::endl;
    }
    //std::cout<<"333"<<std::endl;
    route_lanes.emplace_back(map.lane(index_el));
    while(father_index!=0){
        route_lanes.emplace_back(obtain_lane(route_ids[father_index].id(),map));
        father_index = father[father_index];
    }
    //std::cout<<"sz"<<route_lanes.size()<<std::endl;
    for(int j = 0 ; j < route_lanes.size();j++){
        //std::cout<<map.lane(index_sl).id().id()<<std::endl;
        if(j==0){
            //std::cout<<j<<std::endl;
            for(int k = index_sp ; k < map.lane(index_sl).central_line().point_size();k++){
                auto *point = route.add_route_point();
                point -> CopyFrom(Point3D_to_2D(map.lane(index_sl).central_line().point(k)));
            }
            //std::cout<<index_sp<<" " << map.lane(index_sl).central_line().point_size()<< std::endl;
        }else if(j == route_lanes.size()-1){
            //std::cout<<j<<std::endl;
            for(int k = 0; k <= index_ep ; k++){
                auto *point = route.add_route_point();
                point -> CopyFrom(Point3D_to_2D(map.lane(index_el).central_line().point(k)));
            }
            break;
        }
        //std::cout<<j<<std::endl;
        for(int k = 0; k < route_lanes[route_lanes.size()-1-j].central_line().point_size();k++){
            auto *point = route.add_route_point();
            point -> CopyFrom(Point3D_to_2D(
                route_lanes[route_lanes.size()-1-j].central_line().point(k)));
        }
    }
}

interface::route::Route return_route(){
    return route;
}


private:
interface::route::Route route;
homework6::map::MapLib map_lib_;

double dist(interface::geometry::Point3D p1, interface::geometry::Point3D p2){
    double dx = p1.x()-p2.x();
    double dy = p1.y()-p2.y();
    return sqrt(dx*dx+dy*dy);
}

interface::geometry::Point3D Point2D_to_3D(interface::geometry::Point2D p){
    interface::geometry::Point3D p3d;
    p3d.set_x(p.x());
    p3d.set_y(p.y());
    p3d.set_z(0);
    return p3d;
}

interface::geometry::Point2D Point3D_to_2D(interface::geometry::Point3D p){
    interface::geometry::Point2D p2d;
    p2d.set_x(p.x());
    p2d.set_y(p.y());
    return p2d;
}

interface::map::Lane obtain_lane(std::string id,interface::map::Map map){
    interface::map::Lane l;
    for(int i = 0 ; i < map.lane_size();i++){
        if(map.lane(i).id().id() == id){
            l = map.lane(i);
            return l;
        }
    }
    std::cout<<"cant find lane "<<id<<std::endl;
    return l;
}

void map_process(interface::map::Map &map_data){
    for(int i = 0; i < map_data.lane_size(); i++){
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
    return;
}

};

}