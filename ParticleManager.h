#pragma once
#include "GL/glew.h"
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



class Particle; //Class Forward Defination to Save Compile Time
class ParticleManager
{	
	//std::vector< GLuint >		m_vTextureIDList;
	std::vector<Particle *>		m_vParticleList;
	glm::mat4	m_matView, m_matProj;
	

private :
	bool		IntializeShader();
	GLuint		LoadTexture(const char *_pFilePath);
	
	//Shader References
	GLuint		m_ShaderId;
	GLuint		m_ColorID;
	GLuint		m_MatrixMVPID;
	GLuint		m_TextureID;

	GLuint		m_Texture;

public:
	ParticleManager();
	~ParticleManager();
	
	void Render();
	bool InitializeManager();
	void Release();
	GLuint GetShaderId();
};

