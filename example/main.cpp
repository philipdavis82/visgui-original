#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
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

// Function called by javascript
// EM_JS(void, resizeCanvas, (), {
//     js_resizeCanvas();
// });

// unsigned int rlLoadTextureDepthMOD(int width, int height){
//   unsigned int id = 0;

//   glGenTextures(1, &id);
//   glBindTexture(GL_TEXTURE_2D, id);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24_OES, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//   glBindTexture(GL_TEXTURE_2D, 0);

//   TRACELOG(RL_LOG_INFO, "TEXTURE: Depth texture loaded successfully");
//   return id;
// }

// RenderTexture2D LoadRenderTextureMOD(int width, int height)
// {
//     RenderTexture2D target = { 0 };

//     target.id = rlLoadFramebuffer(width, height);   // Load an empty framebuffer

//     if (target.id > 0)
//     {
//         rlEnableFramebuffer(target.id);

//         // Create color texture (default to RGBA)
//         target.texture.id = rlLoadTexture(0 , width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
//         target.texture.width = width;
//         target.texture.height = height;
//         target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
//         target.texture.mipmaps = 1;

//         // Create depth renderbuffer/texture
//         target.depth.id = rlLoadTextureDepthMOD(width, height);
//         target.depth.width = width;
//         target.depth.height = height;
//         target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
//         target.depth.mipmaps = 1;

//         // Attach color texture and depth renderbuffer/texture to FBO
//         rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
//         rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

//         // Check if fbo is complete with attachments (valid)
//         if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

//         rlDisableFramebuffer();
//     }
//     else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

//     return target;
// }

void on_size_changed()
{
    // glfwSetWindowSize(g_window, g_width, g_height);
    // ResizeWindow()
    // GetScreenWidth()
    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

enum SceneMethod{
    INIT,
    RENDER,
    DRAW
};


static RenderTexture ViewTexture;
// static Rectangle contentRect = { 0 };
static Camera3D camera = { 0 };
static Texture2D GridTexture = { 0 };
static bool Open = true;
static bool Focused = true;
// static position = 0;
void Scene3D(SceneMethod method)
{
    
    if(method == INIT){
        memset(&camera,0,sizeof(camera));
        ViewTexture = LoadRenderTexture(512,512);
        
        // std::cout << IsTextureReady(ViewTexture) << std::endl;
        // ViewTexture = LoadRenderTextureMOD(640,480);
        // camera.fovy = 65;
		// camera.up.y = 1;
        // camera.target.z = 1;
		// camera.position.x = 0;
        // camera.position.y = 0;
		// camera.position.z = 0;
        // camera.
        // camera.projection = CAMERA_PERSPECTIVE;//CAMERA_PERSPECTIVE;
        camera.fovy = 45;
		camera.up.y = 1;
		camera.position.y = 3;
		camera.position.z = -25;
		// Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
		// GridTexture = LoadTextureFromImage(img);
		// UnloadImage(img);
		// GenTextureMipmaps(&GridTexture);
		// SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
		// SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
    }

    if(method == RENDER){
        if (!Open)
			return;

        
		// float period = 10;
		// float magnitude = 25;
        // // camera.fov
        // camera.fovy = 45;
		// camera.up.y = 1;
		// camera.position.y = 3;
		// camera.position.z = -25;
        // camera.fovy =  (float)(sinf((float)GetTime() / period) * 179);;
		// camera.position.x = (float)(sinf((float)GetTime() / period) * magnitude);
        
        // camera.position.z = 
		float period = 10;
		float magnitude = 25;

		camera.position.x = (float)(sinf((float)GetTime() / period) * magnitude);

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
        // BeginTextureMode(ViewTexture);
        //     ClearBackground(SKYBLUE);

        //     BeginMode3D(camera);

        //         DrawCube(Vector3{ 0, 0,  2 }, 1.0f, 1.0f, 1.0f , RED);

        //     EndMode3D();
		// EndTextureMode();

        // UpdateRenderTexture();
    }

    if(method == DRAW){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        // std::cout << GetScreenWidth() << ", " << GetScreenHeight() << std::endl;
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			// draw the view
			rlImGuiImageRenderTextureFit(&ViewTexture, true);
            // rlImGuiImageRect(&ViewTexture.texture, 640,480, {640,480});	
		}
		ImGui::End();
		ImGui::PopStyleVar();
    }
}

void loop()
{
    int i = 0;

    int width = canvas_get_width();
    int height = canvas_get_height();

    if (width != g_width || height != g_height)
    {
        g_width = width;
        g_height = height;
        on_size_changed();
    }
    
    
    
    Scene3D(RENDER);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    rlImGuiBegin();
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    
    
    Scene3D(DRAW);
    
    {
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
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    bool show_implot_demo = true;
    ImPlot::ShowDemoWindow(&show_implot_demo);

    rlImGuiEnd();
    // ImGui::Render();
    EndDrawing();

    // int display_w, display_h;
    // glfwMakeContextCurrent(g_window);
    // glfwGetFramebufferSize(g_window, &display_w, &display_h);
    // glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glfwMakeContextCurrent(g_window);
}

int init_gl()
{
    // if( !glfwInit() )
    // {
    //     fprintf( stderr, "Failed to initialize GLFW\n" );
    //     return 1;
    // }

    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    int canvasWidth = g_width;
    int canvasHeight = g_height;
    // g_window = glfwCreateWindow(canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(canvasWidth, canvasHeight, "webgui-test");
    SetTargetFPS(60);
    // auto context = ImPlot::GetCurrentContext();
    Scene3D(INIT);

    rlImGuiSetup(true);
    ImPlot::SetCurrentContext(ImPlot::CreateContext());
    

    // if( g_window == NULL )
    // {
    //     fprintf( stderr, "Failed to open GLFW window.\n" );
    //     glfwTerminate();
    //     return -1;
    // }
    // glfwMakeContextCurrent(g_window); // Initialize GLEW

    return 0;
}

int init_imgui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    // ImGui::CreateContext();
    // ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    // ImGui_ImplOpenGL3_Init();
    // rlImGuiSetup(true);
    // Setup style
    // ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();

    // Load Fonts
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);

    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 23.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 26.0f);
    io.Fonts->AddFontFromFileTTF("data/open-sans.ttf", 32.0f);
    
    // io.Fonts->AddFontDefault();
    io.FontDefault = io.Fonts->Fonts[1];

    // io.Fonts    

    rlImGuiReloadFonts();

    // resizeCanvas();

    

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
    // glfwTerminate();
}

extern "C" int main(int argc, char **argv)
{
    g_width  = 1920;//canvas_get_width();
    g_height = 1080;//canvas_get_height();
    if (init() != 0)
        return 1;

    // #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
    // #else
    // while (!glfwWindowShouldClose(g_window)){
    // loop();
    // }
    // #endif

    quit();

    return 0;
}
