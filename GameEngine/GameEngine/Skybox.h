#pragma once
#include <string>
#include <vector>
#include "Renderable.h"
#include "ShaderVariable.h"

class Texture;
class SingleMesh;
class ShaderProgram;
class ShaderVariable;

class Skybox : public Renderable
{
public:
    Skybox(const std::string &skyboxMeshName, const std::string &textureName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &negativeXTexturePath,
        const std::string &positiveXTexturePath,
        const std::string &negativeYTexturePath,
        const std::string &positiveYTexturePath,
        const std::string &negativeZTexturePath,
        const std::string &positiveZTexturePath,
        bool printShaderLoadStatus = "false");

    void setMeshName(const std::string &meshName);
    void setShaderName(const std::string &shaderName);
    void setTextureName(const std::string &textureName);

    std::string getMeshName() const { return meshName; }
    std::string getShaderName() const { return shaderName; }
    std::string getTextureName() const { return textureName; }

    ShaderProgram* getShader() const;
    SingleMesh* getMesh() const;
    Texture* getTexture() const;

    std::string className() const { return "Skybox"; }

protected:
    std::string meshName;
    std::string shaderName;
    std::string textureName;
};