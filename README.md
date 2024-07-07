# Visgui

Visgui is a webgl/wasm project that is ment to build 
a performent and visually appealing visualization 
frontend in C++. It uses raylib and imgui as the
basis of this frame 

dependencies:
* [raylib](https://github.com/raysan5/raylib)
* [imgui](https://github.com/ocornut/imgui)
* [implot](https://github.com/epezent/implot)
* [rlImGui](https://github.com/raylib-extras/rlImGui)



# Building 

the building is based in ubuntu and 

```bash
docker build --target build -t visgui.build .
```

# Exporting and Testing 

when exporting and testing the image is based on alpine
since the ubuntu image is ~2GB in size.

```bash
# build
docker build --target build -t visgui.build .
docker build --target test -t visgui.test .
docker build --target export -t visgui.export .

# to test 
docker run -i --rm -p 8000:8000 -p 8765:8765 --env "PYTHONUNBUFFERED=1" visgui.test
# Now navigate to localhost:8000 on a local browser

# To export
docker create visgui.export # This outputs an ID to the shell
docker export <ID> -o visgui.tar.gz # using the ID from the previous line
```

Then extract the tar file. inside `/webgui/app/...` is the wasm files.
