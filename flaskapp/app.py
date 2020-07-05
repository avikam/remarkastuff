import io
import ctypes as ct
from flask import Flask, Response
import zmq
from PIL import Image
import numpy as np

app = Flask(__name__)


class HelloResponse(ct.Structure):
    _fields_ = [
        ("xres", ct.c_uint),
        ("yres", ct.c_uint),
        ("xres_virtual", ct.c_uint),
        ("yres_virtual", ct.c_uint),
        ("xoffset", ct.c_uint),
        ("yoffset", ct.c_uint),
        ("bits_per_pixel", ct.c_uint),
        ("grayscale", ct.c_uint),    # Length of frame buffer mem
        ("line_length", ct.c_uint),  # length of a line in bytes
        ("smem_len", ct.c_uint),
    ]


def gen():
    ctx = zmq.Context()
    socket = ctx.socket(zmq.REQ)
    socket.connect("tcp://192.168.1.7:5555")

    socket.send(b'hello')
    response_bytes = socket.recv()
    hello = HelloResponse.from_buffer_copy(response_bytes)

    stride = hello.line_length
    while True:
        socket.send(b'refresh')
        message = socket.recv()
        arr = np.frombuffer(message, np.uint8)
        arr = arr.reshape((hello.smem_len // stride, stride))
        img = Image.fromarray(arr, 'L')
        img = img.crop(
            (hello.xoffset * 2, hello.yoffset, hello.xres * 2, hello.yres)
        )

        buff = io.BytesIO()
        img.save(buff, 'png')

        buff.seek(0)
        yield (b'--frame\r\n'
               b'Content-Type: image/png\r\n\r\n' + buff.read() + b'\r\n')


@app.route('/video_feed')
def remark_feed():
    return Response(gen(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    app.run()
