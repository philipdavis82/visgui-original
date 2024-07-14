FROM --platform=$BUILDPLATFORM python:3.11-alpine
# FROM --platform=linux/arm64/v8 python:3.11-alpine AS builder

RUN pip install websockets
COPY . /websocket_server

WORKDIR /websocket_server

CMD ["python3", "example_server.py"]