package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "map_visualizer_main",
    srcs = ["map_visualizer_main.cc"],
    deps = [
        "//common/utils/file",
        "//homework5/display:main_window",
        "//homework5/map:map_lib",
        "@gflags//:gflags",
        "@glog",
    ],
)

cc_binary(
    name = "map_processer",
    srcs = ["map_processer.cc"],
    deps = [
        "//common/utils/file",
        "//homework5/map:map_lib",
        "@gflags//:gflags",
        "@glog",
    ],
)

cc_binary(
    name = "route_finder",
    srcs = ["route_finder.cc"],
    deps = [
        "//common/utils/file",
        "//homework5/map:map_lib",
        "//common/proto:cc_route_proto",
        "@gflags//:gflags",
        "@glog",
    ],
)