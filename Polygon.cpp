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

#include "Polygon.h"

namespace geom {

	Polygon::Polygon( ) {
		height = 0.0f;
	}

	Polygon::~Polygon()	{
	}

	void Polygon::clear() {
		vertices.clear();
	}

	void Polygon::addVertex(Vector2 vertex)	{
		vertices.push_back(vertex);
	}

	int Polygon::getVertexCount() {
		return vertices.size();
	}
	
	Vector2 Polygon::getFirstVertex() {
		return vertices.front(); // TODO: Size == 0?
	}

	Vector2 Polygon::getSecondVertex() {
		return vertices.at(1 % vertices.size()); // TODO: Size == 0?
	}

	Vector2 Polygon::getLastVertex() {
		return vertices.back(); // TODO: Size == 0?
	}

	Vector2 Polygon::getSecondToLast() {
		return vertices.at(vertices.size() - 2); // TODO: Size == 0?
	}

	Vector2 Polygon::getThirdToLast() {
		return vertices.at(vertices.size() - 3); // TODO: Size == 0?
	}

	void Polygon::setHeight(float h) {
		height = h;
	}

	void Polygon::updateLastVertex(Vector2 vertex) {
		vertices.pop_back();
		vertices.push_back(vertex);
	}

	void Polygon::renderRoom(std::vector<float> &vertdataarray) {
		float u2 = 10;
		float v2 = 24;

		for (unsigned i=0; i<vertices.size(); i++) {
			Vector2 start = vertices.at(i);
			Vector2 end = vertices.at((i+1) % vertices.size());

			addCubeVertex( start.x, height, start.y,	0,	v2, vertdataarray );
			addCubeVertex( end.x,	height, end.y,		u2, v2, vertdataarray );
			addCubeVertex( end.x,	0,		end.y,		u2, 0,	vertdataarray );

			addCubeVertex( end.x,	0,		end.y,		u2, 0,	vertdataarray );
			addCubeVertex( start.x, 0,		start.y,	0,	0,	vertdataarray );
			addCubeVertex( start.x, height, start.y,	0,	v2, vertdataarray );
		}
	}

	void Polygon::renderLines(std::vector<float> &vertdataarray) {
		Vector3 color( 1, 1, 1 );
		for (unsigned i=0; i<vertices.size()-1; i++) {
			Vector2 start = vertices.at(i);
			Vector2 end = vertices.at((i+1) % vertices.size());
			vertdataarray.push_back( start.x );vertdataarray.push_back( height );vertdataarray.push_back( start.y );
			vertdataarray.push_back( color.x );vertdataarray.push_back( color.y );vertdataarray.push_back( color.z );

			vertdataarray.push_back( end.x );vertdataarray.push_back( height );vertdataarray.push_back( end.y );
			vertdataarray.push_back( color.x );vertdataarray.push_back( color.y );vertdataarray.push_back( color.z );
		}
	}

	void Polygon::addCubeVertex( float x, float y, float z, float u, float v, std::vector<float> &vertdata ) {
		vertdata.push_back( x );
		vertdata.push_back( y );
		vertdata.push_back( z );
		vertdata.push_back( u );
		vertdata.push_back( v );
	}

	void Polygon::addCubeToScene( Matrix4 mat, std::vector<float> &vertdata )
	{
		// Matrix4 mat( outermat.data() );

		float u2 = 10;
		float v2 = 24;

		Vector4 A = mat * Vector4( 0, 0, 0, 1 );
		Vector4 B = mat * Vector4( 1, 0, 0, 1 );
		Vector4 C = mat * Vector4( 1, 1, 0, 1 );
		Vector4 D = mat * Vector4( 0, 1, 0, 1 );
		Vector4 E = mat * Vector4( 0, 0, 1, 1 );
		Vector4 F = mat * Vector4( 1, 0, 1, 1 );
		Vector4 G = mat * Vector4( 1, 1, 1, 1 );
		Vector4 H = mat * Vector4( 0, 1, 1, 1 );

		// triangles instead of quads
		addCubeVertex( E.x, E.y, E.z, 0, v2, vertdata ); //Front
		addCubeVertex( F.x, F.y, F.z, u2, v2, vertdata );
		addCubeVertex( G.x, G.y, G.z, u2, 0, vertdata );
		addCubeVertex( G.x, G.y, G.z, u2, 0, vertdata );
		addCubeVertex( H.x, H.y, H.z, 0, 0, vertdata );
		addCubeVertex( E.x, E.y, E.z, 0, v2, vertdata );

		addCubeVertex( B.x, B.y, B.z, 0, v2, vertdata ); //Back
		addCubeVertex( A.x, A.y, A.z, u2, v2, vertdata );
		addCubeVertex( D.x, D.y, D.z, u2, 0, vertdata );
		addCubeVertex( D.x, D.y, D.z, u2, 0, vertdata );
		addCubeVertex( C.x, C.y, C.z, 0, 0, vertdata );
		addCubeVertex( B.x, B.y, B.z, 0, v2, vertdata );

		addCubeVertex( H.x, H.y, H.z, 0, v2, vertdata ); //Top
		addCubeVertex( G.x, G.y, G.z, u2, v2, vertdata );
		addCubeVertex( C.x, C.y, C.z, u2, 0, vertdata );
		addCubeVertex( C.x, C.y, C.z, u2, 0, vertdata );
		addCubeVertex( D.x, D.y, D.z, 0, 0, vertdata );
		addCubeVertex( H.x, H.y, H.z, 0, v2, vertdata );

		addCubeVertex( A.x, A.y, A.z, 0, v2, vertdata ); //Bottom
		addCubeVertex( B.x, B.y, B.z, u2, v2, vertdata );
		addCubeVertex( F.x, F.y, F.z, u2, 0, vertdata );
		addCubeVertex( F.x, F.y, F.z, u2, 0, vertdata );
		addCubeVertex( E.x, E.y, E.z, 0, 0, vertdata );
		addCubeVertex( A.x, A.y, A.z, 0, v2, vertdata );

		addCubeVertex( A.x, A.y, A.z, 0, v2, vertdata ); //Left
		addCubeVertex( E.x, E.y, E.z, u2, v2, vertdata );
		addCubeVertex( H.x, H.y, H.z, u2, 0, vertdata );
		addCubeVertex( H.x, H.y, H.z, u2, 0, vertdata );
		addCubeVertex( D.x, D.y, D.z, 0, 0, vertdata );
		addCubeVertex( A.x, A.y, A.z, 0, v2, vertdata );

		addCubeVertex( F.x, F.y, F.z, 0, 1, vertdata ); //Right
		addCubeVertex( B.x, B.y, B.z, 1, 1, vertdata );
		addCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
		addCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
		addCubeVertex( G.x, G.y, G.z, 0, 0, vertdata );
		addCubeVertex( F.x, F.y, F.z, 0, 1, vertdata );
	}

}
