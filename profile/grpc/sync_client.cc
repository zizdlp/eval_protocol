/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <chrono>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

ABSL_FLAG(std::string, target, "127.0.0.1", "Server address");
ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");
ABSL_FLAG(uint32_t, loop, 1000, "client call loop times");
ABSL_FLAG(uint32_t, length, 25000, "send data bytes each time");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    // auto s= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
    // .count();
    Status status = stub_->SayHello(&context, request, &reply);
    // auto e= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
    // .count();
    // std::cout<<"in call time consume:"<<e-s<<std::endl;
    // std::cout<<"in call s:"<<s<<std::endl;
    // std::cout<<"in call e:"<<e<<std::endl;
    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
	std::string target = absl::GetFlag(FLAGS_target);
	uint16_t port = absl::GetFlag(FLAGS_port);
  int length = absl::GetFlag(FLAGS_length);
  int loop = absl::GetFlag(FLAGS_loop);
  std::string server_address = absl::StrFormat("%s:%d",target, port);
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  GreeterClient greeter(
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
  
  std::string send_data(length, 'a');
  std::string user(send_data);
  auto s= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
  .count();
  for(int i=0;i<loop;++i){
    std::string reply = greeter.SayHello(user);
  }
  auto e= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
  .count();
  std::cout << "=== grpc sync;"<<"port:"<<port<<";loop:"<<loop<<";length:"<<length<<";time:" << e - s << "us ===" << std::endl;
  return 0;
}
