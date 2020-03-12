#pragma once

#include "Ignis/Core/Texture.h"

namespace ignis
{
	struct Material
	{
		ignis_texture* Diffuse;
		ignis_texture* Normal;
		ignis_texture* Specular;

		float Shininess;

		Material();
		~Material();

		void Bind();
		void Unbind();
	};
}