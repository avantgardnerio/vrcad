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

}
