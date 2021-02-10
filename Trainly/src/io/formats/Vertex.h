#pragma once

struct Vertex
{
	struct {
		float x;
		float y;
		float z;
	} position;

	struct {
		float u;
		float v;
	} uv;

	struct {
		float x;
		float y;
		float z;
	} normal;
};