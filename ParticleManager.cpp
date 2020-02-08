#include "ParticleManager.h"
#include "Particle.h"
#include <string>
#include <fstream>
#include <sstream>
#include <time.h> //For Random Number Generator

//// Include GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

bool ParticleManager::IntializeShader()
{
	// 1. Fetch Shader Code
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(".\\Data\\Particle.vertexshader", std::ios::in);
		fShaderFile.open(".\\Data\\Particle.fragmentshader", std::ios::in);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		//std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	// 2. compile shaders	
	int success; char infoLog[512];
	//vertex Shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
	glCompileShader(vertexShaderId);
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	};

	//Pixel Shader
	GLuint pixelShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixelShaderId, 1, &fShaderCode, NULL);
	glCompileShader(pixelShaderId);
	glGetShaderiv(pixelShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(pixelShaderId, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	};



	//3. shader Program
	m_ShaderId = glCreateProgram();
	glAttachShader(m_ShaderId, vertexShaderId);
	glAttachShader(m_ShaderId, pixelShaderId);
	glLinkProgram(m_ShaderId);
	// print linking errors if any
	glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderId, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// Get a handle for our "Color" uniform
	m_ParticleColorID = glGetUniformLocation(m_ShaderId, "ParticleColor");

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertexShaderId);
	glDeleteShader(pixelShaderId);

	return true;
}

ParticleManager::ParticleManager()
{
	/* initialize random seed: */
	//Only Once
	srand(time(NULL)); 
}


ParticleManager::~ParticleManager()
{
}

void ParticleManager::Render()
{
	glUseProgram(m_ShaderId);
//	glUniform3f(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glEnableVertexAttribArray(0);
	for (unsigned int iParticleIndex = 0; iParticleIndex < m_vParticleList.size(); iParticleIndex++)
	{
		auto pParticle = m_vParticleList[iParticleIndex];
		
		//Render		
		glBindBuffer(GL_ARRAY_BUFFER, pParticle->GetVertexBuffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, pParticle->GetVertexCount());
		
	}
	glDisableVertexAttribArray(0);
}

bool ParticleManager::InitializeManager()
{	
	if (IntializeShader())
	{
		//Add 100 Particle
		for (unsigned int i = 0; i < 100; i++)
		{	
			float randomX =  ((double)rand()  / (RAND_MAX));
			float randomY =  ((double)rand()  / (RAND_MAX));
			float randomZ =  ((double)rand()  / (RAND_MAX));
			m_vParticleList.push_back(new Particle(randomX, randomY, randomZ));
		}
		return true;
	}	
	return false;
}

void ParticleManager::Release()
{
	//Delete All Particles One-By-One
	for (unsigned int iParticleIndex = 0; iParticleIndex < m_vParticleList.size(); iParticleIndex++)
	{
		auto pParticle = m_vParticleList[iParticleIndex];
		pParticle->Release();
	}

	//Delete Shader
	glDeleteProgram(m_ShaderId);
}

GLuint ParticleManager::GetShaderId()
{
	return m_ShaderId;
}
