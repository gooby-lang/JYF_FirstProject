#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "Almost_Prime/Calculator/calculator.grpc.pb.h"
#else
#include "calculator.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using solution::Calculator;
using solution::Input;
using solution::Output;

// Logic and data behind the server's behavior.
class CalculatorServiceImpl final : public Calculator::Service {
    Status CalculatorTheNumberOfPrimes(ServerContext* context, const Input* request,
                                       Output* reply) override {
        int n = request->n();
        int sum = 0;
        for (int i = 2; i <= n; i ++ ) if (check(i)) sum ++;
        reply->set_answer(sum);
        return Status::OK;
    }
    bool check(int x) {
        int num = 0;
        for (int i = 2; i * i <= x; i ++ ) {
            if (x % i) continue;
            while (x % i == 0) x /= i;
            num ++;
        }
        if (x > 1) num ++;
        return num == 2;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    CalculatorServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}