#pragma once
#include "GL/glew.h"
#include <vector>

class Particle;
class ParticleManager
{	
	std::vector<Particle *> m_vParticleList;

private :
	bool IntializeShader();
	GLuint m_ShaderId;
	GLuint m_ParticleColorID;
public:
	ParticleManager();
	~ParticleManager();
	
	void Render();
	bool InitializeManager();
	void Release();
	GLuint GetShaderId();
};

