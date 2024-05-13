#include "simulationComponent.h"

#include <imgui.h>

SimulationComponent::SimulationComponent(const std::string& debugName)
	: Component(debugName)
{
}

void SimulationComponent::init()
{
	m_simGrid = std::make_unique<AmplitudeGrid>(
		100.0f, //size
		0.01f, // min wave length
		10.0f, // max wave length
		10.0f, // wind speed
		128, // number of spatial disc. nodes
		16, // number of wave angle disc. nodes
		1 // number of wave length disc. nodes
	);
}

void SimulationComponent::destroy()
{
}

void SimulationComponent::onUpdate()
{
	double dt = m_simGrid->cflTimeStep() * pow(10, -0.9);

	if (m_isUpdateGrid)
	{
		m_simGrid->timeStep(dt);
	}
}

void SimulationComponent::onRenderGui()
{
	ImGui::Begin("Simulation");
	ImGui::Checkbox("Update Grid", &m_isUpdateGrid);
	ImGui::SliderFloat("Time Multiplier", &m_timeMultiplier, -2.0f, 2.0f);
	ImGui::End();
}
