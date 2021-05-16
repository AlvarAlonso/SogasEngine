
#include <iostream>
#include "core/application.h"

int main()
{

	Application* application = new Application();

	application->create();
	application->run();
	application->shutdown();

	std::cin.get();
	return 0;
}