#include <cstdio>

#define RGFW_IMGUI_IMPLEMENTATION
#include "../imgui_impl_rgfw.h"

#include "../imgui/backends/imgui_impl_opengl2.h"

/* I'm using opengl 2 here because it requires less setup and I'm lazy */
#include "../imgui/backends/imgui_impl_opengl2.cpp"

/* handle these functions across all apis */
void imgui_newFrame(void);
void imgui_render(void);
void imgui_shutdown(void);

int main() {
    RGFW_window* win = RGFW_createWindow("imgui", RGFW_RECT(0, 0, 700, 600), RGFW_CENTER);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = 1.0f / 60.0f;

    // Build atlas
    unsigned char* tex_pixels = nullptr;
    int tex_w, tex_h;
    io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

    ImGui_ImplRgfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL2_Init();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
        RGFW_window_checkEvents(win, 0);
        io.DisplaySize = ImVec2(win->r.w, win->r.h);

        imgui_newFrame();

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            char buffer[200];
            ImGui::InputText("label", buffer, 200);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
        ImGui::End();


        glViewport(0, 0, 500, 500);
        glClearColor(0xFF, 0xFF, 0xFF, 0xFF);
        glClear(GL_COLOR_BUFFER_BIT);

        imgui_render();
        RGFW_window_swapBuffers(win);
    }

    imgui_shutdown();
    RGFW_window_close(win);
    return 0;
}

void imgui_newFrame() {
    ImGui::NewFrame();
    ImGui_ImplRgfw_NewFrame();
    ImGui_ImplOpenGL2_NewFrame();
}

void imgui_render(void) {
    ImGui::Render();

    ImDrawData* draw_data = ImGui::GetDrawData();

    ImGui_ImplOpenGL2_RenderDrawData(draw_data);
}

void imgui_shutdown(void) {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplRgfw_Shutdown();
    ImGui::DestroyContext();
}