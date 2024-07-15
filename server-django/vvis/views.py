from django.http import HttpResponse
from django.shortcuts import render
from .settings import BASE_DIR
import os

# RENDER_CONTEXT = jinja2.Environment(
#     loader=PackageLoader()
# )
# RENDER_GLOBALS = RENDER_CONTEXT.make_globals()
# RENDER_GLOBALS[]
# GLOBAL_CONTEXT = {
    # "JS_VAR_WEBSOCK" : f'<script>const VAR_WEBSOCK=\"{\
        # os.environ.get("EXAMPLE_IM_CLIENT_CONN","ws://127.0.0.1:8002/")}\";</script>',
        
# }
# EXAMPLE_CLINET_CONN: "ws://127.0.0.1:8001/"

def __open_read(filename,io="r"):
    with open(filename,io) as file:
        data = file.read()
    return data

MEDIA_CONTEXT = {
    "raylib_imgui_svg" : __open_read(os.path.join(BASE_DIR,"static","media","raylib_imgui.svg")),
    "imgui_svg"        : __open_read(os.path.join(BASE_DIR,"static","media","imgui_standalone.svg")),
}

def homepage(request):
    context = {} 
    return render(request,"index.html",context,content_type="text/html")

def album(request):
    context = {}
    context.update(MEDIA_CONTEXT)
    return render(request,"album.html",context,content_type="text/html")

def example(request):
    context = {
        "JS_VAR_WEBSOCK" : f'<script>const VAR_WEBSOCK=\"{os.environ.get("EXAMPLE_IM_CLIENT_CONN","ws://127.0.0.1:8001/")}\";</script>'
    }
    return render(request,os.path.join("example","example.html"),context,content_type="text/html")


def example_im(request):
    context = {
        "JS_VAR_WEBSOCK" : f'<script>const VAR_WEBSOCK=\"{os.environ.get("EXAMPLE_IM_CLIENT_CONN","ws://127.0.0.1:8002/")}\";</script>'
    }
    return render(request,os.path.join("example_im","example_im.html"),context,content_type="text/html")

