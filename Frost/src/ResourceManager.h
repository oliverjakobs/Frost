#pragma once

#include "Types.h"

#include "Ignis/Core/Shader.h"
#include "Ignis/Core/Texture.h"
#include "Ignis/Advanced/TextureAtlas.h"

#include "Obelisk/Singelton.h"

class ResourceManager : private Singleton<ResourceManager>
{
private:
	std::map<std::string, unique_ptr<ignis::Shader>> m_shaders;
	std::map<std::string, unique_ptr<ignis::Texture>> m_textures;
	std::map<std::string, unique_ptr<ignis::TextureAtlas>> m_textureAtlases;

public:
	static void AddShader(const std::string& name, ignis::Shader* shader);
	static ignis::Shader* GetShader(const std::string& name);

	static void AddTexture(const std::string& name, ignis::Texture* tex);
	static ignis::Texture* GetTexture(const std::string& name);

	static void AddTextureAtlas(const std::string& name, ignis::TextureAtlas* tex);
	static ignis::TextureAtlas* GetTextureAtlas(const std::string& name);

	static void Load(const std::string& path);
};