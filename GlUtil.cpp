#include "GlUtil.h"

namespace glutil {
	GLuint compileShader(const char *name, const char *vertShader, const char *fragShader) {
		GLuint programId = glCreateProgram();

		GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShaderId, 1, &vertShader, NULL);
		glCompileShader(vertShaderId);

		GLint vertCompiled = GL_FALSE;
		glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != GL_TRUE) {
			printf("%s - Unable to compile vertex shader %d!\n", name, vertShaderId);
			glDeleteProgram(programId);
			glDeleteShader(vertShaderId);
			return 0;
		}
		glAttachShader(programId, vertShaderId);
		glDeleteShader(vertShaderId); // the program hangs onto this once it's attached

		GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShaderId, 1, &fragShader, NULL);
		glCompileShader(fragShaderId);

		GLint fragCompiled = GL_FALSE;
		glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != GL_TRUE) {
			printf("%s - Unable to compile fragment shader %d!\n", name, fragShaderId);
			glDeleteProgram(programId);
			glDeleteShader(fragShaderId);
			return 0;
		}

		glAttachShader(programId, fragShaderId);
		glDeleteShader(fragShaderId); // the program hangs onto this once it's attached

		glLinkProgram(programId);

		GLint programSuccess = GL_TRUE;
		glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE) {
			printf("%s - Error linking program %d!\n", name, programId);
			glDeleteProgram(programId);
			return 0;
		}

		glUseProgram(programId);
		glUseProgram(0);

		return programId;
	}

	GLuint loadTexture(std::string texPath) {
		std::vector<unsigned char> imgBytes;
		unsigned imgWidth, imgHeight;
		unsigned err = lodepng::decode(imgBytes, imgWidth, imgHeight, texPath.c_str());

		if (err != 0)
			return false;

		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, &imgBytes[0]);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texId;
	}

	bool createFrameBuffer(int width, int height, FramebufferDesc &framebufferDesc) {
		glGenFramebuffers(1, &framebufferDesc.renderFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.renderFramebufferId);

		glGenRenderbuffers(1, &framebufferDesc.depthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.depthBufferId);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.depthBufferId);

		glGenTextures(1, &framebufferDesc.renderTextureId);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.renderTextureId);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.renderTextureId, 0);

		glGenFramebuffers(1, &framebufferDesc.resolveFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.resolveFramebufferId);

		glGenTextures(1, &framebufferDesc.resolveTextureId);
		glBindTexture(GL_TEXTURE_2D, framebufferDesc.resolveTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.resolveTextureId, 0);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

}
