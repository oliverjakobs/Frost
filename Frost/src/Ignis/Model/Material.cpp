#include "Material.h"

#include "Obelisk/Obelisk.h"

namespace ignis
{
	Material::Material()
	{
		Diffuse = nullptr;
		Normal = nullptr;
		Specular = nullptr;

		Shininess = 0.0f;
	}

	Material::~Material()
	{
		SAFE_DELETE(Diffuse);
		SAFE_DELETE(Normal);
		SAFE_DELETE(Specular);
	}

	void Material::Bind()
	{
		if (Diffuse)
			ignisBindTexture(Diffuse, 0);

		if (Normal)
			ignisBindTexture(Normal, 1);
	}

	void Material::Unbind()
	{
		if (Diffuse)
			ignisUnbindTexture(Diffuse);

		if (Normal)
			ignisUnbindTexture(Normal);
	}
}

