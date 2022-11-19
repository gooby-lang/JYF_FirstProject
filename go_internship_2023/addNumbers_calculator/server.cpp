#include <iostream>
#include <memory>
#include <cstring>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "addNumbers_calculator/adderPlus.grpc.pb.h"
#else
#include "adderPlus.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using adderPlus::AddNumbers;
using adderPlus::AddNumbersRequest;
using adderPlus::AddNumbersResponse;

// Logic and data behind the server's behavior.
class AddNumbersServiceImpl final : public AddNumbers::Service {
    Status AddNumbers(ServerContext* context, const AddNumbersRequest* request,
                         AddNumbersResponse* reply) override {
        std::string s = request->s();
        int now = 0, sum = 0;
        for (int i = 0; i < s.size(); i ++ ) {
            if (s[i] == ',') {
                sum += now;
                now = 0;
            } else {
                now = now * 10 + s[i] - '0';
            }
        }
        sum += now;
        reply->set_res(sum);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    AddNumbersServiceImpl service;

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