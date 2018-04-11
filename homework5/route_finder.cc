//route_finder.cc

#include "common/utils/file/file.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "homework5/map/map_lib.h"
#include "common/proto/route.pb.h"
#include <string>
#include <vector>
#include <unordered_map>

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

int FILENUM = 6;

int main(){
    //std::cout<<"111"<<std::endl;
    interface::map::Map map;
    //std::unordered_map<std::string,interface::map::Lane> lane_id = obtain_lane_id_map(map);
    CHECK(file::ReadFileToProto("/home/nuz/workspace/PublicCourse/homework5/map/processed_map_proto.txt"
        , &map)) << "Failed to load map";
    for(int i = 1 ; i < FILENUM; i++){
        //std::cout<<"111 "<<i<<std::endl;
        interface::route::Route route;
        CHECK(file::ReadFileToProto("/home/nuz/workspace/PublicCourse/homework5/data/routes/route_request_"
            + std::to_string(i)+ ".txt",&route)) 
            << "Failed to load route request.";
        interface::geometry::Point3D start = Point2D_to_3D(route.start_point());
        interface::geometry::Point3D end = Point2D_to_3D(route.end_point());
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
            interface::map::Lane cur = obtain_lane(route_ids[father_index].id(),map);
            bool done = false;
            for(int j = 0; j< cur.successor_size();j++){
                //std::cout<<"nansi"<<std::endl;
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
        //std::cout<<"111"<<std::endl;
        //std::cout<<route.start_point().x()<<" " << route.end_point().x() <<std::endl;
        CHECK(file::WriteProtoToFile(route,
            "/home/nuz/workspace/PublicCourse/homework5/data/routes/route_"+std::to_string(i)+".txt"))
            << "write file failed";
    }
    return 0;
}