CXX = emcc
LINK = emar
OUTPUT = example_im.js
# OUTPUT = example_im.html
IMGUI_DIR:=../../extern/imgui
IMPLOT_DIR:= ../../extern/implot
# RLGUI_DIR:= ../extern/rlImGui
# RAYLIB_DIR:= ../extern/raylib

SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui_tables.cpp
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_demo.cpp $(IMPLOT_DIR)/implot_items.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
# SOURCES += $(RLGUI_DIR)/rlImGui.cpp

# INCLUDES  = -I$(RAYLIB_DIR)/src 
INCLUDES += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I../

OBJECTS = $(SOURCES:%.cpp=%.o)

LIBS = -lGL -lembind
# LIBS = $(RAYLIB_DIR)/web/libraylib.a -L$(RAYLIB_DIR)  -lembind

WEBGL_VER = -s ASYNCIFY \
			-s EXPORTED_RUNTIME_METHODS=ccall,cwrap \
			-s USE_GLFW=3 \
			-s ERROR_ON_UNDEFINED_SYMBOLS=1 \
			-s ASSERTIONS=1 \
			-s INITIAL_MEMORY=52428800 \
			-s FORCE_FILESYSTEM=1 \
			-s ALLOW_MEMORY_GROWTH=0 \
			-Wall \
			# --shell-file example_im.html \
			# -s EXPORTED_FUNCTIONS=_main,_malloc,_recvServerData \
			# --shell-file $(RAYLIB_DIR)/src/minshell.html

USE_WASM = -s WASM=1 

RAYLIB_DEFINES = -DNO_FONT_AWESOME -DPLATFORM_WEB -D_DEFAULT_SOURCE

all: $(OUTPUT)

%.o: %.cpp 
	$(CXX) -DImDrawIdx="unsigned int" -std=c++17 $(RAYLIB_DEFINES) $(INCLUDES) -c $< -o $@

$(OUTPUT): $(OBJECTS)
	$(CXX) -DImDrawIdx="unsigned int" -std=c++17 -o $(OUTPUT) $(OBJECTS) $(LIBS) $(RAYLIB_DEFINES) $(WEBGL_VER) -O2 --preload-file data $(USE_WASM) $(INCLUDES)


clean:
	rm -f $(OUTPUT)
