#include "simulationComponent.h"

#include "application/application.h"
#include "visualization/camera.h"
#include "utils/parameters.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>
#include <format>

SimulationComponent::SimulationComponent(const std::string& debugName)
	: Component(debugName)
{
}

void SimulationComponent::init()
{
	std::cout << "INFO: Initializing " << m_debugName << std::endl;

	m_simGrid = std::make_unique<AmplitudeGrid>(
		100.0f,		//size
		0.01f,		// min wave length
		10.0f		// max wave length
	);


	// register mouse movement callback
	Application& app = Application::getInstance();
	Window& window = app.getWindow();
	glfwSetCursorPosCallback(window.getHandle(), [](GLFWwindow* w, double x, double y)
		{
			const auto& io = ImGui::GetIO();
			if (io.WantCaptureMouse) return;

			if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT))
			{
				AmplitudeGrid& simGrid = Application::getInstance().getSimComp().getGrid();
				glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
				simGrid.addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
			}
		});
	
	//register mouse click callback
	glfwSetMouseButtonCallback(window.getHandle(), [](GLFWwindow* w, int button, int action, int mods)
		{
			const auto& io = ImGui::GetIO();
			if (io.WantCaptureMouse) return;

			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(w, &x, &y);
				AmplitudeGrid& simGrid = Application::getInstance().getSimComp().getGrid();
				glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
				simGrid.addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
			}
		});
}

void SimulationComponent::destroy()
{
}

void SimulationComponent::onUpdate(float dt)
{
	float dtt = dt;
	// check if time step meets cfl condition
	double cfl_dt = m_simGrid->cflTimeStep(dt ,m_timeMultiplier);
	m_dtLast = cfl_dt;
	// perform a simulation step
	m_simGrid->timeStep(cfl_dt, m_isUpdateGrid);
}

static bool selected = false;
static bool selected2 = true;
void SimulationComponent::onRenderGui()
{
	ImGui::Begin("Simulation");

	ImGui::Checkbox("Update Grid", &m_isUpdateGrid);
	ImGui::SliderFloat("Time Multiplier", &m_timeMultiplier, -2.0f, 2.0f);
	ImGui::SliderInt("Periodicity of profile", &m_simGrid->m_periodicity, 1, 10);
	//ImGui::InputFloat("Used dt", &m_dtLast, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Current dt: %.3f", m_dtLast);

	const char* spectra[] = { "Pierson-Moskowitz", "JONSWAP", "Tessendorf"};
	static int selected = 0;
	if (ImGui::BeginCombo("Spectrum", spectra[selected]))
	{
		for (int i = 0; i < _countof(spectra); ++i)
		{
			bool is_selected = selected == i;
			if (ImGui::Selectable(spectra[i], is_selected))
			{
				selected = i;
				m_simGrid->setSpectrum(i);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndCombo();
	ImGui::Separator();

	ImGui::End();
}

void SimulationComponent::addPointDisturbance(double viewportX, double viewportY)
{
	glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(viewportX, viewportY));
	m_simGrid->addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
}
