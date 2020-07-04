#include <string>
#include <zmq.hpp>
#include <iostream>
#include "FbFile.h"
#include "messages.h"
/*
 * tmBaixhAva
 */

messages::hello fromFrameBuffer(const FbFile& fbFile) {
    messages::hello h {};
    auto var_screen_info = fbFile.varScreenInfo();

    h.xres = var_screen_info.xres;
    h.yres = var_screen_info.yres;
    h.xres_virtual = var_screen_info.xres_virtual;
    h.yres_virtual = var_screen_info.yres_virtual;
    h.bits_per_pixel = var_screen_info.bits_per_pixel;
    h.grayscale = var_screen_info.grayscale;

    auto fix_screen_info = fbFile.fixScreenInfo();
    h.line_length = fix_screen_info.line_length;
    h.smem_len = fix_screen_info.smem_len;
    return h;
}

int main() {
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::rep);
    sock.bind("tcp://*:5555");

    auto fbFile = FbFile("/dev/fb0");

    auto errTupple = fbFile.getError();
    auto err = std::get<FbFile::FbError>(errTupple);
    if (err != FbFile::NO_ERROR) {
        std::cout << "Error: (" << err << ") " << std::strerror(std::get<int>(errTupple)) << std::endl;
        if (err == FbFile::MMAP) {
            std::cout << "mmap failed for length: " << fbFile.screenBytesSize() << std::endl;
        }
        return -1;
    }

    bool serve = true;
    while (serve) {
        zmq::message_t req;
        auto read = sock.recv(req);

        if (read > 0) {
            auto req_string = req.to_string();

            if (req_string == "hello") {
                zmq::message_t resp(sizeof(messages::hello));
                auto hello = fromFrameBuffer(fbFile);

                memcpy(resp.data(), &hello, sizeof(messages::hello));
                sock.send(resp, zmq::send_flags::dontwait);
            }

            if (req_string == "refresh") {
                fbFile.refresh();

                zmq::message_t resp(fbFile.screenBytesSize());
                memcpy(resp.data(), fbFile.screen(), fbFile.screenBytesSize());
                sock.send(resp, zmq::send_flags::dontwait);
            }
        }
    }
}
