#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <map>

#include <openvr.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <glm.hpp>

#include "lodepng.h"
#include "GlUtil.h"
#include "Matrices.h"
#include "thirdparty/openvr-1.0.5/samples/shared/pathtools.h"

#include "CGLRenderModel.h"
#include "Polygon.h"

class App {
private:
	struct VertexDataScene
	{
		Vector3 position;
		Vector2 texCoord;
	};

	struct VertexDataWindow
	{
		Vector2 position;
		Vector2 texCoord;

		VertexDataWindow(const Vector2 & pos, const Vector2 tex) : position(pos), texCoord(tex) {	}
	};

	struct Character {
		GLuint     textureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters;

public:
	App( int argc, char *argv[] );
	virtual ~App();

	// lifecycle
	bool init();
	bool initGl();
	void App::initFonts();
	void initDeviceModels();
	bool loadTextures();
	bool setupHmdRenderTargets();
	void setupMonitorWindow();
	void setupCameraMatrices();
	void shutdown();

	// rendering
	void regenVB();
	void mainLoop();
	void renderFrame();
	void renderControllerAxes();
	void renderToHmd();
	void renderToMonitorWindow();
	void renderToEye(vr::Hmd_Eye nEye);
	void App::renderText(GLuint s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	// input
	bool handleInput();
	void App::triggerPressed(vr::TrackedDeviceIndex_t deviceIdx, float t, Vector2 isec2d, Vector3 laserOrigin);
	Vector2 App::getSnap(Vector3 target);

	// GL util
	GLuint loadShader(std::string name);
	bool createShaders();

	// OpenVR Util
	CGLRenderModel *getDeviceModel(const char *pchRenderModelName);
	std::string getDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL);
	void initDeviceModel(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
	Matrix4 getEyeProj(vr::Hmd_Eye nEye);
	Matrix4 getEyePos(vr::Hmd_Eye nEye);
	Matrix4 getEyeProjMat(vr::Hmd_Eye nEye);
	Matrix4 steamMatToMatrix4(const vr::HmdMatrix34_t &matPose);
	void updateHmdPose();

	// utility
	void sleep( unsigned long nMilliseconds );
	const char *App::byteToBinary(int x);
	std::string measure(float length);

private:

	// ---- me
	enum EditMode {
		none,
		draw,
		extrude
	};

	EditMode mode;
	vr::TrackedDeviceIndex_t currentController;
	geom::Polygon currentPolygon;
	std::vector<geom::Polygon> polygons;
	bool buttonPressed;

	std::string text;
	Matrix4 textPos;

	const uint64_t BTN_GRIP = 4;
	const uint64_t BTN_TRIGGER = 0x200000000;

	Vector3 gripLeft;
	Vector3 gripRight;
	Matrix4 gripWorld;
	Matrix4 gripInverse;

	// ---- ???

	vr::IVRSystem *hmd;
	vr::TrackedDevicePose_t devicePose[ vr::k_unMaxTrackedDeviceCount ];
	Matrix4 devicePoseMat[ vr::k_unMaxTrackedDeviceCount ];

private: // SDL bookkeeping
	SDL_Window *monitorWindow;
	uint32_t monitorWinWidth;
	uint32_t monitorWinHeight;
	SDL_GLContext monitorGlContext;

private: // OpenGL bookkeeping
	std::string poseClasses;								// what classes we saw poses for this frame TODO: Kill this
	char classForDeviceIdx[ vr::k_unMaxTrackedDeviceCount ];   // for each device, a character representing its class

	GLuint brickTextureId;

	unsigned int sceneVertCount;

	GLuint sceneVertBuffer;
	GLuint sceneVertexAr;
	GLuint monitorWinVertAr;
	GLuint monitorWinVertBuff;
	GLuint monitorWinIdxBuff;
	unsigned int monitorWinIdxSize;

	GLuint controllerVertBuffer;
	GLuint controllerVertAr;
	unsigned int controllerVertCount;

	Matrix4 worldTrans;
	Matrix4 torsoPose;
	Matrix4 hmdPose;
	Matrix4 inverseHmdPose;
	Matrix4 leftHandPose;
	Matrix4 rightHandPose;
	Matrix4 leftEyePos;
	Matrix4 rightEyePos;

	Matrix4 leftEyeProj;
	Matrix4 rightEyeProj;

	GLuint sceneShader;
	GLuint monitorWindowShader;
	GLuint controllerShader;
	GLuint renderModelShader;
	GLuint fontShader;

	GLint sceneShaderMatrix;
	GLint controllerShaderMatrix;
	GLint renderModelShaderMatrix;
	GLint textColor;
	GLint textProj;

	GLuint VAO, VBO;

	glutil::FramebufferDesc leftEyeDesc;
	glutil::FramebufferDesc rightEyeDesc;

	uint32_t hmdRenderWidth;
	uint32_t hmdRenderHeight;

	std::vector< CGLRenderModel * > modelInventory;
	CGLRenderModel *trackedDeviceModels[ vr::k_unMaxTrackedDeviceCount ];
};
