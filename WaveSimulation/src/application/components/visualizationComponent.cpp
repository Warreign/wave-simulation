#include "visualizationComponent.h"

#include <imgui.h>

#include <iostream>

VisualizationComponent::VisualizationComponent(const std::string& debugName)
	: Component(debugName)
{
}

void VisualizationComponent::init()
{
	std::cout << "INFO: Initializing " << m_debugName << std::endl;

	std::cout << "\t Initializing shaders:" << std::endl;
	m_commonShader = std::make_unique<Shader>("shaders/standard.vert", "shaders/standard.frag");
	m_waterShader = std::make_unique<WaterShader>();

	std::cout << "\t Initializing main camera" << std::endl;
	m_camera = std::make_unique<Camera>(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0), 0.1f, 100.0f, 80.0f, 30.0f);
	m_camera->makeActive();

	std::cout << "\t Initializing skybox" << std::endl;
	m_skybox = std::make_unique<Skybox>();

}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate()
{
	m_camera->updateMatrices();
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
	//ImGui::Text("Skybox Visible");
	ImGui::Checkbox("Skybox Visible", &m_isSkyboxVisible);
	ImGui::End();
}
