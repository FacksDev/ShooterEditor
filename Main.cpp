#include <core/print.hpp>
#include <core/list.hpp>
#include <core/os/window.hpp>
#include <core/os/clock.hpp>
#include <core/unique_ptr.hpp>
#include <graphics/api/swapchain.hpp>
#include <imgui/backend.hpp>
#include <imgui/widgets.hpp>
#include <string>

struct AutoWindow: Window{
    AutoWindow(int width, int height, const char* title) {
        Open(width, height, title);
    }

    ~AutoWindow() {
        Close();
    }
};

namespace ImGui{
    void AutoDockSpace(Vector2f window_size){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
        ImGui::SetNextWindowPos({0, 0});
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        flags |= ImGuiWindowFlags_NoMove;

        ImGui::Begin("DockWindow", nullptr, flags);
        {
            ImGui::DockSpace(ImGui::GetID("Dockspace"));
        }
        ImGui::End();

        ImGui::PopStyleVar(2);
    }
}

class Editor {
private:
    AutoWindow m_Window{1280, 720, "Shit"};
    FramebufferChain m_Swapchain{&m_Window};
    ImGuiBackend m_Backend{m_Swapchain.Pass()};
    UniquePtr<Texture2D> m_Texture{Texture2D::Create(1, 1, TextureFormat::RGBA8, TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout::ShaderReadOnlyOptimal)};
public:

    Editor() {
        m_Window.SetEventsHanlder({ this, &Editor::OnEvent });
        u32 white = 0xFFFFFFFF;
        m_Texture->Copy(&white, {1, 1});
    }

    void Run() {
        Clock cl;
        Semaphore acquire, present;

        while (m_Window.IsOpen()) {
            m_Window.DispatchEvents();

            float dt = cl.GetElapsedTime().AsSeconds();
            cl.Restart();
            
            m_Backend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());
            Draw();

            m_Swapchain.AcquireNext(&acquire);
            m_Backend.RenderFrame(m_Swapchain.CurrentFramebuffer(), &acquire, &present);
            m_Swapchain.PresentCurrent(&present);
        }
    }

    void OnEvent(const Event& e) {
        if(e.Type == EventType::WindowClose)
            m_Window.Close();
        m_Backend.HandleEvent(e);
    }

    void Draw() {
        ImGui::AutoDockSpace(m_Window.Size());
        ImGui::ShowDemoWindow();
  
        ImGui::Begin("EditorWindow");

        ImGui::End();

        ImGui::Begin("Text Window");
        ImGui::Text("Romchick");
        ImGui::End();
    }
};

int StraitXMain(Span<const char*> args){
    Println("Suck some dick");

    Editor().Run();

    return 0;
}

