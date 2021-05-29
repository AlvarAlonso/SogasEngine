#pragma once
#include "core/application.h"

extern Application* createApplication();

int main(int argc, char** argv)
{
	auto app = createApplication();
	app->run();
	delete app;
}