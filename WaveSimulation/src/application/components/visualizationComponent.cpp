#include "visualizationComponent.h"

#include <imgui.h>

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
