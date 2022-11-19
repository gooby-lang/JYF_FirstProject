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
        std::string s = request->s();
        //(1,2),(1,3)
        AddTwoNumbersBatchResponse ans;
        int number1 = 0, number2 = 0, tmp = 0;
        bool nowIsFirst = false;
        for (int i = 0; i < s.size(); i ++ ) {
            if (s[i] == '(') {
                nowIsFirst = true;
            } else if (s[i] == ',') {
                if (nowIsFirst) {
                    tmp = number1;
                    nowIsFirst = false;
                } else {
                    continue;
                }
            } else if (s[i] == ')') {
                tmp += number2;
                reply->add_res(tmp);
                number1 = number2 = tmp = 0;
            } else {
                if (nowIsFirst) number1 = number1 * 10 + s[i] - '0';
                else number2 = number2 * 10 + s[i] - '0';
            }
        }
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    AddTwoNumbersBatchServiceImpl service;

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