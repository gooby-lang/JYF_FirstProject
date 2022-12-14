#include <grpcpp/grpcpp.h>

#include "httplib.h"
#include "add_calculator/adder.grpc.pb.h"

using namespace httplib;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using adder::Adder;
using adder::AddTwoNumbersRequest;
using adder::AddTwoNumbersReply;

class AdderClient {
public:
    AdderClient(std::shared_ptr<Channel> channel)
            : stub_(Adder::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int AddTwoNumbers(int a, int b) {
        // Data we are sending to the server.
        AddTwoNumbersRequest request;
        request.set_a(a);
        request.set_b(b);
        // Container for the data we expect from the server.
        AddTwoNumbersReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->AddTwoNumbers(&context, request, &reply);

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
    std::unique_ptr<Adder::Stub> stub_;
};

int main(void) {
    Server svr;
    std::string target_str = "localhost:50051";
    AdderClient adder(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    svr.Get("/add", [&](const Request & req, Response &res) {
        auto a = req.get_param_value("a");
        auto b = req.get_param_value("b");
        int ans = adder.AddTwoNumbers(std::stoi(a), std::stoi(b));
        res.set_content("The Answer is" + std::to_string(ans), "text/plain");
    });

    svr.listen("0.0.0.0", 8080);
}