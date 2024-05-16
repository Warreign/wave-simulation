#pragma once

#include "application/components/component.h"
#include "visualization/shaders/shader.h"
#include "visualization/shaders/waterShader.h"
#include "visualization/objects/skybox.h"
#include "visualization/objects/water.h"
#include "visualization/camera.h"


class VisualizationComponent : public Component
{
public:
	VisualizationComponent(const std::string& debugName, const AmplitudeGrid& simGrid);

	void init() override;
	void destroy() override;

	void onUpdate(float dt) override;
	void onRender() override;
	void onRenderGui() override;

	void setDirection(int value);
	void setAmplitude(float value);

private:

	bool m_isSkyboxVisible = true;
	bool m_isWaterWireframe = false;
	float m_ampMultiplier = 2.0f;

	const size_t m_waterSize = 100;
	const float m_waterScale = 200.0f;

	const AmplitudeGrid& m_simGrid;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<WaterShader> m_waterShader;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Skybox> m_skybox;
	
	std::unique_ptr<WaterMesh> m_waterMesh;
	std::unique_ptr<Water> m_water;
};