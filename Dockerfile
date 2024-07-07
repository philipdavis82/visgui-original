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
RUN ./emsdk install latest 
RUN ./emsdk activate latest

# raylib layer
COPY ./extern/raylib-lite /visgui/extern/raylib
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && env
RUN . ./emsdk_env.sh && \
    cd /visgui/extern/raylib/src &&\
    make -e PLATFORM=PLATFORM_WEB -e RAYLIB_RELEASE_PATH=/visgui/extern/raylib/web -B

# Tooling layers
COPY ./extern /visgui/extern
# COPY ./extern/imgui /visgui/extern/imgui
# COPY ./extern/implot /visgui/extern/implot
# COPY ./extern/rlImGui /visgui/extern/rlImGui
# COPY ./extern/simdjson /visgui/extern/simdjson
# COPY ./extern/rapidjson /visgui/extern/rapidjson

# Source Layer
COPY ./example /visgui/example
WORKDIR /emscripten/emsdk
RUN . ./emsdk_env.sh && \
    cd /visgui/example &&\
    make -j

# 62MB
FROM --platform=$BUILDPLATFORM python:3.11-alpine AS test

RUN pip install websockets

WORKDIR /visgui/example

COPY --from=build /visgui/example/example.html /visgui/example/index.html 
COPY --from=build /visgui/example/example.html /visgui/example/example.html 
COPY --from=build /visgui/example/example.js /visgui/example/example.js 
COPY --from=build /visgui/example/example.wasm /visgui/example/example.wasm
COPY --from=build /visgui/example/example.data /visgui/example/example.data
COPY --from=build /visgui/example/media /visgui/example/media 

COPY ./test_server/test_server.py /visgui/example/test_server.py
COPY ./test_server/test_websocket.py /visgui/example/test_websocket.py
COPY ./test_server/start.sh /visgui/example/start.sh

# Html Port
EXPOSE 8000:8000
# Websocket Port
EXPOSE 8765:8765 

CMD ["sh" , "start.sh"]
# CMD ["python3" , "test_server.py"]


# FROM scratch as export
FROM scratch AS export

COPY --from=build /visgui/example/example.html /visgui/example/index.html 
COPY --from=build /visgui/example/example.html /visgui/example/example.html 
COPY --from=build /visgui/example/example.js /visgui/example/example.js 
COPY --from=build /visgui/example/example.wasm /visgui/example/example.wasm
COPY --from=build /visgui/example/example.data /visgui/example/example.data
COPY --from=build /visgui/example/media /visgui/example/media 



