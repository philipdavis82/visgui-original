# FROM wasm_env:latest AS build

# 2+ GB
FROM  ubuntu:jammy AS build 
LABEL stage=builder
# apt layer
RUN apt-get update && apt-get install -y \
    make \
    git \
    python3 \
    xz-utils

# emscripten layer
WORKDIR /emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git

WORKDIR /emscripten/emsdk

# SHELL ["/bin/bash" , "-c"]

RUN ./emsdk install latest 
RUN ./emsdk activate latest

# raylib layer
COPY ./extern/raylib-lite /visgui/raylib
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && env
RUN . ./emsdk_env.sh && \
    cd /visgui/raylib/src &&\
    make -e PLATFORM=PLATFORM_WEB -e RAYLIB_RELEASE_PATH=/visgui/raylib/web -B

# Tooling layers
COPY ./extern/imgui /visgui/imgui
COPY ./extern/implot /visgui/implot
COPY ./extern/rlImGui /visgui/rlImGui

# Source Layer
COPY ./example /visgui/example
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && \
    cd /visgui/example &&\
    make -j

RUN ls /visgui/example

# FROM scratch as export
# 62MB
FROM --platform=$BUILDPLATFORM python:3.11-alpine AS export

COPY --from=build /visgui/example/example.html /visgui/example/index.html 
COPY --from=build /visgui/example/example.html /visgui/example/example.html 
COPY --from=build /visgui/example/example.js /visgui/example/example.js 
COPY --from=build /visgui/example/example.wasm /visgui/example/example.wasm
COPY --from=build /visgui/example/example.data /visgui/example/example.data
COPY --from=build /visgui/example/test_server.py /visgui/example/test_server.py

WORKDIR /visgui/example

EXPOSE 8000:8000

CMD ["python3" , "test_server.py"]


