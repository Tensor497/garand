#include <SDL2pp/SDL2pp.hh>
#include <exception>
#include <fmt/format.h>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <string_view>
#include <vector>

using namespace SDL2pp;

static constexpr uint32_t SCREEN_WIDTH = 640;
static constexpr uint32_t SCREEN_HEIGHT = 480;
static constexpr uint32_t framerate = 60;

char buf[4096];
float f = 0;
bool show_demo_window = false;
void debuggui() {
  if (show_demo_window) {
    ImGui::ShowDemoWindow(&show_demo_window);
  }
  ImGui::Begin("Hello, world!");
  ImGui::Text("Hello, world %d", 123);
  if (ImGui::Button("Save")) {
    fmt::print("Error: Save is not implemented\n");
  }
  // Edit bools storing our window open/close state
  ImGui::Checkbox("Demo Window", &show_demo_window);
  ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  ImGui::End();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  try {
    fmt::print("Booting\n");
    // Initialize SDL library
    SDL sdl{SDL_INIT_VIDEO};

    Window window("garand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  SCREEN_WIDTH, SCREEN_HEIGHT,
                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    // Create accelerated video renderer with default driver
    Renderer renderer{window, -1,
                      SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};

    // Checking
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.Get(), renderer.Get());
    ImGui_ImplSDLRenderer_Init(renderer.Get());
    // Main loop
    auto base_ticks = SDL_GetTicks();
    while (1) {
      // Timing: calculate difference between this and previous frame
      // in milliseconds
      auto const frame_ticks = SDL_GetTicks();
      auto const frame_delta = frame_ticks - base_ticks;
      base_ticks = frame_ticks;

      // Event processing:
      // - If window is closed, or Q or Escape buttons are pressed,
      //   quit the application
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
          return 0;
        } else if (event.type == SDL_KEYDOWN) {
          switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
          case SDLK_q:
            return 0;
          }
        }
      }

      ImGui_ImplSDLRenderer_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      debuggui();
      ImGui::Render();

      // Clear screen
      renderer.Clear();

      // Show rendered frame
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
      renderer.Present();

      // Frame limiter
      SDL_Delay(1);
    }

    // Here all resources are automatically released and library deinitialized
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return 0;
  } catch (std::exception &e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
  }
}