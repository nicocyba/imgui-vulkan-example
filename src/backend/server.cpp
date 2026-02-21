
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include "proto/service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using uicontrol::GuiService;
using uicontrol::UpdateResponse;
using uicontrol::ActionRequest;
using uicontrol::ActionResponse;
using uicontrol::Empty;

class GuiServiceImpl final : public GuiService::Service {
    // 1. Continuous Update Logic
    Status GetUpdates(ServerContext* context, const Empty* request, 
                      grpc::ServerWriter<UpdateResponse>* writer) override {
        float mock_progress = 0.0f;
        while (!context->IsCancelled()) {
            UpdateResponse response;
            response.set_status_message("System Running...");
            response.set_progress(mock_progress);
            
            writer->Write(response);
            
            mock_progress += 0.1f;
            if (mock_progress > 1.0f) mock_progress = 0.0f;

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        return Status::OK;
    }

    // 2. Button Click Logic
    Status TriggerAction(ServerContext* context, const ActionRequest* request,
                         ActionResponse* response) override {
        std::cout << "Button clicked with command: " << request->command() << std::endl;
        response->set_success(true);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GuiServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}