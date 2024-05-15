#include "visualizationComponent.h"

#include "application/application.h"

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
	m_waterMesh = std::make_unique<WaterMesh>(m_waterShader.get(), m_waterSize, m_waterScale);

	std::cout << "\t Initializing Water object" << std::endl;
	m_water = std::make_unique<Water>(m_waterMesh.get());
}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate()
{
	m_camera->updateMatrices();

	if (Application::getInstance().getSimComp().isUpdateGird())
	{
		m_waterShader->setInteger("u_Amplitude", 1);
		glBindTextureUnit(1, m_simGrid.getAmpTexture());

		m_waterMesh->updateData(m_ampMultiplier, m_simGrid);
		m_waterMesh->setProfileBuffer(m_simGrid.m_profileBuffers[0]);
	}
}

void VisualizationComponent::onRender()
{
	if (!m_isWaterWireframe)
	{
		m_water->draw(Camera::get());
	}
	else
	{
		m_water->draw(Camera::get(), GL_LINE);
	}

	if (m_isSkyboxVisible)
	{
		m_skybox->draw(Camera::get());
	}
}

void VisualizationComponent::onRenderGui()
{
	ImGui::Begin("Visualization");
	ImGui::Checkbox("Skybox Visible", &m_isSkyboxVisible);
	ImGui::Checkbox("Wireframe Water", &m_isWaterWireframe);
	ImGui::SliderFloat("Amplitude Multiplier", &m_ampMultiplier, 0.1f, 5.0f);
	ImGui::End();

	ImGui::Begin("Camera Settings");
	Camera& cam = Camera::get();
	//ImGui::DragFloat("Speed", &cam.speed, 5.0, 0.0);
	ImGui::SliderAngle("FOV", &cam.fovAngle, 10.0f, 179.0f);
	//ImGui::DragFloat("Sensitivity", &cam.sensitivity, 0.1f, 3.0f, 0.1f);
	ImGui::DragFloat("Far Plane", &cam.farPlane, 10.0f, 1.0f);
	//if (ImGui::Button("Toggle Free Mode")) cam.toggleFreeMode();
	ImGui::End();
}
