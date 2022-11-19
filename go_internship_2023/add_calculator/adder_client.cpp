#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "add_calculator/adder.grpc.pb.h"
#else
#include "adder.grpc.pb.h"
#endif

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

int main(int argc, char** argv) {
	// Instantiate the client. It requires a channel, out of which the actual RPCs
	// are created. This channel models a connection to an endpoint specified by
	// the argument "--target=" which is the only expected argument.
	// We indicate that the channel isn't authenticated (use of
	// InsecureChannelCredentials()).
	std::string target_str;
	target_str = "localhost:50051";
	AdderClient adder(
			grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	auto res = adder.AddTwoNumbers(a, b);
	std::cout << a << " + " << b << " = " << res << std::endl;
	return 0;
}