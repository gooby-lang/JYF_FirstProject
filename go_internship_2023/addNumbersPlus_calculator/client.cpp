#include <iostream>
#include <memory>
#include <cstring>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "addNumbersPlus_calculator/adderPlusUltra.grpc.pb.h"
#else
#include "adderPlusUltra.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using adderPlusUltra::AddTwoNumbersBatch;
using adderPlusUltra::AddTwoNumbersBatchRequest;
using adderPlusUltra::AddTwoNumbersBatchResponse;

class AddTwoNumbersBatchClient {
public:
    AddTwoNumbersBatchClient(std::shared_ptr<Channel> channel)
            : stub_(AddTwoNumbersBatch::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    google::protobuf::RepeatedField<int> AddTwoNumbersBatch(std::string &s) {

        // Data we are sending to the server.
        AddTwoNumbersBatchRequest request;
        request.set_s(s);
        // Container for the data we expect from the server.
        AddTwoNumbersBatchResponse reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->AddTwoNumbersBatch(&context, request, &reply);
        google::protobuf::RepeatedField<int> g;
        // Act upon its status.
        if (status.ok()) {
            return reply.res();
        } else {
            std::cout << status.error_code() << ":=== " << status.error_message()
                      << std::endl;
            return g;
        }
    }

private:
    std::unique_ptr<AddTwoNumbersBatch::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string target_str;
    target_str = "localhost:50051";
    AddTwoNumbersBatchClient adder(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    auto t = argv[1];
    std::string s;
    for (int i = 0; t[i]; i ++ ) s.push_back(t[i]);
//    std::cout << s << std::endl;
    auto ans = adder.AddTwoNumbersBatch(s);
    for (auto ite : ans) std::cout << ite << ' ';
    std::cout << std::endl;
    return 0;
}