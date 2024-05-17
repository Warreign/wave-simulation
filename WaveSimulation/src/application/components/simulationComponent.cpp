#include "simulationComponent.h"

#include "application/application.h"
#include "visualization/camera.h"

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
		100.0f, //size
		0.01f, // min wave length
		10.0f, // max wave length
		10.0f, // wind speed
		128, // number of spatial disc. nodes
		16, // number of wave angle disc. nodes
		1 // number of wave length disc. nodes
	);

	m_simGrid->setDirection(m_defaultDirection);
	m_simGrid->setDefaultAmp(m_defaultAmplitude);
	m_simGrid->setWindSpeed(m_windSpeed);


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
	
	//glfwSetMouseButtonCallback(window.getHandle(), [](GLFWwindow* w, int button, int action, int mods)
	//	{
	//		const auto& io = ImGui::GetIO();
	//		if (io.WantCaptureMouse) return;

	//		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//		{
	//			double x, y;
	//			glfwGetCursorPos(w, &x, &y);
	//			AmplitudeGrid& simGrid = Application::getInstance().getSimComp().getGrid();
	//			glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(x, y));
	//			simGrid.addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
	//		}
	//	});
}

void SimulationComponent::destroy()
{
}

void SimulationComponent::onUpdate(float dt)
{
	float dtt = dt;
	double cfl_dt = m_simGrid->cflTimeStep(dt ,m_timeMultiplier);
	m_dtLast = cfl_dt;

	if (m_isUpdateGrid)
	{
		m_simGrid->timeStep(cfl_dt);
	}
}

void SimulationComponent::onRenderGui()
{
	ImGui::Begin("Simulation");

	ImGui::Checkbox("Update Grid", &m_isUpdateGrid);
	ImGui::SliderFloat("Time Multiplier", &m_timeMultiplier, -2.0f, 2.0f);
	ImGui::InputFloat("dt", &m_dtLast, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::Separator();

	Application& app = Application::getInstance();
	VisualizationComponent& visualComp = app.getVisualComp();
	if (ImGui::SliderInt("Direction", &m_defaultDirection, 0, 15))
	{
		m_simGrid->setDirection(m_defaultDirection);
		visualComp.setDirection(m_defaultDirection);
	}
	if (ImGui::SliderFloat("Amplitude", &m_defaultAmplitude, 0.0f, 1.0f))
	{
		m_simGrid->setDefaultAmp(m_defaultAmplitude);
		visualComp.setAmplitude(m_defaultAmplitude);
	}
	if (ImGui::SliderFloat("Wind Speed", &m_windSpeed, 0.5, 10))
	{
		m_simGrid->setWindSpeed(m_windSpeed);
	}
	
	ImGui::Separator();

	ImGui::End();
}

void SimulationComponent::addPointDisturbance(double viewportX, double viewportY)
{
	glm::vec3 planePoint = Camera::get().intersectPlane(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(viewportX, viewportY));
	m_simGrid->addPointDisturbance(glm::vec2(planePoint.x, planePoint.z), 0.1);
}
