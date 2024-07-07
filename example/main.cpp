#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/websocket.h>
#include <emscripten/bind.h>
#endif

// #define GLFW_INCLUDE_ES3
// #include <GLES3/gl3.h>
// #include <GLFW/glfw3.h>

#include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#include "../implot/implot.h"

#include "raylib.h"
#include "raymath.h"
#include "../rlImGui/rlImGui.h"

#include <iostream>

// GLFWwindow *g_window;

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool show_another_window = false;
int g_width;
int g_height;

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_width, (), {
    return Module.canvas.width;
});

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_height, (), {
    return Module.canvas.height;
});

static float camera_angle = 0;
void EMSCRIPTEN_KEEPALIVE recvServerData(std::string data){
    // std::cout << data << std::endl;
    camera_angle = stof(data);
    return;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("recvServerData", &recvServerData);
}

// EM_JS(std::string, canvas_get_height, (), {
//     return Module.canvas.height;
// });

// void on_size_changed()
// {
//     ImGui::SetCurrentContext(ImGui::GetCurrentContext());
// }

enum SceneMethod{
    INIT,
    RENDER,
    DRAW
};


static RenderTexture ViewTexture;
static Camera3D camera = { 0 };
static Texture2D GridTexture = { 0 };
static bool Open = true;
static bool Focused = true;
static Vector3 original_position; 

void Scene3D(SceneMethod method)
{
    
    if(method == INIT){
        memset(&camera,0,sizeof(camera));
        ViewTexture = LoadRenderTexture(512,512);
        
        camera.fovy = 45;
		camera.up.y = 1;
		camera.position.y = 6;
		camera.position.z = -25;
        original_position = camera.position;
    }

    if(method == RENDER){
        if (!Open)
			return;

        // camera.position.z = 
		float period = 10;
		float magnitude = 25;
        camera.position = original_position;
		// camera.position.x = (float)(sinf((float)GetTime() / period) * magnitude);
        camera.position = Vector3Transform(camera.position,MatrixRotateY(camera_angle));
		// Vector
        BeginTextureMode(ViewTexture);
		ClearBackground(SKYBLUE);

		BeginMode3D(camera);

		// grid of cube trees on a plane to make a "world"
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
		float spacing = 4;
		int count = 5;

		for (float x = -count * spacing; x <= count * spacing; x += spacing)
		{
			for (float z = -count * spacing; z <= count * spacing; z += spacing)
			{
				Vector3 pos = { x, 0.5f, z };

				Vector3 min = { x - 0.5f,0,z - 0.5f };
				Vector3 max = { x + 0.5f,1,z + 0.5f };

				DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
				DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
			}
		}

		EndMode3D();
		EndTextureMode();
    }

    if(method == DRAW){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			rlImGuiImageRenderTextureFit(&ViewTexture, true);
		}
		ImGui::End();
		ImGui::PopStyleVar();
    }
}
static bool first_loop = true;
// static bool size_change
void loop()
{
    int i = 0;

    int width = canvas_get_width();
    int height = canvas_get_height();

    if (width != g_width || height != g_height)
    {
        g_width = width;
        g_height = height;
        // on_size_changed();
    }
    
    
    
    Scene3D(RENDER);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    rlImGuiBegin();
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    
    // if(first_loop)
        // ImGui::SetNextWindowPos(ImVec2(0, 0),ImGuiCond_FirstUseEver);
    Scene3D(DRAW);
    if(first_loop)
        ImGui::SetWindowPos("3D View",ImVec2(0, 300));

    {
        ImGui::Begin("Debug");
        ImGui::SetWindowSize(ImVec2(400,300));
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                            // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Hello");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window)
    {
        // if(first_loop)
            // ImGui::SetNextWindowPos(ImVec2(1000, 0),ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&show_demo_window);
        if(first_loop)
            ImGui::SetWindowPos("Dear ImGui Demo",ImVec2(1000, 0));
    }

    bool show_implot_demo = true;
    // if(first_loop)
        // ImGui::SetNextWindowPos(ImVec2(400, 0), ImGuiCond_FirstUseEver);
    ImPlot::ShowDemoWindow(&show_implot_demo);
    if(first_loop)
        ImGui::SetWindowPos("ImPlot Demo",ImVec2(400, 0));

    if(first_loop)
        ImGui::SetWindowPos("Debug",ImVec2(0, 0));

    first_loop = false;

    rlImGuiEnd();
    EndDrawing();
}

int init_gl()
{
    int canvasWidth = g_width;
    int canvasHeight = g_height;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(canvasWidth, canvasHeight, "webgui-test");
    SetTargetFPS(60);
    Scene3D(INIT);

    rlImGuiSetup(true);
    ImPlot::SetCurrentContext(ImPlot::CreateContext());

    return 0;
}

int init_imgui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 23.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 26.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 32.0f);
    io.FontDefault = io.Fonts->Fonts[1];

    rlImGuiReloadFonts();
    return 0;
}

int init()
{
    init_gl();
    init_imgui();
    return 0;
}

void quit()
{
    rlImGuiShutdown();
}

extern "C" int main(int argc, char **argv)
{
    g_width  = 1920;//canvas_get_width();
    g_height = 1080;//canvas_get_height();
    
    if (init() != 0)
        return 1;

    emscripten_set_main_loop(loop, 0, 1);

    quit();

    return 0;
}
