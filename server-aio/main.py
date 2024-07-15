import aiohttp
from aiohttp import web
import asyncio
import sys,os
import logging

import jinja2


def index_bootstrap(request):
    print(f"index.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/index.html","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/html')
def index_bootstrap_css(request):
    print(f"index.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/bootstrap.min.css","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/css')
def index_bootstrap_cover_css(request):
    print(f"index.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/cover.css","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/css')

def album_bootstrap(request):
    print(f"album.html requested",file=sys.stderr)
    album = jinja2.Environment()
    with open("bootstrap/CoverPage/raylib_imgui.svg","r") as file:
        ray = file.read()
    with open("bootstrap/CoverPage/imgui_standalone.svg","r") as file:
        im = file.read()
    with open("bootstrap/CoverPage/album.html","r") as file:
        data = file.read()
    data = album.from_string(data,globals={
        "raylib_imgui_svg":ray,
        "imgui_svg":im,
    })
    return web.Response(body=data.render(),content_type='text/html')
def album_bootstrap_css(request):
    print(f"album.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/bootstrap.min.css","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/css')
def album_bootstrap_cover_css(request):
    print(f"album.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/album.css","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='text/css')
def album_bootstrap_bundle_js(request):
    print(f"album.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/bootstrap.bundle.min.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
def album_jquery_3_4_1_js(request):
    print(f"album.html requested",file=sys.stderr)
    with open("bootstrap/CoverPage/jquery-3.4.1.slim.min.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
    

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
    template = jinja2.Environment()
    with open("/example/templates/example.html","r") as file:
        data = file.read()
    template = template.from_string(data,globals={
        "JS_VAR_WEBSOCK" : f'<script>const VAR_WEBSOCK=\"{os.environ["EXAMPLE_CLINET_CONN"]}\";</script>'
    })
    return web.Response(body=template.render(),content_type='text/html')
def example_js(request):
    print(f"example.js requested",file=sys.stderr)
    with open("/example/static/example.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
def example_wasm(request):
    print(f"example.wasm requested",file=sys.stderr)
    with open("/example/static/example.wasm","rb") as file:
        data = file.read()
    return web.Response(body=data,content_type="application/wasm")
def example_data(request):
    print(f"example.data requested",file=sys.stderr)
    with open("/example/static/example.data","rb") as file:
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
    template = jinja2.Environment()
    with open("/example_im/templates/example_im.html","r") as file:
        data = file.read()
    template = template.from_string(data,globals={
        "JS_VAR_WEBSOCK" : f'<script>const VAR_WEBSOCK=\"{os.environ["EXAMPLE_IM_CLIENT_CONN"]}\";</script>'
    })
    return web.Response(body=template.render(),content_type='text/html')
def example_im_js(request):
    print(f"example_im.js requested",file=sys.stderr)
    with open("/example_im/static/example_im.js","r") as file:
        data = file.read()
    return web.Response(body=data,content_type='application/javascript')
def example_im_wasm(request):
    print(f"example_im.wasm requested",file=sys.stderr)
    with open("/example_im/static/example_im.wasm","rb") as file:
        data = file.read()
    return web.Response(body=data,content_type="application/wasm")
def example_im_data(request):
    print(f"example_im.data requested",file=sys.stderr)
    with open("/example_im/static/example_im.data","rb") as file:
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
    # app.add_routes([
        # web.get("/",index),
        # web.get("/media/favicon.ico",index_fav),
    # ])
    # Bootstrap
    app.add_routes([
        web.get("/",index_bootstrap),
        web.get("/bootstrap.min.css",index_bootstrap_css),
        web.get("/cover.css",index_bootstrap_cover_css),
        web.get("/favicon.ico",index_fav),
    ])

    app.add_routes([
        web.get("/album/",album_bootstrap),
        web.get("/album/bootstrap.min.css",album_bootstrap_css),
        web.get("/album/album.css",album_bootstrap_cover_css),
        web.get("/album/bootstrap.bundle.min.js",album_bootstrap_bundle_js),
        web.get("/album/jquery-3.4.1.slim.min.js",album_jquery_3_4_1_js),
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
    web.run_app(app,
                host=os.environ["WEBIP"],
                port=int(os.environ["WEBPORT"]))



if __name__ == "__main__":
    setup()