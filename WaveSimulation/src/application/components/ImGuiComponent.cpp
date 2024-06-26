#include "ImGuiComponent.h"

#include "application/application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

ImGuiComponent::ImGuiComponent(const std::string& debugName)
	: Component(debugName)
{
}

void ImGuiComponent::init()
{
	std::cout << "INFO: Initializing GUI component" << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
 
	Application& app = Application::getInstance();
	Window& window = app.getWindow();
	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void ImGuiComponent::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiComponent::onRender()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiComponent::startFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiComponent::endFrame()
{
	ImGui::Render();
}


