#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include <openvr.h>

#include "lodepng.h"
#include "Matrices.h"
#include "thirdparty/openvr-1.0.5/samples/shared/pathtools.h"

namespace geom {
	struct Polygon {
	public:
		Polygon( );
		~Polygon();

		void Polygon::clear();
		void addVertex(Vector2 vertex);
		Vector2 Polygon::getFirstVertex();
		Vector2 Polygon::getSecondVertex();
		Vector2 Polygon::getLastVertex();
		Vector2 Polygon::getSecondToLast();
		void Polygon::updateLastVertex(Vector2 vertex);
		void Polygon::setHeight(float height);

		void Polygon::addCubeVertex( float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float> &vertdata );
		void Polygon::addCubeToScene( Matrix4 mat, std::vector<float> &vertdata );
		void Polygon::renderRoom(std::vector<float> &vertdataarray);
		void Polygon::renderLines(std::vector<float> &vertdataarray);

	private:
		std::vector<Vector2> vertices;
		float height;
	};
}
