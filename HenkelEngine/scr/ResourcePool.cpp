#include "ResourcePool.h"
#include <opengl\openglHelper.h>

ResourcePool::ResourcePool() : m_shaders(), m_textures()
{
}

ResourcePool::~ResourcePool()
{
	m_shaders.clear();
	m_textures.clear();
}

void ResourcePool::CreateShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	std::string key = vertexShaderFilePath + fragmentShaderFilePath;
	auto it = m_shaders.find(key);
	if (it == m_shaders.end())
		m_shaders[key] = RefCounter<Shader>{ 1u, std::make_unique<Shader>(vertexShaderFilePath, fragmentShaderFilePath) };
	else
		it->second.count++;
}

Shader* ResourcePool::RetriveShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	std::string key = vertexShaderFilePath + fragmentShaderFilePath;
	auto it = m_shaders.find(key);
	if (it == m_shaders.end())
	{
		ASSERT(true);
		return nullptr;
	}
	return it->second.reference.get();
}

void ResourcePool::ReleaseShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	std::string key = vertexShaderFilePath + fragmentShaderFilePath;
	auto it = m_shaders.find(key);
	if (it == m_shaders.end()) return;
	it->second.count--;
	if (it->second.count <= 0)
		m_shaders.erase(it);
}

void ResourcePool::CreateTexture(const std::string& path)
{
	std::string key = path;
	auto it = m_textures.find(key);
	if (it == m_textures.end())
		m_textures[key] = RefCounter<Texture>{ 1u, std::make_unique<Texture>(path) };
	else
		it->second.count++;
}

Texture* ResourcePool::RetriveTexture(const std::string& path)
{
	std::string key = path;
	auto it = m_textures.find(key);
	if (it == m_textures.end())
	{
		ASSERT(true);
		return nullptr;
	}
	return it->second.reference.get();
}

void ResourcePool::ReleaseTexture(const std::string& path)
{
	std::string key = path;
	auto it = m_textures.find(key);
	if (it == m_textures.end()) return;
	it->second.count--;
	if (it->second.count <= 0)
		m_textures.erase(it);
}
