#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include <glm/glm.hpp>
//using namespace glm;

class Particle
{
	//Vertex Buffer
	GLuint			m_VB, m_TextureUVBuffer; 

	//No of Vertices to form the One Particle
	GLuint			m_VertexCount;

	//Radius of Particle : Since Rectangle is the shape of particle so radius represent 
	GLfloat			m_fRadius;

	//Color Of Particle
	glm::vec3		m_vColor;

	glm::vec3		m_vPos;

public:
	Particle(float _fCenterX, float _fCenterY, float _fCenterZ);
	~Particle();

	GLuint		GetTextureUVBuffer();
	GLuint		GetVertexBuffer();
	GLuint		GetVertexCount();
	glm::vec3	GetColor();
	glm::vec3	GetPosition();
	void		Release();	
};

