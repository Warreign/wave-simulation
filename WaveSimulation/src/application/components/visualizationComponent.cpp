#include "visualizationComponent.h"

#include <imgui.h>

VisualizationComponent::VisualizationComponent(const std::string& debugName)
	: Component(debugName)
{
}

void VisualizationComponent::init()
{
}

void VisualizationComponent::destroy()
{
}

void VisualizationComponent::onUpdate()
{
}

void VisualizationComponent::onRender()
{
}

void VisualizationComponent::onRenderGui()
{
	ImGui::Begin("Visualization");
	ImGui::Text("Test");
	ImGui::End();
}
