#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include <openvr.h>

#include "thirdparty/openvr-1.0.5/samples/shared/lodepng.h"
#include "thirdparty/openvr-1.0.5/samples/shared/Matrices.h"
#include "thirdparty/openvr-1.0.5/samples/shared/pathtools.h"

namespace net_squarelabs
{
	struct Polygon
	{
	public:
		Polygon( );
		~Polygon();

		void addVertex(Vector3 vertex);
		Vector3 Polygon::getFirstVertex();
		void Polygon::updateLastVertex(Vector3 vertex);
		void Polygon::setHeight(float height);

		void Polygon::addCubeVertex( float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float> &vertdata );
		void Polygon::addCubeToScene( Matrix4 mat, std::vector<float> &vertdata );
		void Polygon::addToVb(std::vector<float> &vertdataarray);
		void Polygon::addToVb2(std::vector<float> &vertdataarray);

	private:
		std::vector<Vector3> vertices;
		float height;
	};
}
