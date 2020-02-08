#pragma once
#include "GLFW/glfw3.h"

class RenderWindow
{
	GLFWwindow* m_pWindowHandle = nullptr;

public:
	RenderWindow();
	~RenderWindow();

	bool CreateRenderWindow(int _w, int _h, const char *_pTitle, bool _bIsFullScreen = false);
	GLFWwindow* GetWindowHandle();
	void Release();
};

