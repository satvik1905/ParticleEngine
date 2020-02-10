#include "Particle.h"

Particle::Particle(float _fCenterX, float _fCenterY, float _fCenterZ)
	//:m_fRadius(0.0125f)
	:m_fRadius(1.0f)
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

void Particle::Release()
{
	glDeleteBuffers(1, &m_VB);
	glDeleteBuffers(1, &m_TextureUVBuffer);

}
