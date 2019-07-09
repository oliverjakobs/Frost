#pragma once

#include "Shader.h"
#include "Texture.h"

#include "Utility/Singelton.h"

class ResourceManager : private Singleton<ResourceManager>
{
private:
	std::map<std::string, unique_ptr<Shader>> m_shaders;
	std::map<std::string, unique_ptr<Texture>> m_textures;
	std::map<std::string, unique_ptr<TextureAtlas>> m_textureAtlases;

public:
	static void AddShader(const std::string& name, Shader* shader);
	static Shader* GetShader(const std::string& name);

	static void AddTexture(const std::string& name, Texture* tex);
	static Texture* GetTexture(const std::string& name);

	static void AddTextureAtlas(const std::string& name, TextureAtlas* tex);
	static TextureAtlas* GetTextureAtlas(const std::string& name);

	static void Load(const std::string& path);
};