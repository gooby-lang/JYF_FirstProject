#include <iostream>
#include <memory>
#include <cstring>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "addNumbersPlus_calculator/adderPlusUltra.grpc.pb.h"
#else
#include "adderPlusUltra.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using adderPlusUltra::AddTwoNumbersBatch;
using adderPlusUltra::AddTwoNumbersBatchRequest;
using adderPlusUltra::AddTwoNumbersBatchResponse;

// Logic and data behind the server's behavior.
class AddTwoNumbersBatchServiceImpl final : public AddTwoNumbersBatch::Service {
    Status AddTwoNumbersBatch(ServerContext* context, const AddTwoNumbersBatchRequest* request,
                              AddTwoNumbersBatchResponse* reply) override {
        std::string s[] = request->s();
        int now = 0, sum = 0, lastNum = 0;
        bool is_first = true;
        string ans = "(";
        for (int i = 0; i < s.size(); i ++ ) {
            if (s[i] >= '0' && s[i] <= '9') {
                now = now * 10 + s[i] - '0';
            } else {
                if (s[i] == ',' && is_first) {
                    lastNum = now;
                    now = 0;
                    is_first = false;
                } else {

                }
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