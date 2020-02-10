#include "ParticleManager.h"
#include "Particle.h"
#include <string>
#include <fstream>
#include <sstream>
#include <time.h> //For Random Number Generator


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


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
	m_ColorID = glGetUniformLocation(m_ShaderId, "ParticleColor");

	// Get a handle for our "MVP" uniform
	m_MatrixMVPID = glGetUniformLocation(m_ShaderId, "MVP");

	// Get a handle for our "TextureSampler" uniform
	m_TextureID = glGetUniformLocation(m_ShaderId, "ParticleTexture");


	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertexShaderId);
	glDeleteShader(pixelShaderId);

	return true;
}

GLuint ParticleManager::LoadTexture(const char * _pFilePath)
{
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fopen_s(&fp, _pFilePath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", _pFilePath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}

ParticleManager::ParticleManager()
{
	/* initialize random seed: */
	//Only Once : Random Value Use to Generate Particle Position
	srand(time(NULL)); 

	//Set View Matrix
	glm::vec3 vCameraPos = glm::vec3(0, 0, -3);
	glm::vec3 vCameraDir = glm::vec3(0, 0, 1);
	glm::vec3 vUp		 = glm::vec3(0, 1, 0);
	m_matView = glm::lookAt(vCameraPos, vCameraDir, vUp);

	//Set Projection Matrix
	m_matProj = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 10000.0f);
}



ParticleManager::~ParticleManager()
{
}

void ParticleManager::Render()

{	
	
	for (unsigned int iParticleIndex = 0; iParticleIndex < m_vParticleList.size(); iParticleIndex++)
	{
		auto pParticle = m_vParticleList[iParticleIndex];
		
		glUseProgram(m_ShaderId);
		//Set MVP in Vertex Shader
		float fTransX = (rand() % 20)  + 1;
		float fTransY = (rand() % 10) + 1;
		float fTransZ = (rand() % 5) + 1;
		//glm::mat4 matModel = glm::translate(glm::mat4(1.0f), glm::vec3(fTransX, fTransY, fTransZ));
		glm::mat4 matModel = glm::mat4(1.0f);
		//glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 1));

		glm::mat4 MVP = m_matProj * m_matView *  matModel;/**  matModel;*/ ///*matScale;
		glm::vec3 vColor = pParticle->GetColor();
		glUniform3f(m_ColorID, vColor.r, vColor.g, vColor.b);
		glUniformMatrix4fv(m_MatrixMVPID, 1, GL_FALSE, &MVP[0][0]);
		
		//Set Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);		
		glUniform1i(m_TextureID, 0);
				
		//1 - Set verticex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, pParticle->GetVertexBuffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2 - Set UVs buffer 
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, pParticle->GetTextureUVBuffer());
		glVertexAttribPointer(1, 2,	GL_FLOAT, GL_FALSE,	0, (void*)0);
		
		glDrawArrays(GL_TRIANGLES, 0, pParticle->GetVertexCount());
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);		
	}
	
	
}

bool ParticleManager::InitializeManager()
{	
	if (IntializeShader())
	{
		//Add 1000 Particle
		for (unsigned int i = 0; i < 1; i++)
		{	
			float fPosX =  ((double)rand()  / (RAND_MAX));
			float fPosY =  ((double)rand()  / (RAND_MAX));
			float fPosZ =  ((double)rand()  / (RAND_MAX));
			m_vParticleList.push_back(new Particle(fPosX, fPosY, fPosZ));
		}

		//LoadTexture		
		m_Texture = LoadTexture(".\\Data\\Textures\\test-dxt5.dds");
		
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
	glDeleteTextures(1, &m_Texture);
	glDeleteVertexArrays(1, &m_TextureID);

}

GLuint ParticleManager::GetShaderId()
{
	return m_ShaderId;
}
