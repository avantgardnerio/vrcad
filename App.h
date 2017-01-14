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
#include "GlUtil.h"
#include "thirdparty/openvr-1.0.5/samples/shared/Matrices.h"
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

public:
	App( int argc, char *argv[] );
	virtual ~App();

	// lifecycle
	bool init();
	bool initGl();
	void initDeviceModels();
	bool loadTextures();
	bool setupHmdRenderTargets();
	void setupMonitorWindow();
	void setupCameraMatrices();
	void shutdown();

	// rendering
	void regenVB();
	void mainLoop();
	bool handleInput();
	void renderFrame();
	void renderControllerAxes();
	void renderToHmd();
	void renderToMonitorWindow();
	void renderToEye(vr::Hmd_Eye nEye);

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

private: 

	// ---- me
	enum EditMode { 
		none,
		draw, 
		extrude 
	};

	EditMode mode;
	vr::TrackedDeviceIndex_t currentController;
	net_squarelabs::Polygon *currentPolygon;
	std::vector<net_squarelabs::Polygon> polygons;
	bool buttonPressed;

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

	Matrix4 inverseHmdPose;
	Matrix4 leftEyePos;
	Matrix4 rightEyePos;

	Matrix4 leftEyeProj;
	Matrix4 rightEyeProj;

	GLuint sceneShader;
	GLuint monitorWindowShader;
	GLuint controllerShader;
	GLuint renderModelShader;

	GLint sceneShaderMatrix;
	GLint controllerShaderMatrix;
	GLint renderModelShaderMatrix;

	glutil::FramebufferDesc leftEyeDesc;
	glutil::FramebufferDesc rightEyeDesc;

	uint32_t hmdRenderWidth;
	uint32_t hmdRenderHeight;

	std::vector< CGLRenderModel * > modelInventory;
	CGLRenderModel *trackedDeviceModels[ vr::k_unMaxTrackedDeviceCount ];
};
