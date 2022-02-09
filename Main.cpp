#include <core/print.hpp>
#include <core/list.hpp>
#include <core/os/window.hpp>
#include <core/os/clock.hpp>
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

class Editor {
private:
    AutoWindow m_Window{1280, 720, "Shit"};
    FramebufferChain m_Swapchain{&m_Window};
    ImGuiBackend m_Backend{m_Swapchain.Pass()};
    List<std::string> m_List;
public:

    Editor() {
        Function<void(const Event &)> handler;
        handler.Bind<Editor, &Editor::OnEvent>(this);
        m_Window.SetEventsHanlder(handler);
    }

    void Run() {
        Clock cl;
        Semaphore acquire, present;

        while (m_Window.IsOpen()) {
            m_Window.DispatchEvents();

            float dt = cl.GetElapsedTime().AsSeconds();
            cl.Restart();
            
            m_Swapchain.AcquireNext(&acquire);
            m_Backend.NewFrame(dt, Mouse::RelativePosition(m_Window), m_Window.Size());
            Draw();
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
        static float color[4];
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(m_Window.Size().x, m_Window.Size().y));
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  
        ImGui::Begin("Window", nullptr, flags);
        for (const auto& string : m_List) 
            ImGui::Text(string.c_str());

        static char s_Buffer[256] = {};
        ImGui::InputText("ToAdd", s_Buffer, sizeof(s_Buffer));
        if (ImGui::Button("Add")) {
            m_List.Add(s_Buffer);
            s_Buffer[0] = 0;
        }
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

