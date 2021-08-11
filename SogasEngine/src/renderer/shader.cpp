#include "sgspch.h"

#include "shader.h"
#include "core/utils.h"
#include "renderer.h"
#include "platform/OpenGL/openGLShader.h"

namespace Sogas 
{
	std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::s_loadedShaders;
	extern std::vector<std::string> g_assetsPath;

	std::shared_ptr<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}

		SGSASSERT_MSG(false, "Unknown renderer API!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		SGSASSERT_MSG(false, "Unknown renderer API!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::GET(const std::string filename)
	{
		if (!Shader::s_loadedShaders[filename])
		{
			std::string path = findFile(filename, g_assetsPath);
			if (!path.empty()) {
				std::shared_ptr<Shader> shader = Shader::create(path);
				shader->setName(filename);
				s_loadedShaders[filename] = shader;
				return shader;
			}
		}
		return Shader::s_loadedShaders[filename];
	}

	std::shared_ptr<Shader> Shader::GETDefault(std::string name)
	{

		if (Shader::s_loadedShaders[name.c_str()])
		{
			return s_loadedShaders[name.c_str()];
		}

		std::string vs = "";
		std::string fs = "";

		vs = "attribute vec3 a_vertex; attribute vec3 a_normal; attribute vec2 a_uv; attribute vec4 a_color; \
		uniform mat4 u_model;\n\
		uniform mat4 u_viewprojection;\n\
		varying vec3 v_position;\n\
		varying vec3 v_world_position;\n\
		varying vec4 v_color;\n\
		varying vec3 v_normal;\n\
		varying vec2 v_uv;\n\
		void main()\n\
		{\n\
			v_normal = (u_model * vec4(a_normal, 0.0)).xyz;\n\
			v_position = a_vertex;\n\
			v_color = a_color;\n\
			v_world_position = (u_model * vec4(a_vertex, 1.0)).xyz;\n\
			v_uv = a_uv;\n\
			gl_Position = u_viewprojection * vec4(v_world_position, 1.0);\n\
		}";

		if (name == "grid")
		{
			vs = "attribute vec3 a_vertex; attribute vec4 a_color; \
			uniform mat4 u_model;\n\
			uniform mat4 u_viewprojection;\n\
			varying vec3 v_position;\n\
			varying vec3 v_world_position;\n\
			varying vec4 v_color;\n\
			void main()\n\
			{\n\
				v_position = a_vertex;\n\
				v_color = a_color;\n\
				v_world_position = (u_model * vec4(a_vertex, 1.0)).xyz;\n\
				gl_Position = u_viewprojection * vec4(v_world_position, 1.0);\n\
			}";

			fs = "uniform vec4 u_color;\n\
					varying vec4 v_color;\n\
					uniform vec3 u_camera_position;\n\
					varying vec3 v_world_position;\n\
					void main() {\n\
						vec4 color = u_color * v_color;\n\
						color.a *= pow( 1.0 - length(v_world_position.xz - u_camera_position.xz) / 5000.0, 4.5);\n\
						if(v_world_position.x == 0.0)\n\
							color.xyz = vec3(1.0, 0.5, 0.5);\n\
						if(v_world_position.z == 0.0)\n\
							color.xyz = vec3(0.5, 0.5, 1.0);\n\
						vec3 E = normalize(v_world_position - u_camera_position);\n\
						gl_FragColor = color;\n\
					}";
		}

		std::shared_ptr<Shader> sh = Shader::create(vs, fs);
		if (!sh)
		{
			SGSASSERT_MSG(false, "Unable to compile default shader.")
		}

		s_loadedShaders[name.c_str()] = sh;

		sh->bind();
		sh->setUniform("u_color", glm::vec4(1.0f));
		sh->unbind();

		return sh;
	}
}