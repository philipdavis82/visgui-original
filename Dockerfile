# FROM wasm_env:latest AS build

FROM  ubuntu:jammy AS build
LABEL stage=builder
# apt layer
RUN apt-get update && apt-get install -y \
    make \
    git \
    python3 \
    xz-utils

    # freeglut3-dev \
    #\
    # libglfw3-dev \
    # emscripten 

# emscripten layer
WORKDIR /emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git

WORKDIR /emscripten/emsdk

# SHELL ["/bin/bash" , "-c"]

RUN ./emsdk install latest 
RUN ./emsdk activate latest
# RUN echo "source /emscripten/emsdk/emsdk_env.sh" >> ~/.bashrc
# RUN cat /etc/bash.bashrc

# raylib layer
COPY ./raylib-lite /webgui/raylib
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && env
RUN . ./emsdk_env.sh && \
    cd /webgui/raylib/src &&\
    make -e PLATFORM=PLATFORM_WEB -e RAYLIB_RELEASE_PATH=/webgui/raylib/web -B

# Tooling layers
COPY ./imgui /webgui/imgui
COPY ./implot /webgui/implot
COPY ./rlImGui /webgui/rlImGui

# Source Layer
COPY ./app /webgui/app
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && \
    cd /webgui/app &&\
    make -j

RUN ls /webgui/app

# FROM scratch as export
FROM --platform=$BUILDPLATFORM python:3.11-alpine as export

COPY --from=build /webgui/app/imgui.html /webgui/app/index.html 
COPY --from=build /webgui/app/imgui.js /webgui/app/imgui.js 
COPY --from=build /webgui/app/imgui.wasm /webgui/app/imgui.wasm
COPY --from=build /webgui/app/imgui.data /webgui/app/imgui.data
COPY --from=build /webgui/app/test_server.py /webgui/app/test_server.py

WORKDIR /webgui/app

EXPOSE 8000

CMD ["python3" , "test_server.py"]


