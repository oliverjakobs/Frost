#pragma once

#include "Font/Font.h"

#include "Utility/Utils.h"
#include "Patterns/Singelton.h"

class ResourceManager : private Singleton<ResourceManager>
{
private:
	std::map<std::string, unique_ptr<Shader>> m_shaders;
	std::map<std::string, unique_ptr<Image>> m_images;
	std::map<std::string, unique_ptr<Font>> m_fonts;

public:
	// TODO: Rescource creation only through here
	// TODO: copy resources or dont change res for each referenze (i.e. remove setRenderFlip)
	static void AddShader(const std::string& name, Shader* shader);
	static Shader* GetShader(const std::string& name);

	static void AddImage(const std::string& name, Image* image);
	static Image* GetImage(const std::string& name);

	static void AddFont(const std::string& name, Font* font);
	static Font* GetFont(const std::string& name);

	static void Load(const std::string& path);
};