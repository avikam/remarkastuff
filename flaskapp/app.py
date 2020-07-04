import io
from flask import Flask, Response
import zmq
from PIL import Image
import numpy as np

app = Flask(__name__)

def gen():
    ctx = zmq.Context()
    socket = ctx.socket(zmq.REQ)
    socket.connect("tcp://192.168.1.7:5555")

    while True:
        socket.send(b'refresh')
        message = socket.recv()
        arr = np.frombuffer(message, np.uint16)
        arr = arr.reshape((3840 // 2, 1408 * 2))
        img = Image.fromarray(arr, 'L')

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
