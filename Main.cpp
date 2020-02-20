#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "RenderWindow.h"
#include "ParticleManager.h"

int main()
{
	//Intialize the Window
	RenderWindow *pWindow = new RenderWindow();
	bool bResultValue = pWindow->CreateRenderWindow(600, 400, "ParticleEngine", false); //Set Last Paramter false to Normal Window Size
	if (!bResultValue)
		return 0;
	
	//Intialize GLEW
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		return 0;

	ParticleManager *pParticleManager = new ParticleManager();
	bResultValue = pParticleManager->InitializeManager();
	if (!bResultValue)
		return 0;


	//Set View Matrix
	glm::vec3 vCameraPos = glm::vec3(0, 0, -3);
	glm::vec3 vCameraDir = glm::vec3(0, 0, 1);
	glm::vec3 vUp = glm::vec3(0, 1, 0);
	glm::mat4 matView = glm::lookAt(vCameraPos, vCameraDir, vUp);


	//Set Projection Matrix
	int count = 0;
	const GLFWvidmode *pScreen = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	float fAspectRatio = 600.0f / 400.0f; //(float)pScreen->width / (float)pScreen->height;
	glm::mat4 matProj = glm::perspective(glm::radians(60.0f), fAspectRatio, 0.1f, 10000.0f);
		
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	
	double lastTime = glfwGetTime();
	//Infinite Loop
	while (!glfwWindowShouldClose(pWindow->GetWindowHandle()))
	{
		double currentTime = glfwGetTime();
		double Tick = currentTime - lastTime;
		lastTime = currentTime;

		if (glfwGetKey(pWindow->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow->GetWindowHandle(), true);
					
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ViewPort Coordinate
		double xPos = 0.0f, yPos = 0.0f;
		glfwGetCursorPos(pWindow->GetWindowHandle(), &xPos, &yPos);
		printf("\nMousePos (%f, %f)", xPos, yPos);
				
		//Normalize Coordinate
		double ScreenW = 600.0f, ScreenH = 400.0f;
		double xNormalize = (2.0f * xPos) / ScreenW - 1;
		double yNormalize = (2.0f * yPos) / ScreenH - 1;

		//Homogenious Clip Space
		glm::vec4 clipCoordinate = glm::vec4(xNormalize, yNormalize, -1.0f, 1.0f);
		
		glm::mat4 matInverseViewProj = glm::inverse(matProj * matView);
				
		glm::vec4 worldPos = clipCoordinate * matInverseViewProj;
		
		worldPos.w = 1 / worldPos.w;
		worldPos.x *= worldPos.w;
		worldPos.y *= worldPos.w;
		worldPos.z *= worldPos.w;
				
		pParticleManager->Render(Tick, glm::vec3(worldPos.x, worldPos.y, worldPos.z), matView, matProj);
		
		glfwSwapBuffers(pWindow->GetWindowHandle());
		glfwPollEvents();
	}


	//Release the Window and Geometry(Triangle)
	pWindow->Release();
	delete pWindow;

	pParticleManager->Release();
	delete pParticleManager;

	return 0;
}