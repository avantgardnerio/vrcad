#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#pragma once

namespace glutil {
	GLuint compileShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader);
}
