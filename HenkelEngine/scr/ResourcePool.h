#pragma once

#include <unordered_map>
#include <string>
#include <tuple>
#include "opengl/Shader.h"
#include "opengl\Texture.h"
#include <memory>

class ResourcePool
{
public:
	template <typename T>
	struct RefCounter {
		unsigned int count;
		std::unique_ptr<T> reference;
	};

	ResourcePool();
	~ResourcePool();

	void CreateShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	Shader* RetrieveShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void ReleaseShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void CreateTexture(const std::string& path);
	Texture* RetrieveTexture(const std::string& path);
	void ReleaseTexture(const std::string& path);

private:

	std::unordered_map<std::string, RefCounter<Shader>> m_shaders;
	std::unordered_map<std::string, RefCounter<Texture>> m_textures;

};

