#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "Almost_Prime/Calculator/calculator.grpc.pb.h"
#else
#include "calculor.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using solution::Calculator;
using solution::Input;
using solution::Output;

class CalculatorClient {
public:
    CalculatorClient(std::shared_ptr<Channel> channel)
            : stub_(Calculator::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int CalculatorTheNumberOfPrimes(int n) {

        // Data we are sending to the server.
        Input request;
        request.set_n(n);
        // Container for the data we expect from the server.
        Output reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->CalculatorTheNumberOfPrimes(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.answer();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return -1;
        }
    }

private:
    std::unique_ptr<Calculator::Stub> stub_;
};

int main(int argc, char** argv) {
    Server svr;
    std::string target_str = "localhost:50051";
    CalculatorClient calculator(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    int n = atoi(argv[1]);
    auto res = calculator.CalculatorTheNumberOfPrimes(n);
    std::cout << res << std::endl;
    return 0;
}