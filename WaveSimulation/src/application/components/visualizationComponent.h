/**
 * @file visualizationComponent.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "application/components/component.h"
#include "visualization/shaders/shader.h"
#include "visualization/shaders/lightingShader.h"
#include "visualization/objects/skybox.h"
#include "visualization/objects/water.h"
#include "visualization/camera.h"


class VisualizationComponent : public Component
{
public:

	/*
	* Component doing all the rendering
	* 
	* @param simGrid refernece to the simulation grid to render water surface
	*/
	VisualizationComponent(const std::string& debugName, AmplitudeGrid& simGrid);

	void init() override;
	void destroy() override;

	void onUpdate(float dt) override;
	void onRender() override;
	void onRenderGui() override;

private:

	// internal state variables to  update from the gui
	bool m_isSkyboxVisible = true;
	bool m_isWaterWireframe = false;
	bool m_useCubeBoundary = false;
	float m_ampMultiplier = 2.0f;
	float m_defaultAmplitude = 0.15f;
	int m_defaultDirection = 2;
	float m_windSpeed = 8.0f;
	float m_cameraYaw = glm::radians(0.0f); // Radians
	float m_cameraPitch = glm::radians(-90.0f); // Radians


	// water mesh parameter constants
	const size_t m_waterSize = 200;
	const float m_waterScale = 200.0f;

	AmplitudeGrid& m_simGrid;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<LIghtingShader> m_waterShader;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Skybox> m_skybox;
	
	std::unique_ptr<WaterMesh> m_waterMesh;
	std::unique_ptr<Water> m_water;

	std::unique_ptr<ObjectInstance> m_cube;


	friend class SimulationComponent;
};