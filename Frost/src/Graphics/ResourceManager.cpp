#include "ResourceManager.h"

#include "Debugger.h"

#include "Script/JSONParser.h"

void ResourceManager::AddShader(const std::string& name, Shader* shader)
{
	DEBUG_ASSERT(!name.empty(), "Shader name missing");

	Get().m_shaders[name] = unique_ptr<Shader>(shader);
}

Shader* ResourceManager::GetShader(const std::string& name)
{
	try
	{
		return Get().m_shaders.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("[Res] No such shader: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddTexture(const std::string& name, Texture* tex)
{
	DEBUG_ASSERT(!name.empty(), "Texture name missing");

	Get().m_textures[name] = unique_ptr<Texture>(tex);
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
	try
	{
		return Get().m_textures.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("[Res] No such texture: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddTextureAtlas(const std::string& name, TextureAtlas* tex)
{
	DEBUG_ASSERT(!name.empty(), "Texture atlas name missing");

	Get().m_textureAtlases[name] = unique_ptr<TextureAtlas>(tex);
}

TextureAtlas* ResourceManager::GetTextureAtlas(const std::string& name)
{
	try
	{
		return Get().m_textureAtlases.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("[Res] No such texture atlas: {0}", name);
		return nullptr;
	}
}

void ResourceManager::Load(const std::string& path)
{
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	json root = jsonParseFile(path);

	// shaders 
	if (root.find("shaders") != root.end())
	{
		for (auto&[name, shader] : root.at("shaders").items())
		{
			std::string vert = jsonToString(shader, "vert");
			std::string frag = jsonToString(shader, "frag");

			AddShader(name, new Shader(vert, frag));
		}
	}

	// textures 
	if (root.find("texture") != root.end())
	{
		for (auto&[name, texture] : root.at("texture").items())
		{
			std::string src = jsonToString(texture, "src");

			AddTexture(name, new Texture(src.c_str()));
		}
	}

	// textureAtlases 
	if (root.find("textureAtlas") != root.end())
	{
		for (auto&[name, textureAtlas] : root.at("textureAtlas").items())
		{
			std::string src = jsonToString(textureAtlas, "src");
			uint rows = jsonToInt(textureAtlas, "rows", 1);
			uint columns = jsonToInt(textureAtlas, "columns", 1);

			AddTextureAtlas(name, new TextureAtlas(src.c_str(), rows, columns));
		}
	}

	DEBUG_INFO("[Res] Loaded Resources ({0})", path);
}

void ResourceManager::Clear()
{
	Get().m_shaders.clear();
	Get().m_textures.clear();
	Get().m_textureAtlases.clear();
}
