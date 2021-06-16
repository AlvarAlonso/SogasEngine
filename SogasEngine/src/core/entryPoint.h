#pragma once
#include "core/application.h"

extern Sogas::Application* Sogas::createApplication();

int main(int argc, char** argv)
{
	auto app = Sogas::createApplication();
	app->run();
	delete app;
}