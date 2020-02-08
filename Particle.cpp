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

void Particle::Release()
{
	glDeleteBuffers(1, &m_VB);
}
