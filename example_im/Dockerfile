# FROM wasm_env:latest AS build
# philipdavis82/visgui.example.export
# 2+ GB
FROM  ubuntu:jammy AS build 

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
RUN ./emsdk install latest 
RUN ./emsdk activate latest

# raylib layer
# COPY ./extern/raylib-lite /visgui/extern/raylib
# WORKDIR /emscripten/emsdk
# RUN . ./emsdk_env.sh && env
# RUN . ./emsdk_env.sh && \
#     cd /visgui/extern/raylib/src &&\
#     make -e PLATFORM=PLATFORM_WEB -e RAYLIB_RELEASE_PATH=/visgui/extern/raylib/web -B

# Tooling layers
COPY ./extern /visgui/extern

# Source Layer
COPY ./example_im /visgui/example_im
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && \
    cd /visgui/example_im &&\
    make -j

# FROM scratch as export
FROM scratch AS export

# COPY --from=build /visgui/example/example.html /visgui/example/index.html 
COPY --from=build  /visgui/example_im/example_im.html      export/example_im.html 
COPY --from=build  /visgui/example_im/example_im.js        export/example_im.js 
COPY --from=build  /visgui/example_im/example_im.wasm      export/example_im.wasm
COPY --from=build  /visgui/example_im/example_im.data      export/example_im.data
COPY --from=build  /visgui/example_im/media                export/media 
COPY --from=build  /visgui/example_im/websocket_server     export/websocket_server 



