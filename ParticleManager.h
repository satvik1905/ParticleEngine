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
	
	float		m_fMAX_LIFE = 1.0f, m_fTex1Life = 0.8;	
private :
	bool		IntializeShader();
	GLuint		LoadTexture(const char *_pFilePath);
	
	//Shader References
	GLuint		m_ShaderId;
	GLuint		m_ColorID;
	GLuint		m_MatrixMVPID;
	GLuint		m_TextureID;
	GLuint		m_IsColorSetID;

	GLuint		m_Texture1, m_Texture2;
	void		UpdateParticleCount();

public:
	ParticleManager();
	~ParticleManager();
	
	void Render(double _dDelta, bool _bColor, glm::vec3 _vWorldPos,  glm::mat4 _matView, glm::mat4 _matProj);
	bool InitializeManager();
	void Release();
	GLuint GetShaderId();
};

