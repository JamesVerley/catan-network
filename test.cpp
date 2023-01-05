//#include "asio/asio/include/asio.hpp"
#include <agrpc/asio_grpc.hpp>
#include <asio.hpp>


helloworld::Greeter::Stub stub{grpc::CreateChannel(host, grpc::InsecureChannelCredentials())};
agrpc::GrpcContext grpc_context;

asio::co_spawn(
    grpc_context,
    [&]() -> asio::awaitable<void>
    {
        using RPC = agrpc::RPC<&helloworld::Greeter::Stub::PrepareAsyncSayHello>;
        grpc::ClientContext client_context;
        helloworld::HelloRequest request;
        request.set_name("world");
        helloworld::HelloReply response;
        status = co_await RPC::request(grpc_context, stub, client_context, request, response, asio::use_awaitable);
        std::cout << status.ok() << " response: " << response.message() << std::endl;
    },
    asio::detached);

grpc_context.run();