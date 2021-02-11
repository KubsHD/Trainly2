#pragma once

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

class Material
{
public:

	Material();

	void Create(const char* materialDescriptorData);

private:
	PixelShader m_pix;
	VertexShader m_vert;
};