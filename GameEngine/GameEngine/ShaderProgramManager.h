#pragma once
#include <memory>

#include "ShaderProgram.h"
#include "ResourceManager.h"

#define DUPLICATE_SHADER	  0x1		// Shader already loaded in program, status.message
										// returned will have the name of the shader.
#define SHADER_NAME_TAKEN	  0x2
#define LOAD_FAILED			  0x4

struct ShaderManagerLoadStatus
{
	GLushort shaderLoadStatus;
	std::string message;
};

class ShaderProgramManager : public ResourceManager<ShaderProgram>
{
public:
	static ShaderProgramManager* getInstance();
	
	ShaderManagerLoadStatus loadShader(const std::string &shaderName, const std::string &vertexFilePath,
		const std::string &fragmentFilePath, const std::vector<ShaderVariable> &shaderVars);

private:
	static std::unique_ptr<ShaderProgramManager> myInstance;

	ShaderProgramManager() {};
};