#include "RenderWindow.h"



RenderWindow::RenderWindow()
{
}


RenderWindow::~RenderWindow()
{
}

bool RenderWindow::CreateRenderWindow(int _w, int _h, const char * _pWindowTitle, bool _bIsFullScreen)
{
	/*Initialize the Library*/
	if (!glfwInit())
		return false;

	/*Create Window*/
	if (_bIsFullScreen)
		m_pWindowHandle = glfwCreateWindow(_w, _h, _pWindowTitle, glfwGetPrimaryMonitor(), nullptr);
	else
		m_pWindowHandle = glfwCreateWindow(_w, _h, _pWindowTitle, nullptr, nullptr);

	if (!m_pWindowHandle)
	{
		glfwTerminate(); //Free glfw Resources
		return false;
	}
	
	//Initialize Glew
	glfwMakeContextCurrent(m_pWindowHandle);

	

	return true;
}

GLFWwindow * RenderWindow::GetWindowHandle()
{
	return m_pWindowHandle;
}

void RenderWindow::Release()
{
	glfwTerminate(); //Free glfw Resources
}
