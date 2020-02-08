#pragma once
#define GLEW_STATIC
#include "GL/glew.h"

class Particle
{
	//Vertex Buffer
	GLuint			m_VB; 

	//No of Vertices to form the One Particle
	GLuint			m_VertexCount;

	//Radius of Particle : Since Rectangle is the shape of particle so radius represent 
	GLfloat			m_fRadius;

public:
	Particle(float _fCenterX, float _fCenterY, float _fCenterZ);
	~Particle();

	GLuint GetVertexBuffer();
	GLuint GetVertexCount();
	void Release();	
};

