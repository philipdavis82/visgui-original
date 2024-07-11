import aiohttp
from aiohttp import web
import asyncio
import sys
import logging

# Main Page
def index(request):
    print(f"index.html requested",file=sys.stderr)
    with open("index.html","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/html')
def index_fav(request):
    print(f"index favicon.ico requested",file=sys.stderr)
    with open("media/favicon.ico","rb") as file:
        data = file.read()
    return web.Response(body=data)#,content_type='text/plain')

# Raylib Example
def example(request):
    print(f"example.html requested",file=sys.stderr)
    with open("/example/example.html","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/html')
def example_js(request):
    print(f"example.js requested",file=sys.stderr)
    with open("/example/example.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
def example_wasm(request):
    print(f"example.wasm requested",file=sys.stderr)
    with open("/example/example.wasm","rb") as file:
        data = file.read()
    return web.Response(body=data,content_type="application/wasm")
def example_data(request):
    print(f"example.data requested",file=sys.stderr)
    with open("/example/example.data","rb") as file:
        data = file.read()
    return web.Response(body=data)
def example_fav(request):
    print(f"example favicon.ico requested",file=sys.stderr)
    with open("/example/media/favicon.ico","rb") as file:
        data = file.read()
    return web.Response(body=data)#,content_type='text/plain')

# ImGui Standalone Example
def example_im(request):
    print(f"example_im.html requested",file=sys.stderr)
    with open("/example_im/example_im.html","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/html')
def example_im_js(request):
    print(f"example_im.js requested",file=sys.stderr)
    with open("/example_im/example_im.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
def example_im_wasm(request):
    print(f"example_im.wasm requested",file=sys.stderr)
    with open("/example_im/example_im.wasm","rb") as file:
        data = file.read()
    return web.Response(body=data,content_type="application/wasm")
def example_im_data(request):
    print(f"example_im.data requested",file=sys.stderr)
    with open("/example_im/example_im.data","rb") as file:
        data = file.read()
    return web.Response(body=data)
def example_im_fav(request):
    print(f"example_im favicon.ico requested",file=sys.stderr)
    with open("/example_im/media/favicon.ico","rb") as file:
        data = file.read()
    return web.Response(body=data)#,content_type='text/plain')
    

def setup():
    app = web.Application()
    # logging.basicConfig(level=logging.DEBUG)
    # Main
    app.add_routes([
        web.get("/",index),
        web.get("/media/favicon.ico",index_fav),
    ])
    # Example
    app.add_routes([
        web.get("/example/",example),
        web.get("/example/example.js",example_js),
        web.get("/example/example.wasm",example_wasm),
        web.get("/example/example.data",example_data),
        web.get("/example/media/favicon.ico",example_fav),
    ])

    app.add_routes([
        web.get("/example_im/",example_im),
        web.get("/example_im/example_im.js",example_im_js),
        web.get("/example_im/example_im.wasm",example_im_wasm),
        web.get("/example_im/example_im.data",example_im_data),
        web.get("/example_im/media/favicon.ico",example_im_fav),
    ])
    web.run_app(app,port=8000)



if __name__ == "__main__":
    setup()