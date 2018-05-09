package(default_visibility = ["//visibility:public"])

cc_library(
    name = "my_agent",
    hdrs = ["my_agent.h"],
    deps = [
        "//homework6/simulation:vehicle_agent",
        "//homework6/simulation:vehicle_agent_factory",
        "//homework6/route:route",
        "//common/proto:cc_agent_status_proto",
        "//common/proto:cc_control_proto",
        "//common/proto:cc_simulation_proto",
        "//common/proto:cc_route_proto",
        "@glog",
    ],
)
