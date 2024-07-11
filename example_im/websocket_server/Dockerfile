FROM --platform=$BUILDPLATFORM python:3.11-alpine

RUN pip install websockets
COPY . /websocket_server

WORKDIR /websocket_server

CMD ["python3", "example_server.py"]