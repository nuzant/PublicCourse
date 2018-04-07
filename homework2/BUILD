package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pointcloud",
    srcs = ["pointcloud.cc"],
    hdrs = ["pointcloud.h"],
    deps = [
        "@eigen//:eigen",
        "@glog//:glog",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":pointcloud",
    ],
)

cc_binary(
    name = "histdata",
    srcs = ["histdata.cc"],
    deps = [
        ":pointcloud",
    ],
)

cc_binary(
    name = "convert_to_grey",
    srcs = ["convert_to_grey.cc"],
    deps = [
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgproc",
    ],
)
cc_binary(
    name = "lane",
    srcs = ["lane.cc"],
    deps = [
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgproc",
    ],
)