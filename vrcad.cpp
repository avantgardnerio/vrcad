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

#include "stdafx.h"
#include "App.h"

int main(int argc, char *argv[]) {
	App *app = new App(argc, argv);

	if (!app->init()) {
		app->Shutdown();
		return 1;
	}

	app->RunMainLoop();

	app->Shutdown();

	return 0;
}
