#include <fstream>
#include <iostream>
#include <string>
#include <zmq.hpp>
#include "messages.h"
int main()
{
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::req);
    sock.connect("tcp://192.168.1.7:5555");

    {
        zmq::message_t req(5);
        memcpy(req.data(), "hello", 5);
        sock.send(req, zmq::send_flags::none);

        zmq::message_t resp;
        auto read = sock.recv(resp);
        if (read > 0) {
            auto* hello = resp.data<messages::hello>();
            std::cout << "Hello: "
                      << hello->xres << ", "
                      << hello->yres << ", "
                      << hello->xres_virtual << ", "
                      << hello->yres_virtual << ", "
                      << hello->grayscale << ", "
                      << hello->bits_per_pixel << ", "
                      << hello->line_length << ", "
                      << hello->smem_len << ", "
                      << "\n";
        }
    }

    {
        zmq::message_t req(7);
        memcpy(req.data(), "refresh", 7);
        sock.send(req, zmq::send_flags::none);

        zmq::message_t resp;
        auto rread = sock.recv(resp);

        if (rread > 0) {
            auto* data = resp.data<char>();
            std::ofstream myfile("example.hex", std::ios::out | std::ios::binary);
            myfile.write(data, resp.size());
            myfile.close();
            std::cout << "wrote to file: " << rread.value() << resp.str() << std::endl;
        }
    }
    return 0;
}
