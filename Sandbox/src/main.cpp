
#include <iostream>
#include "defines.h"
#include "sogas.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void onUpdate() override
	{
	}

	void onEvent(Event& event) override
	{
		SGSINFO("Event in example layer: %s", event.toString().c_str());
	}
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Application* createApplication()
{
	return new Sandbox();
}