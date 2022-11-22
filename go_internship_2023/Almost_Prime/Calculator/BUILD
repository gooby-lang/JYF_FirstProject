package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")


proto_library(
    name = "calculator_proto",
    srcs = ["calculator.proto"],
)

cc_proto_library(
    name = "calculator_cc_proto",
    deps = [":calculator_proto"],
)

cc_grpc_library(
    name = "calculator_cc_grpc",
    srcs = [":calculator_proto"],
    grpc_only = True,
    deps = [":calculator_cc_proto"],
)


cc_binary(
    name = "calculator_server",
    srcs = ["server.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":calculator_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)


cc_binary(
    name = "calculator_client",
    srcs = ["client.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":calculator_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
