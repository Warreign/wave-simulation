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
	VisualizationComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onUpdate() override;
	void onRender() override;
	void onRenderGui() override;

private:

	bool m_isSkyboxVisible = true;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<WaterShader> m_waterShader;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Skybox> m_skybox;
};