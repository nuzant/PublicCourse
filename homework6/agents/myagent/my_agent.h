//my_agent.h

#pragma once

#include "homework6/simulation/vehicle_agent.h"

#include "common/proto/agent_status.pb.h"
#include "common/proto/geometry.pb.h"
#include "common/proto/vehicle_status.pb.h"
#include "common/proto/route.pb.h"
#include "common/utils/math/math_utils.h"
#include "homework6/simulation/vehicle_agent_factory.h"
#include "homework6/route/route.h"
#include <cmath>
#include <fstream>
namespace sample {

class MyVehicleAgent : public simulation::VehicleAgent {
 public:
  explicit MyVehicleAgent(const std::string& name) : VehicleAgent(name) {}

  virtual void Initialize(const interface::agent::AgentStatus& agent_status) {
    //initialize routing 
    interface::geometry::Point3D start_point;
    interface::geometry::Point3D end_point;
    start_point.set_x(agent_status.vehicle_status().position().x());
    start_point.set_y(agent_status.vehicle_status().position().y());
    start_point.set_z(agent_status.vehicle_status().position().z());
    end_point.CopyFrom(agent_status.route_status().destination());
    findroute::FindRoute find_route(start_point,end_point); 
    route = find_route.return_route();

    current = route.route_point(0);
    next = route.route_point(1);
  }

  virtual interface::control::ControlCommand RunOneIteration(
      const interface::agent::AgentStatus& agent_status) {
    interface::control::ControlCommand command;
    // Vehicle's current position reaches the destination
    if (CalcDistance(agent_status.vehicle_status().position(),
                     agent_status.route_status().destination()) < 3.0) {
      position_reached_destination_ = true;
    }
    // Vehicle's current velocity reaches 5 m/s
    if (CalcVelocity(agent_status.vehicle_status().velocity()) > 5) {
      velocity_reached_threshold_ = true;
    }
    double angle = 0;
    //print v
    /*double simu_time = agent_status.simulation_status().simulation_time();
    std::ofstream fout("/home/nuz/workspace/PublicCourse/homework6/agents/myagent/v.txt");
    if(simu_time > clk){
      fout << clk << ":" << CalcVelocity(agent_status.vehicle_status().velocity()) << std::endl;
      clk += 0.1;
    }*/
    //
    double dx = next.x()-current.x();
    double dy = next.y()-current.y();
    double dvx = agent_status.vehicle_status().velocity().x();
    double dvy = agent_status.vehicle_status().velocity().y();
    if(CalcVelocity(agent_status.vehicle_status().velocity())<0.5){
      dvx = dx;
      dvy = dy;
    }
    angle = std::atan2(dy,dx) - std::atan2(dvy,dvx);
    if(angle > 6 || angle < -6) angle = 0;
    double c = 50.0;
    //std::cout<<index<<":"<<angle<<" dx:"<< dx << " dy:" << dy << " dvx:"<< dvx << " dvy:"<< dvy <<std::endl;
    command.set_steering_angle(c*angle);

    if(CalcDistance2(agent_status.vehicle_status().position(),next) < 1.0){
      //std::cout<<index<<std::endl;
      index ++;
      current = next;
      if(index <= route.route_point_size()-1){
        next = route.route_point(index+1);
      }
    }
    
    if (position_reached_destination_) {
      // Set maximum brake ratio to stop the vehicle
      command.set_brake_ratio(0.5);
    } else {
      if (!velocity_reached_threshold_) {
        // Set throttle ratio to accelerate
        command.set_throttle_ratio(0.3);
      }
    }
    return command;
  }

 private:
  double CalcDistance(const interface::geometry::Vector3d& position,
                      const interface::geometry::Point3D& destination) {
    double sqr_sum =
        math::Sqr(position.x() - destination.x()) + math::Sqr(position.y() - destination.y());
    ;
    return std::sqrt(sqr_sum);
  }

  double CalcDistance2(const interface::geometry::Vector3d& position,
                      const interface::geometry::Point2D& destination) {
    double sqr_sum =
        math::Sqr(position.x() - destination.x()) + math::Sqr(position.y() - destination.y());
    ;
    return std::sqrt(sqr_sum);
  }

  double CalcVelocity(const interface::geometry::Vector3d& velocity) {
    double sqr_sum = math::Sqr(velocity.x()) + math::Sqr(velocity.y());
    ;
    return std::sqrt(sqr_sum);
  }
  // current route point and next route point
  interface::geometry::Point2D current;
  interface::geometry::Point2D next;
  int index = 0;
  // map data and route data
  interface::route::Route route;
  // Whether vehicle's current position reaches the destination
  bool position_reached_destination_ = false;
  // Whether vehicle's current velocity reaches 5 m/s
  bool velocity_reached_threshold_ = false;
  //map lib 
  homework6::map::MapLib map_lib_;

  //time clock;
  double clk = 0.0;
};

}  // namespace sample
