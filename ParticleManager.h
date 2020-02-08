#pragma once
#include "GL/glew.h"
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



class Particle;
class ParticleManager
{	
	std::vector<Particle *> m_vParticleList;
	glm::mat4	m_matView, m_matProj;
	

private :
	bool IntializeShader();
	GLuint m_ShaderId;
	GLuint m_ColorID;
	GLuint m_MatrixMVPID;

public:
	ParticleManager();
	~ParticleManager();
	
	void Render();
	bool InitializeManager();
	void Release();
	GLuint GetShaderId();
};

