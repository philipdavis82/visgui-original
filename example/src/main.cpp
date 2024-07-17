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
#include "imgui_internal.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#include "../implot/implot.h"

#include "raylib.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "raymath.h"
#include "../rlImGui/rlImGui.h"

#include <iostream>

// #define GLSL_VERSION            100
#define MAX_LIGHTS 4

static std::string fragment_sobel = "#version 100\n\
precision mediump float;\n\
// Input vertex attributes (from vertex shader)\n\
varying vec2 fragTexCoord;\n\
varying vec4 fragColor;\n\
// Input uniform values\n\
uniform sampler2D texture0;\n\
uniform vec4 colDiffuse;\n\
// NOTE: Add here your custom variables\n\
vec2 resolution = vec2(800.0, 450.0);\n\
void main()\n\
{\n\
    float x = 1.0/resolution.x;\n\
    float y = 1.0/resolution.y;\n\
    vec4 horizEdge = vec4(0.0);\n\
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))*1.0;\n\
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y    ))*2.0;\n\
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))*1.0;\n\
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))*1.0;\n\
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y    ))*2.0;\n\
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))*1.0;\n\
    vec4 vertEdge = vec4(0.0);\n\
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))*1.0;\n\
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y - y))*2.0;\n\
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))*1.0;\n\
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))*1.0;\n\
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y + y))*2.0;\n\
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))*1.0;\n\
    vec3 edge = sqrt((horizEdge.rgb*horizEdge.rgb) + (vertEdge.rgb*vertEdge.rgb));\n\
    gl_FragColor = vec4(edge, texture2D(texture0, fragTexCoord).a);\n\
}";

static std::string fragment_lighting = "\
#version 100\n\
precision mediump float;\n\
// Input vertex attributes (from vertex shader)\n\
varying vec3 fragPosition;\n\
varying vec2 fragTexCoord;\n\
varying vec4 fragColor;\n\
varying vec3 fragNormal;\n\
// Input uniform values\n\
uniform sampler2D texture0;\n\
uniform vec4 colDiffuse;\n\
// NOTE: Add here your custom variables\n\
#define     MAX_LIGHTS              4\n\
#define     LIGHT_DIRECTIONAL       0\n\
#define     LIGHT_POINT             1\n\
struct Light {\n\
    int enabled;\n\
    int type;\n\
    vec3 position;\n\
    vec3 target;\n\
    vec4 color;\n\
};\n\
// Input lighting values\n\
uniform Light lights[MAX_LIGHTS];\n\
uniform vec4 ambient;\n\
uniform vec3 viewPos;\n\
void main(){\n\
    // Texel color fetching from texture sampler\n\
    vec4 texelColor = texture2D(texture0, fragTexCoord);\n\
    vec3 lightDot = vec3(0.0);\n\
    vec3 normal = normalize(fragNormal);\n\
    vec3 viewD = normalize(viewPos - fragPosition);\n\
    vec3 specular = vec3(0.0);\n\
    // NOTE: Implement here your fragment shader code\n\
    for (int i = 0; i < MAX_LIGHTS; i++){\n\
        if (lights[i].enabled == 1){\n\
            vec3 light = vec3(0.0);\n\
            if (lights[i].type == LIGHT_DIRECTIONAL){\n\
                light = -normalize(lights[i].target - lights[i].position);\n\
            }\n\
            if (lights[i].type == LIGHT_POINT){\n\
                light = normalize(lights[i].position - fragPosition);\n\
            }\n\
            float NdotL = max(dot(normal, light), 0.0);\n\
            lightDot += lights[i].color.rgb*NdotL;\n\
            float specCo = 0.0;\n\
            if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 16.0); // 16 refers to shine\n\
            specular += specCo;\n\
        }\n\
    }\n\
    vec4 finalColor = (texelColor*((colDiffuse + vec4(specular, 1.0))*vec4(lightDot, 1.0)));\n\
    finalColor += texelColor*(ambient/10.0);\n\
    // Gamma correction\n\
    gl_FragColor = pow(finalColor, vec4(1.0/2.2));\n\
}\n";

static std::string vertex_lighting = "\
#version 100\n\
// Input vertex attributes\n\
attribute vec3 vertexPosition;\n\
attribute vec2 vertexTexCoord;\n\
attribute vec3 vertexNormal;\n\
attribute vec4 vertexColor;\n\
// Input uniform values\n\
uniform mat4 mvp;\n\
uniform mat4 matModel;\n\
// Output vertex attributes (to fragment shader)\n\
varying vec3 fragPosition;\n\
varying vec2 fragTexCoord;\n\
varying vec4 fragColor;\n\
varying vec3 fragNormal;\n\
// NOTE: Add here your custom variables\n\
// https://github.com/glslify/glsl-inverse\n\
mat3 inverse(mat3 m)\n\
{\n\
  float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];\n\
  float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];\n\
  float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];\n\
  float b01 = a22*a11 - a12*a21;\n\
  float b11 = -a22*a10 + a12*a20;\n\
  float b21 = a21*a10 - a11*a20;\n\
  float det = a00*b01 + a01*b11 + a02*b21;\n\
  return mat3(b01, (-a22*a01 + a02*a21), (a12*a01 - a02*a11),\n\
              b11, (a22*a00 - a02*a20), (-a12*a00 + a02*a10),\n\
              b21, (-a21*a00 + a01*a20), (a11*a00 - a01*a10))/det;\n\
}\n\
// https://github.com/glslify/glsl-transpose\n\
mat3 transpose(mat3 m)\n\
{\n\
  return mat3(m[0][0], m[1][0], m[2][0],\n\
              m[0][1], m[1][1], m[2][1],\n\
              m[0][2], m[1][2], m[2][2]);\n\
}\n\
void main()\n\
{\n\
    // Send vertex attributes to fragment shader\n\
    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));\n\
    fragTexCoord = vertexTexCoord;\n\
    fragColor = vertexColor;\n\
    mat3 normalMatrix = transpose(inverse(mat3(matModel)));\n\
    fragNormal = normalize(normalMatrix*vertexNormal);\n\
    // Calculate final vertex position\n\
    gl_Position = mvp*vec4(vertexPosition, 1.0);\n\
}\n";

static std::string fragment_cross_hatching = "\
# version 100\n\
precision mediump float;\n\
// Input vertex attributes (from vertex shader)\n\
varying vec2 fragTexCoord;\n\
varying vec4 fragColor;\n\
// Input uniform values\n\
uniform sampler2D texture0;\n\
uniform vec4 colDiffuse;\n\
// NOTE: Add here your custom variables\n\
float hatchOffsetY = 5.0;\n\
float lumThreshold01 = 0.9;\n\
float lumThreshold02 = 0.7;\n\
float lumThreshold03 = 0.5;\n\
float lumThreshold04 = 0.3;\n\
void main(){\n\
    vec3 tc = vec3(1.0, 1.0, 1.0);\n\
    float lum = length(texture2D(texture0, fragTexCoord).rgb);\n\
    if (lum < lumThreshold01){\n\
        if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);}\n\
    if (lum < lumThreshold02){\n\
        if (mod(gl_FragCoord .x - gl_FragCoord .y, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);}\n\
    if (lum < lumThreshold03){\n\
        if (mod(gl_FragCoord .x + gl_FragCoord .y - hatchOffsetY, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);}\n\
    if (lum < lumThreshold04){\n\
        if (mod(gl_FragCoord .x - gl_FragCoord .y - hatchOffsetY, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);}\n\
    gl_FragColor = vec4(tc, 1.0);\n\
}\n";


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

static bool  connection_established = false;
static float camera_angle = 0;
void EMSCRIPTEN_KEEPALIVE recvServerData(std::string data){
    connection_established = true;
    camera_angle = stof(data); return;
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




static RenderTexture ViewTexture;
static Camera3D camera = { 0 };
static Texture2D GridTexture = { 0 };
static bool Open = true;
static bool Focused = true;
static Vector3 original_position;

static Light lights[MAX_LIGHTS] = { 0 };

static Shader default_shader;
static Shader sobel_shader;
static Shader lighting_shader;
static Shader cross_hatching_shader;

static Shader *shader_list[] = {
    &default_shader,
    &sobel_shader,
    &lighting_shader,
    &cross_hatching_shader
};

const char* shadersCombo[] = {
    "defualt_shader",
    "sobel_shader",
    "lighting_shader",
    "cross_hatching"
};

static int shaderSel = 0;

enum SceneMethod{ INIT, RENDER, DRAW };

void Scene3D(SceneMethod method)
{
    
    if(method == INIT){
        default_shader  = LoadShaderFromMemory(0,0);
        sobel_shader    = LoadShaderFromMemory(0,fragment_sobel.c_str());
        lighting_shader = LoadShaderFromMemory(
            vertex_lighting.c_str(),
            fragment_lighting.c_str());
        cross_hatching_shader = LoadShaderFromMemory(0,fragment_cross_hatching.c_str());
        
        int ambientLoc = GetShaderLocation(lighting_shader, "ambient");
        SetShaderValue(lighting_shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
        lighting_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lighting_shader, "viewPos");

        lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -3, 1,  0 }, Vector3Zero(), YELLOW, lighting_shader);
        lights[1] = CreateLight(LIGHT_POINT, (Vector3){  3, 1,  0 }, Vector3Zero(), RED, lighting_shader);
        lights[2] = CreateLight(LIGHT_POINT, (Vector3){  0, 1,  3 }, Vector3Zero(), GREEN, lighting_shader);
        lights[3] = CreateLight(LIGHT_POINT, (Vector3){  0, 1, -3 }, Vector3Zero(), BLUE, lighting_shader);

        rlguiLoadShader(default_shader,0);
        rlguiLoadShader(sobel_shader,1);
        rlguiLoadShader(lighting_shader,2);
        rlguiLoadShader(cross_hatching_shader,3);

        memset(&camera,0,sizeof(camera));
        ViewTexture = LoadRenderTexture(512,512);
        
        camera.fovy = 45;
		camera.up.y = 1;
		camera.position.y = 2.2;
		camera.position.z = -5;
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
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(lighting_shader, lighting_shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        
        if (IsKeyPressed(KEY_ONE))   { lights[0].enabled   = !lights[0].enabled; }
        if (IsKeyPressed(KEY_TWO))   { lights[1].enabled   = !lights[1].enabled; }
        if (IsKeyPressed(KEY_THREE)) { lights[2].enabled   = !lights[2].enabled; }
        if (IsKeyPressed(KEY_FOUR))  { lights[3].enabled   = !lights[3].enabled; }
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(lighting_shader, lights[i]);
		// Vector
        BeginTextureMode(ViewTexture);
		ClearBackground(SKYBLUE);

		BeginMode3D(camera);

            if(shaderSel == 2) BeginShaderMode(*shader_list[shaderSel]);
                if(shaderSel == 2)  DrawCube(Vector3{0,0,0},2,2,2,WHITE);
                else                DrawCube(Vector3{0,0,0},2,2,2,GREEN);
                DrawCubeWires(Vector3{0,0,0},2,2,2, CLITERAL(Color){ 0, 0, 0, 200 });
            if(shaderSel == 2) EndShaderMode();
		EndMode3D();
		EndTextureMode();
    }

    if(method == DRAW){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		// ImGui::SetNextWindowSizeConstraints(ImVec2(400, 500), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			rlImGuiImageRenderTextureFit(&ViewTexture, true);
            rlguiEnableShader(shaderSel,0,1,"3D View");
            ImGui::Combo("Shaders",&shaderSel,shadersCombo,IM_ARRAYSIZE(shadersCombo));
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

    if(ImGui::GetIO().Framerate > 1 && ImGui::GetIO().Framerate < 2000){
        float ROTATIONS_PER_MIN = 9.0;
        if(!connection_established){
            camera_angle -= GetFrameTime()/60.0*ROTATIONS_PER_MIN*2.0*3.14159;
            while(camera_angle < -2*3.14159){
                camera_angle += 2*3.14159;
            }
        }
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    rlImGuiBegin();
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".

    Scene3D(DRAW);
    if(first_loop)
        ImGui::SetWindowPos("3D View",ImVec2(0, 300));
    if(first_loop)
        ImGui::SetWindowSize("3D View",ImVec2(400, 500));

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
        ImGui::ShowDemoWindow(&show_demo_window);
        if(first_loop)
            ImGui::SetWindowPos("Dear ImGui Demo",ImVec2(1000, 0));
    }

    bool show_implot_demo = true;
    ImPlot::ShowDemoWindow(&show_implot_demo);
    if(first_loop)
        ImGui::SetWindowPos("ImPlot Demo",ImVec2(400, 0));

    if(first_loop)
        ImGui::SetWindowPos("Debug",ImVec2(0, 0));

    first_loop = false;

    rlImGuiEnd();
    EndDrawing();

    rlguiClearShaders();
}

int init_gl()
{
    int canvasWidth  = g_width;
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
