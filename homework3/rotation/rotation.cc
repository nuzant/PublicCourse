// Copyright @2018 Pony AI Inc. All rights reserved.

#include "homework3/rotation/rotation.h"

namespace homework3 {

Eigen::Vector3d ToRollPitchYaw(Eigen::Matrix3d rotation) {
  double alpha, beta, gamma, r[3][3];
  for (int i = 0 ; i < 3 ; i++){
    for (int j = 0 ; j < 3 ; j++){
      r[i][j] = rotation(i,j);
    }
  }

  alpha = atan2(r[1][0],r[0][0]);
  beta = atan2(-r[2][0],sqrt(r[2][1]*r[2][1]+r[2][2]*r[2][2]));
  gamma = atan2(r[2][1],r[2][2]);

  return Eigen::Vector3d(alpha, beta, gamma);
}

Eigen::AngleAxisd ToAngleAxis(Eigen::Matrix3d rotation) {
  Eigen::Vector3d axis;
  double x,y,z,denom,angle,r[3][3];
  for (int i = 0 ; i < 3 ; i++){
    for (int j = 0 ; j < 3 ; j++){
      r[i][j] = rotation(i,j);
    }
  }

  denom = sqrt((r[1][2]-r[2][1])*(r[1][2]-r[2][1])+(r[0][2]-r[2][0])*(r[0][2]-r[2][0])+(r[1][0]-r[0][1])*(r[1][0]-r[0][1]));

  angle = acos((r[0][0]+r[1][1]+r[2][2]-1)/2);
  if(angle!=0 && denom!=0){
    x = (r[2][1]-r[1][2])/denom;
    y = (r[0][2]-r[2][0])/denom;
    z = (r[1][0]-r[0][1])/denom;
  } else {
    x = 0.0;
    y = 0.0;
    z = 0.0;
  }

  axis << x,y,z;

  return Eigen::AngleAxisd(angle, axis);
}
}  // namespace homework3
