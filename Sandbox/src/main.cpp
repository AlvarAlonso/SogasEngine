
#include <iostream>
#include "sogas.h"

class Sandbox : public Application
{
public:
	Sandbox()
	{
		create();
	}

	~Sandbox()
	{

	}
};

Application* createApplication()
{
	return new Sandbox();
}