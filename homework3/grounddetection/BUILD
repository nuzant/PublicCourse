package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pointcloud",
    srcs = ["pointcloud.cc"],
    hdrs = ["pointcloud.h"],
    deps = [
        "@eigen//:eigen",
        "@glog//:glog",
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgproc",
        "@opencv//:calib3d",
        "@opencv//:features2d",
        "@opencv//:flann",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":pointcloud",
    ],
)