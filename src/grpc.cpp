#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "suprema-gw.grpc.pb.h"
#include "suprema-gw.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using gateway::SupremaGateway;
using gateway::DebugReply;
using gateway::DebugRequest;

// ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class SupremaGatewayImpl final : public SupremaGateway::Service {
    Status DebugCommand(ServerContext* context, const DebugRequest* request,
                    DebugReply* reply) override {
        std::string prefix("Hello ");
        std::cout << "Got request with name = " << request->cmd() << std::endl;
        reply->set_answer(prefix + request->cmd());
        return Status::OK;
    }
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
    SupremaGatewayImpl service;

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

// void grpc_init (int argc, char** argv) {
//     // absl::ParseCommandLine(argc, argv);
// }

int grpc_run () {
    // RunServer(absl::GetFlag(FLAGS_port));
    RunServer(50051);
    return 0;
}
