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
		
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Infinite Loop
	while (!glfwWindowShouldClose(pWindow->GetWindowHandle()))
	{
		if (glfwGetKey(pWindow->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow->GetWindowHandle(), true);
					
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		pParticleManager->Render();
		
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