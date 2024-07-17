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


# Building Wasm Examples

## WASM Examples

Building example wasm project:
Build this in the top level directory. `cd <PATH>/visgui`


```bash
# Example Build
docker build --target build  -t visgui.example.build  --file example/Dockerfile .
docker build --target export -t visgui.example.export --file example/Dockerfile .
# Imgui Standalone Build
docker build --target build  -t visgui.example_im.build  --file example_im/Dockerfile .
docker build --target export -t visgui.example_im.export --file example_im/Dockerfile .
```

Changing server to use local image. Change `server-django/Dockerfile`

```Dockerfile
#FROM philipdavis82/visgui.example.export as example  <----- "Comment out this"
FROM visgui.example.export as example                 <----- "Add This"
```


# Running Docker Locally

To run the server and the websocket service run the following

```bash
docker compose build
docker compose up
```

Now go to localhost:8000 to connect to the local server.

# Deploying the server

follow the steps to setup and open the tunnel 

`https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/get-started/create-remote-tunnel/`

or 

`https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/get-started/create-local-tunnel/`

Once the tunnel is up run the following.

```bash
# Before this configure the cloudflared tunnel for this device.
git clone/pull git@github.com:philipdavis82/visgui.git
cd visgui
export SECRET_KEY="<insert key here>"
docker compose -f compose-deploy.yaml up --build
# All Done. This should be running now.
```

# Other Setups 

The above uses nginx and django but the aiohttp build is also available. 

Change the `compose.yaml` file to run

`django` setup

```yaml
    # build:
      # context: server-aio
    build:
      context: server-django
    command : sh -c "python manage.py runserver 0.0.0.0:8000"
```

`aiohttp` setup

```yaml
    build:
      context: server-aio
    #build:
     # context: server-django
    #command : sh -c "python manage.py runserver 0.0.0.0:8000"
```

# Future plans

* Get aio running with a reverse proxy `https://docs.aiohttp.org/en/stable/deployment.html`
* Get lighttpd running as the reverse proxy instead of nginx. 