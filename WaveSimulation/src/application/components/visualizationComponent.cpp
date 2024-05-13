#include "visualizationComponent.h"

#include <imgui.h>

#include <iostream>

VisualizationComponent::VisualizationComponent(const std::string& debugName, const AmplitudeGrid& simGrid)
	: Component(debugName), m_simGrid(simGrid)
{
}

void VisualizationComponent::init()
{
	std::cout << "INFO: Initializing " << m_debugName << std::endl;

	std::cout << "\t Initializing shaders:" << std::endl;
	m_commonShader = std::make_unique<Shader>("shaders/standard.vert", "shaders/standard.frag");
	m_waterShader = std::make_unique<WaterShader>();

	std::cout << "\t Initializing main camera" << std::endl;
	m_camera = std::make_unique<Camera>(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0), 0.1f, 100.0f, glm::radians(80.0f), 30.0f);
	m_camera->makeActive();

	std::cout << "\t Initializing skybox" << std::endl;
	m_skybox = std::make_unique<Skybox>();

	std::cout << "\t Initializing WaterMesh" << std::endl;
	m_waterMesh = std::make_unique<WaterMesh>(m_waterShader, 100, 100.0f);

	std::cout << "\t Initializing Water object" << std::endl;
	m_water = std::make_unique<Water>(m_waterMesh);
}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate()
{
	m_camera->updateMatrices();

	m_waterMesh->updateData(m_ampMultiplier, m_simGrid);
	m_waterMesh->setProfileBuffer(m_simGrid.profileBuffers[0]);
}

void VisualizationComponent::onRender()
{
	if (m_isSkyboxVisible)
	{
		m_skybox->draw(Camera::get());
	}
}

void VisualizationComponent::onRenderGui()
{
	ImGui::Begin("Visualization");
	ImGui::Checkbox("Skybox Visible", &m_isSkyboxVisible);
	ImGui::SliderFloat("Amplitude Multiplier", &m_ampMultiplier, 0.1f, 5.0f);
	ImGui::End();
}
