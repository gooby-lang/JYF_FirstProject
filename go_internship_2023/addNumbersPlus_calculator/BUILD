package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")


proto_library(
    name = "adderPlusUltra_proto",
    srcs = ["adderPlusUltra.proto"],
)

cc_proto_library(
    name = "adderPlusUltra_cc_proto",
    deps = [":adderPlusUltra_proto"],
)

cc_grpc_library(
    name = "adderPlusUltra_cc_grpc",
    srcs = [":adderPlusUltra_proto"],
    grpc_only = True,
    deps = [":adderPlusUltra_cc_proto"],
)


cc_binary(
    name = "adderPlusUltra_server",
    srcs = ["server.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":adderPlusUltra_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)


cc_binary(
    name = "adderPlusUltra_client",
    srcs = ["client.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":adderPlusUltra_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
