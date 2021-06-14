#include "defines.h"

#include "EditorLayer.h"
#include "core/entryPoint.h"
#include "core/application.h"

class Editor : public Application
{
public:
	Editor()
	{
		pushLayer(new EditorLayer());
	}

	~Editor()
	{}
};


Application* createApplication()
{
	return new Editor();
}