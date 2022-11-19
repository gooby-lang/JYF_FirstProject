package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")


proto_library(
    name = "adderPlus_proto",
    srcs = ["adderPlus.proto"],
)

cc_proto_library(
    name = "adderPlus_cc_proto",
    deps = [":adderPlus_proto"],
)

cc_grpc_library(
    name = "adderPlus_cc_grpc",
    srcs = [":adderPlus_proto"],
    grpc_only = True,
    deps = [":adderPlus_cc_proto"],
)


cc_binary(
    name = "adderPlus_server",
    srcs = ["server.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":adderPlus_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)


cc_binary(
    name = "adderPlus_client",
    srcs = ["client.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":adderPlus_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
