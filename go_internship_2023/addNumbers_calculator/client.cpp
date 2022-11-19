#include <iostream>
#include <memory>
#include <cstring>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "addNumbers_calculator/adderPlus.grpc.pb.h"
#else
#include "adderPlus.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using adderPlus::AddNumbers;
using adderPlus::AddNumbersRequest;
using adderPlus::AddNumbersResponse;

class AddNumbersClient {
public:
    AddNumbersClient(std::shared_ptr<Channel> channel)
            : stub_(AddNumbers::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int AddNumbers(std::string &s) {

        // Data we are sending to the server.
        AddNumbersRequest request;
        request.set_s(s);
        // Container for the data we expect from the server.
        AddNumbersResponse reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->AddNumbers(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.res();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return -1;
        }
    }

private:
    std::unique_ptr<AddNumbers::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string target_str;
    target_str = "localhost:50051";
    AddNumbersClient adder(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    auto t = argv[1];
    std::string s;
    for (int i = 0; t[i]; i ++ ) s.push_back(t[i]);
    auto res = adder.AddNumbers(s);
    std::cout << "Sum is " << res << std::endl;
    return 0;
}