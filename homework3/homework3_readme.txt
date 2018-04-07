homework3
task1 /homework3/rotation
task2 /homework3/chessboard
task3 /homework3/grounddetection
task3 data directory /homework3/grounddetection/pony_data (not posted)

file modified
/utils/bazel/opencv.BUILD
cc_library(
    name = "flann",
    srcs = ["lib/libopencv_flann.so"],
    hdrs = glob([
        "include/opencv2/flann/*.h",
        "include/opencv2/flann/*.hpp",
    ]),
    includes = ["include"],
    linkstatic = 1,
)