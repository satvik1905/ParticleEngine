#include "Particle.h"

Particle::Particle(float _fCenterX, float _fCenterY, float _fCenterZ)
	:m_fRadius(0.0125f)
{
	const GLfloat pVertices[] = 
	{
		//First Triangle
		 _fCenterX - m_fRadius,  _fCenterY - m_fRadius, _fCenterZ,
		 _fCenterX + m_fRadius,  _fCenterY - m_fRadius, _fCenterZ,
		 _fCenterX + m_fRadius,  _fCenterY + m_fRadius, _fCenterZ,

		 //Second Triangle
		 _fCenterX + m_fRadius,  _fCenterY + m_fRadius, _fCenterZ,
		 _fCenterX - m_fRadius,  _fCenterY + m_fRadius, _fCenterZ,
		 _fCenterX - m_fRadius,  _fCenterY - m_fRadius, _fCenterZ,
	};
	m_VertexCount = 6;

	
	glGenBuffers(1, &m_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);

	//Set Particle Color
	GLfloat clr = 0.5 + ((rand() % 100) / 100.0f); //Not using Now
	m_vColor =   glm::vec3((double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX));
}

Particle::~Particle()
{
}

GLuint Particle::GetVertexBuffer()
{
	return m_VB;
}

GLuint Particle::GetVertexCount()
{
	return m_VertexCount;
}

glm::vec3 Particle::GetColor()
{
	return m_vColor;
}

void Particle::Release()
{
	glDeleteBuffers(1, &m_VB);
}
