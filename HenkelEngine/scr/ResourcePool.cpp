#include "ResourcePool.h"
#include <opengl\openglHelper.h>
#include <format>

ResourcePool::ResourcePool() : m_shaders(), m_textures()
{
}

ResourcePool::~ResourcePool()
{
	m_textures.clear();
	m_shaders.clear();
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

Shader* ResourcePool::RetrieveShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
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
		if (path.compare("blank") == 0) // TODO Think of a better way to handle blank textures
			m_textures[key] = RefCounter<Texture>{ 1u, std::make_unique<Texture>() };
		else
			m_textures[key] = RefCounter<Texture>{ 1u, std::make_unique<Texture>(path) };
	else
		it->second.count++;
}

Texture* ResourcePool::RetrieveTexture(const std::string& path)
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

void ResourcePool::CreateFont(const std::string& path, int size)
{
	std::string key = std::format("{} {}", path, size);
	auto it = m_fonts.find(key);
	if (it == m_fonts.end())
	{
		unsigned int rendererID;
		int width, height;
		m_fonts[key] = RefCounter<Font>{ 1u, std::make_unique<Font>(path, size, rendererID, width, height) };
		CreateFontTexture(key, rendererID, width, height);
	}
	else
		it->second.count++;
}

Font* ResourcePool::RetrieveFont(const std::string& path, int size)
{
	std::string key = std::format("{} {}", path, size);
	auto it = m_fonts.find(key);
	if (it == m_fonts.end())
	{
		ASSERT(true);
		return nullptr;
	}
	return it->second.reference.get();
}

void ResourcePool::ReleaseFont(const std::string& path, int size)
{
	std::string key = std::format("{} {}", path, size);
	auto it = m_fonts.find(key);
	if (it == m_fonts.end()) return;
	it->second.count--;
	if (it->second.count <= 0)
		m_fonts.erase(it);
}

void ResourcePool::CreateFontTexture(const std::string& key, unsigned int rendererID, int width, int height)
{
	auto it = m_textures.find(key);
	ASSERT(it == m_textures.end());
	m_textures[key] = RefCounter<Texture>{ 1u, std::make_unique<Texture>(rendererID, width, height) };
}
