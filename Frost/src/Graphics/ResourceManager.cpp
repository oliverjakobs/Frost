#include "ResourceManager.h"

#include "Script/JSONParser.h"

#include "Obelisk/Debugger.h"

void ResourceManager::AddShader(const std::string& name, ignis::Shader* shader)
{
	if (!name.empty())
	{
		Get().m_shaders[name] = unique_ptr<ignis::Shader>(shader);
	}
}

ignis::Shader* ResourceManager::GetShader(const std::string& name)
{
	try
	{
		return Get().m_shaders.at(name).get();
	}
	catch (std::out_of_range)
	{
		OBELISK_WARN("[Res] No such shader: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddTexture(const std::string& name, ignis::Texture* tex)
{
	if (!name.empty())
	{
		Get().m_textures[name] = std::unique_ptr<ignis::Texture>(tex);
	}
}

ignis::Texture* ResourceManager::GetTexture(const std::string& name)
{
	try
	{
		return Get().m_textures.at(name).get();
	}
	catch (std::out_of_range)
	{
		OBELISK_WARN("[Res] No such texture: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddTextureAtlas(const std::string& name, ignis::TextureAtlas* tex)
{
	if (!name.empty())
	{
		Get().m_textureAtlases[name] = unique_ptr<ignis::TextureAtlas>(tex);
	}
}

ignis::TextureAtlas* ResourceManager::GetTextureAtlas(const std::string& name)
{
	try
	{
		return Get().m_textureAtlases.at(name).get();
	}
	catch (std::out_of_range)
	{
		OBELISK_WARN("[Res] No such texture atlas: {0}", name);
		return nullptr;
	}
}

void ResourceManager::Load(const std::string& path)
{
	json root = jsonParseFile(path);

	// shaders 
	if (root.find("shaders") != root.end())
	{
		for (auto&[name, shader] : root.at("shaders").items())
		{
			std::string vert = jsonToString(shader, "vert");
			std::string frag = jsonToString(shader, "frag");

			if (vert.empty())
			{
				OBELISK_WARN("[JSON] Could not load Shader: Vertex Shader is missing ({0})", name);
			}
			else if (frag.empty())
			{
				OBELISK_WARN("[JSON] Could not load Shader: Fragment Shader is missing ({0})", name);
			}
			else
			{
				AddShader(name, new ignis::Shader(vert, frag));
			}
		}
	}

	// textures 
	if (root.find("texture") != root.end())
	{
		for (auto&[name, texture] : root.at("texture").items())
		{
			std::string src = jsonToString(texture, "src");

			if (src.empty())
			{
				OBELISK_WARN("[JSON] Could not load Texture: Src is missing ({0})", name);
			}
			else
			{
				AddTexture(name, new ignis::Texture(src));
			}
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

			if (src.empty())
			{
				OBELISK_WARN("[JSON] Could not load Texture Atlas: Src is missing ({0})", name);
			}
			else
			{
				AddTextureAtlas(name, new ignis::TextureAtlas(src, rows, columns));
			}
		}
	}

	OBELISK_INFO("[JSON] Loaded Resources ({0})", path);
}