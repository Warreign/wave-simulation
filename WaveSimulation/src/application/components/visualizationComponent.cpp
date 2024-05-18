#include "visualizationComponent.h"

#include "application/application.h"

#include <imgui.h>

#include <iostream>

VisualizationComponent::VisualizationComponent(const std::string& debugName, AmplitudeGrid& simGrid)
	: Component(debugName), m_simGrid(simGrid)
{
}

void VisualizationComponent::init()
{
	std::cout << "INFO: Initializing " << m_debugName << std::endl;

	std::cout << "\t Initializing shaders:" << std::endl;
	m_commonShader = std::make_unique<Shader>("shaders/standard.vert", "shaders/standard.frag");
	m_waterShader = std::make_unique<WaterShader>("shaders/water.vert", "shaders/water.frag", "shaders/water_macros.glsl", "shaders/water_common.glsl");

	std::cout << "\t Initializing main camera" << std::endl;
	m_camera = std::make_unique<Camera>(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0), 0.1f, 200.0f, glm::radians(80.0f), 30.0f);
	m_camera->makeActive();

	std::cout << "\t Initializing skybox" << std::endl;
	m_skybox = std::make_unique<Skybox>();

	std::cout << "\t Initializing WaterMesh" << std::endl;
	m_waterMesh = std::make_unique<WaterMesh>(m_waterShader.get(), m_waterSize, m_waterScale);

	std::cout << "\t Initializing Water object" << std::endl;
	m_water = std::make_unique<Water>(m_waterMesh.get());

	m_simGrid.setDirection(m_defaultDirection);
	m_waterShader->setInteger("u_direction", m_defaultDirection);
	m_simGrid.setDefaultAmp(m_defaultAmplitude);
	m_waterShader->setFloat("u_defaultAmp", m_defaultAmplitude);
	m_simGrid.setWindSpeed(m_windSpeed);
}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate(float dt)
{
	m_camera->updateMatrices();

	if (Application::getInstance().getSimComp().isUpdateGird())
	{
#ifndef COMPUTE_SHADER
		m_waterMesh->updateData(m_ampMultiplier, m_simGrid);
		m_waterMesh->setProfileBuffer(m_simGrid.m_profileBuffers[0]);
#else
		m_waterShader->setInteger("u_waterSize", m_waterSize);
		m_waterShader->setFloat("u_waterScale", m_waterScale);
		m_waterShader->setFloat("u_multiplier", m_ampMultiplier);
		m_waterShader->setInteger("u_Amplitude", 1);
		glBindTextureUnit(1, m_simGrid.getAmpTexture());

		m_waterShader->setFloat("profilePeriod", m_simGrid.m_profileBuffers[0].period);
		m_waterShader->setInteger("profileBuffer", 0);
		glBindTextureUnit(0, m_simGrid.m_profileBuffers[0].getTexture());
#endif // !COMPUTE_SHADER
	}

	glm::vec2 min = m_simGrid.m_min;
	glm::vec2 max = m_simGrid.m_max;
	m_waterShader->setVec2("u_min", min);
	m_waterShader->setVec2("u_max", max);
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
	ImGui::SliderFloat("Amplitude Multiplier", &m_ampMultiplier, 0.1f, 3.0f);
	ImGui::End();

	ImGui::Begin("Camera Settings");
	Camera& cam = Camera::get();
	//ImGui::DragFloat("Speed", &cam.speed, 5.0, 0.0);
	ImGui::SliderFloat("Height", &cam.position.y, 5.0f, 100.0f);
	if (ImGui::SliderAngle("Veiw yaw angle", &m_cameraYaw))
		cam.rotateViewAbs(glm::degrees(m_cameraYaw), glm::degrees(m_cameraPitch));
	if (ImGui::SliderAngle("View pitch angle", &m_cameraPitch, -90.0f, 90.0f))
		cam.rotateViewAbs(glm::degrees(m_cameraYaw), glm::degrees(m_cameraPitch));
	ImGui::SliderAngle("FOV", &cam.fovAngle, 10.0f, 179.0f);
	//ImGui::DragFloat("Sensitivity", &cam.sensitivity, 0.1f, 3.0f, 0.1f);
	ImGui::DragFloat("Far Plane", &cam.farPlane, 10.0f, 1.0f);
	//if (ImGui::Button("Toggle Free Mode")) cam.toggleFreeMode();
	ImGui::End();

	ImGui::Begin("Simulation #2");
	if (ImGui::SliderInt("Direction", &m_defaultDirection, 0, N_THETA-1))
	{
		m_simGrid.setDirection(m_defaultDirection);
		m_waterShader->setInteger("u_direction", m_defaultDirection);
	}
	if (ImGui::SliderFloat("Amplitude", &m_defaultAmplitude, 0.0f, 1.0f))
	{
		m_simGrid.setDefaultAmp(m_defaultAmplitude);
		m_waterShader->setFloat("u_defaultAmp", m_defaultAmplitude);
	}
	if (ImGui::SliderFloat("Wind Speed", &m_windSpeed, 0.5, 10))
	{
		m_simGrid.setWindSpeed(m_windSpeed);
	}
	ImGui::Separator();
	ImGui::End();
}
