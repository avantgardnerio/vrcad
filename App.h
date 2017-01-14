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

#include "CGLRenderModel.h"
#include "Polygon.h"

class App {
public:
	App( int argc, char *argv[] );
	virtual ~App();

	const char *byte_to_binary(int x);
	bool init();
	bool initGl();
	GLuint App::loadShader(std::string name);
	void App::regenVB();

	void initDeviceModels();

	void Shutdown();

	void mainLoop();
	bool handleInput();
	void processVrEvent( const vr::VREvent_t & event );
	void RenderFrame();

	bool loadTextures();

	void RenderControllerAxes();

	bool setupHmdRenderTargets();
	void setupMonitorWindow();
	void setupCameraMatrices();

	void RenderStereoTargets();
	void RenderCompanionWindow();
	void RenderScene( vr::Hmd_Eye nEye );

	std::string App::getDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL);
	void ThreadSleep( unsigned long nMilliseconds );
	void dprintf( const char *fmt, ... );
	static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	Matrix4 getEyeProj( vr::Hmd_Eye nEye );
	Matrix4 getEyePos( vr::Hmd_Eye nEye );
	Matrix4 GetCurrentViewProjectionMatrix( vr::Hmd_Eye nEye );
	void UpdateHMDMatrixPose();

	Matrix4 ConvertSteamVRMatrixToMatrix4( const vr::HmdMatrix34_t &matPose );

	GLuint CompileGLShader( const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader );
	bool createShaders();

	void initDeviceModel( vr::TrackedDeviceIndex_t unTrackedDeviceIndex );
	CGLRenderModel *getRenderModel( const char *pchRenderModelName );

private: 
	bool m_bPerf;

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

	vr::IVRSystem *hmd;
	std::string m_strDriver;
	std::string m_strDisplay;
	vr::TrackedDevicePose_t devicePose[ vr::k_unMaxTrackedDeviceCount ];
	Matrix4 devicePoseMat[ vr::k_unMaxTrackedDeviceCount ];
	bool m_rbShowTrackedDevice[ vr::k_unMaxTrackedDeviceCount ];

private: // SDL bookkeeping
	SDL_Window *monitorWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;

	SDL_GLContext monitorGlContext;

private: // OpenGL bookkeeping
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[ vr::k_unMaxTrackedDeviceCount ];   // for each device, a character representing its class

	int m_iSceneVolumeInit;                                  // if you want something other than the default 20x20x20
	
	GLuint brickTextureId;

	unsigned int sceneVertCount;

	GLuint sceneVertBuffer;
	GLuint sceneVertexAr;
	GLuint monitorWinVertAr;
	GLuint monitorWinVertBuff;
	GLuint monitorWinIdxBuff;
	unsigned int monitorWinIdxSize;

	GLuint m_glControllerVertBuffer;
	GLuint m_unControllerVAO;
	unsigned int m_uiControllerVertcount;

	Matrix4 m_mat4HMDPose;
	Matrix4 leftEyePos;
	Matrix4 rightEyePos;

	Matrix4 m_mat4ProjectionCenter;
	Matrix4 leftEyeProj;
	Matrix4 rightEyeProj;

	struct VertexDataScene
	{
		Vector3 position;
		Vector2 texCoord;
	};

	struct VertexDataWindow
	{
		Vector2 position;
		Vector2 texCoord;

		VertexDataWindow( const Vector2 & pos, const Vector2 tex ) :  position(pos), texCoord(tex) {	}
	};

	GLuint sceneShader;
	GLuint monitorWindowShader;
	GLuint controllerShader;
	GLuint renderModelShader;

	GLint sceneShaderMatrix;
	GLint controllerShaderMatrix;
	GLint renderModelShaderMatrix;

	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	bool createFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc );
	
	uint32_t hmdRenderWidth;
	uint32_t hmdRenderHeight;

	std::vector< CGLRenderModel * > m_vecRenderModels;
	CGLRenderModel *m_rTrackedDeviceToRenderModel[ vr::k_unMaxTrackedDeviceCount ];
};
