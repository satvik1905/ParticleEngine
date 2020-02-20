#include "Particle.h"

Particle::Particle(float _fCenterX, float _fCenterY, float _fCenterZ)
	:m_vPos(_fCenterX, _fCenterY, _fCenterZ),
	 m_vVelocity(0.0f , 5.0f),
	 m_fLife(0.0f),
	 m_fRadius(0.125f)	
	 //m_fRadius(0.5f)
{
	const GLfloat pVertices[] = 
	{
		//First Triangle
		  - 1.0f,   - 1.0f, 0,
		  + 1.0f,   - 1.0f, 0,
		  + 1.0f,   + 1.0f, 0,

		 //Second Triangle
		  + 1.0f,   + 1.0f, 0,
		  - 1.0f,   + 1.0f, 0,
		  - 1.0f,   - 1.0f, 0,
	};
	m_VertexCount = 6;

	
	glGenBuffers(1, &m_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);

	//Set Particle Color
	GLfloat clr = 0.5 + ((rand() % 100) / 100.0f); 
	m_vColor =   glm::vec3((double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX));


	// Texture UV coordinates for each vertex. 
	const GLfloat pTextureUV[] = {
		//First Triangle  Vertices Taxture uv mapping
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		//Second Triangle  Vertices Taxture uv mapping
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,		
	};
	glGenBuffers(1, &m_TextureUVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pTextureUV), pTextureUV, GL_STATIC_DRAW);

	
}

Particle::~Particle()
{
}

GLuint Particle::GetTextureUVBuffer()
{
	return m_TextureUVBuffer;
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

glm::vec3 Particle::GetPosition()
{
	return m_vPos;
}

void Particle::SetPosition(glm::vec3 _vNewPos)
{
	m_vPos = _vNewPos;
}

glm::vec2 Particle::GetVelocity()
{
	return m_vVelocity;
}

float Particle::GetLife()
{
	return m_fLife;
}

void Particle::SetLife(float _fNewLife)
{
	m_fLife = _fNewLife;
}

void Particle::Release()
{
	glDeleteBuffers(1, &m_VB);
	glDeleteBuffers(1, &m_TextureUVBuffer);

}

float Particle::GetSize()
{
	return m_fRadius;
}

void Particle::SetSize(float _fSize)
{
	m_fRadius = _fSize;
}
