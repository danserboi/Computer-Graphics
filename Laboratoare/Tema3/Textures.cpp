#include "Tema3.h"

#include <Core/Engine.h>

using namespace std;

void Tema3::InitTextures() {
	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "concrete.png").c_str(), GL_REPEAT);
		mapTextures["concrete"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sphere_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["sphere_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "collectable_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["collectable_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "metal_core_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["metal_core_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "tetrahedron_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["tetrahedron_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "pyramid_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["decorative_pyramid_texture"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "cylinder_texture.jpg").c_str(), GL_REPEAT);
		mapTextures["decorative_cylinder_texture"] = texture;
	}
}
