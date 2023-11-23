#include <zinx/inc/ZPacketProcessor.h>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
using ip::tcp;

class TCPClient {
public:
    TCPClient(io_service& io_service, const std::string& server_address, short port)
        : resolver_(io_service)
        , socket_(io_service)
    {
        resolve(server_address, port);
    }

private:
    void resolve(const std::string& server_address, short port) {
        tcp::resolver::query query(server_address, std::to_string(port));
        resolver_.async_resolve(query,
            [this](boost::system::error_code ec, tcp::resolver::iterator it) {
                if (!ec) {
                    connect(it);
                }
            });
    }

    void connect(tcp::resolver::iterator it) {
        async_connect(socket_, it,
            [this](boost::system::error_code ec, tcp::resolver::iterator) {
                if (!ec) {
                    std::cout << "Connected to server." << std::endl;
                    writeData();
                }
            });
    }

    void writeData() {
        zinx::ZinxPacket pack = zinx::ZinxPacket(1, "v-0.1 test message.\n");
        
        async_write(socket_, boost::asio::buffer(pack.GetAllData()),
            [this, pack](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "send " << length << " bytes, content: "
                        << pack.GetAllData() << std::endl;
                }
            }
        );
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf response_;
};

int main() {
    boost::asio::io_service io_service;
    TCPClient client(io_service, "0.0.0.0", 8080);
    io_service.run();
    return 0;
}