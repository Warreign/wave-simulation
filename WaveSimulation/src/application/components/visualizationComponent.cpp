#include "visualizationComponent.h"

#include "application/application.h"
#include "visualization/meshes/cubeMesh.h"
#include <visualization/objects/cube.h>

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
	ShaderBase::addIncludeFile("shaders/water_macros.glsl");
	ShaderBase::addIncludeFile("shaders/water_common.glsl");
	m_commonShader = std::make_unique<LIghtingShader>("shaders/lighting.vert", "shaders/lighting.frag");
	m_waterShader = std::make_unique<LIghtingShader>("shaders/water.vert", "shaders/water.frag");

	std::cout << "\t Initializing main camera" << std::endl;
	m_camera = std::make_unique<Camera>(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0), 0.1f, 200.0f, glm::radians(80.0f), 30.0f);
	m_camera->makeActive();

	std::cout << "\t Initializing skybox" << std::endl;
	m_skybox = std::make_unique<Skybox>();

	std::cout << "\t Initializing WaterMesh" << std::endl;
	m_waterMesh = std::make_unique<WaterMesh>(m_waterShader.get(), m_waterSize, m_waterScale);

	std::cout << "\t Initializing Water object" << std::endl;
	m_water = std::make_unique<Water>(m_waterMesh.get());

	std::cout << "\t Initializing cube" << std::endl;
	//CubeMesh* cube_mesh = new CubeMesh(m_commonShader.get());
	glm::vec3 cube_position(11, 0, 15);
	float cube_scale = 8;
	m_cube = std::make_unique<Cube>(m_commonShader.get(), glm::translate(glm::mat4(1), cube_position) * glm::scale(glm::mat4(1), glm::vec3(cube_scale)));

	m_simGrid.setDirection(m_defaultDirection);
	m_waterShader->setInteger("u_direction", m_defaultDirection);
	m_simGrid.setDefaultAmp(m_defaultAmplitude);
	m_waterShader->setFloat("u_defaultAmp", m_defaultAmplitude);
	m_simGrid.setWindSpeed(m_windSpeed);

	m_waterShader->setInteger("u_skybox", 10);
}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate(float dt)
{
	m_camera->updateMatrices();

	if (Application::getInstance().getSimComp().isUpdateGird())
	{
	}
#ifndef COMPUTE_SHADER
		m_waterMesh->updateData(m_ampMultiplier, m_simGrid);
		m_waterMesh->setProfileBuffer(m_simGrid.m_profileBuffers[0]);
#else

		m_waterShader->setInteger("u_waterSize", m_waterSize);
		m_waterShader->setFloat("u_waterScale", m_waterScale);
		m_waterShader->setFloat("u_multiplier", m_ampMultiplier);

		int location = m_waterShader->uniformLocation("u_Amps");
		for (int ik = 0; ik < N_K; ++ik)
		{
			m_waterShader->setInteger(location + ik, ik+1);
			glBindTextureUnit(ik+1, m_simGrid.getAmpTexture(ik));
		}

		m_waterShader->setFloat("u_profilePeriod", m_simGrid.m_max.w * m_simGrid.m_periodicity);
		m_waterShader->setInteger("u_profileBuffer", 0);
		glBindTextureUnit(0, m_simGrid.m_profileTexture);

#endif // !COMPUTE_SHADER

	glm::vec2 min = m_simGrid.m_min;
	glm::vec2 max = m_simGrid.m_max;
	m_waterShader->setVec2("u_min", min);
	m_waterShader->setVec2("u_max", max);
}

void VisualizationComponent::onRender()
{

	glBindTextureUnit(10, m_skybox->getTexture());
	if (!m_isWaterWireframe)
	{
		m_water->draw(Camera::get());
	}
	else
	{
		m_water->draw(Camera::get(), GL_LINE);
	}
	if (m_useCubeBoundary)
	{
		m_cube->draw(Camera::get());		
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
	if (ImGui::SliderFloat("Wind Speed", &m_windSpeed, 0.01, 15))
	{
		m_simGrid.setWindSpeed(m_windSpeed);
	}
	if (ImGui::Checkbox("Use Cube Boundary", &m_useCubeBoundary))
	{
		m_simGrid.setDoReflections(m_useCubeBoundary);
	}
	ImGui::Separator();
	ImGui::End();
}
