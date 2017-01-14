#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include "lodepng.h"

#pragma once

namespace glutil {
	struct FramebufferDesc
	{
		GLuint depthBufferId;
		GLuint renderTextureId;
		GLuint renderFramebufferId;
		GLuint resolveTextureId;
		GLuint resolveFramebufferId;
	};

	GLuint compileShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader);
	GLuint loadTexture(std::string texPath);
	bool createFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc);
}
