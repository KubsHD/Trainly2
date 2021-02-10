#pragma once

#include "../../stdafx.h"


struct MODEL
{
	std::string SIGNATURE = "SMDL";
	uint32_t VERSION = 1;

	std::string name;

	uint32_t meshCount;
	std::vector<MESH> meshes;
};