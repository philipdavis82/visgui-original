# Building and Testing

```bash
docker build -t <target> .
docker run --rm <target>
```

# Exporting 

Make the Dockerfile look like the one below

```dockerfile
FROM wasm_env:latest AS build
COPY . /webgui
WORKDIR /webgui/app
RUN make -j4

# Comment out this
# CMD ["python3" , "test_server.py"]

# Uncomment the below
FROM scratch
WORKDIR /webgui/app
COPY --from=build /webgui/app/index.html /webgui/app/index.html 
COPY --from=build /webgui/app/imgui.js /webgui/app/imgui.js 
COPY --from=build /webgui/app/imgui.wasm /webgui/app/imgui.wasm
COPY --from=build /webgui/app/imgui.data /webgui/app/imgui.data
COPY --from=build /webgui/app/test_server.py /webgui/app/test_server.py
```

Then run this in the shell

```bash
docker build -t <target> .
docker create <target> # This outputs an ID to the shell
docker export <ID> -o <target>.tar.gz
```

Then extract the tar file. inside `/webgui/app/...` is the wasm files.
