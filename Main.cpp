#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "RenderWindow.h"
#include "ParticleManager.h"

int main()
{
	//Intialize the Window
	RenderWindow *pWindow = new RenderWindow();
	bool bResultValue = pWindow->CreateRenderWindow(600, 400, "ParticleEngine", true); //Set Last Paramter false to Normal Window Size
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
	float fAspectRatio = (float)pScreen->width / (float)pScreen->height;//600.0f / 400.0f; //
	glm::mat4 matProj = glm::perspective(glm::radians(60.0f), fAspectRatio, 0.1f, 10000.0f);
		
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	
	double lastTime = glfwGetTime(); bool bSetColor = false;
	//Infinite Loop
	while (!glfwWindowShouldClose(pWindow->GetWindowHandle()))
	{
		double currentTime = glfwGetTime();
		double Tick = currentTime - lastTime;
		lastTime = currentTime;

		if (glfwGetKey(pWindow->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow->GetWindowHandle(), true);

		if (glfwGetKey(pWindow->GetWindowHandle(), GLFW_KEY_C) == GLFW_PRESS)
			bSetColor = (bSetColor == true) ? false : true;
					
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		pParticleManager->Render(Tick, bSetColor, matView, matProj);
		
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