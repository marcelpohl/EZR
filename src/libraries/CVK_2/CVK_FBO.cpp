#include "CVK_FBO.h"

#include <iostream>

CVK::FBO::FBO(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture, bool cubeMapTexture)
{
	create(width, height, numColorTextures, depthTexture, stencilTexture, cubeMapTexture);
}

CVK::FBO::~FBO()
{
	reset();
}

void CVK::FBO::create(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture, bool cubeMapTexture)
{
	reset();

	m_width = width;
	m_height = height;

	// generate fbo
	glGenFramebuffers(1, &m_frameBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
	
	// generate color textures
//std::vector<GLenum> drawBuffers; Problem: funktioniert durch .data() nur in neuer Umgebung...
	GLenum *drawBuffers = new GLenum[ numColorTextures];
	
	for(int i = 0; i < numColorTextures; i++)
	{
		GLuint colorTextureHandle = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F, m_width, m_height, 0,GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+m_colorTextures.size(), GL_TEXTURE_2D, colorTextureHandle, 0);
		m_colorTextures.push_back(colorTextureHandle);
//drawBuffers.push_back(GL_COLOR_ATTACHMENT0+i);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
	}
	// use color textures in fbo
//glDrawBuffers(numColorTextures, drawBuffers.data());
	glDrawBuffers(numColorTextures, drawBuffers);

	if(depthTexture)
	{
		// generate depth texture
		m_depthTexture = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, m_width, m_height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	}

	if(stencilTexture)
	{
		// generate stencil texture
		m_stencilTexture = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH24_STENCIL8, m_width, m_height, 0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilTexture, 0);
	}

	if (cubeMapTexture)
	{
		m_depthCubeMapTexture = createTexture(true);
		for (int i = 0; i < 6; i++) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubeMapTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	
	// Any errors while generating fbo ?
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Unable to create FBO." << std::endl;
		reset();
	}
	
	// cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CVK::FBO::reset()
{
	// delete all textures
	for(unsigned int i = 0; i < m_colorTextures.size(); i++)
	{
		glDeleteTextures(1, &m_colorTextures[i]);
	}
	m_colorTextures.clear();
	if(m_depthTexture != INVALID_GL_VALUE)
		glDeleteTextures(1, &m_depthTexture);
	if(m_stencilTexture != INVALID_GL_VALUE)
		glDeleteTextures(1, &m_stencilTexture);
	if(m_frameBufferHandle != INVALID_GL_VALUE)
		glDeleteFramebuffers(1, &m_frameBufferHandle);
	if (m_depthCubeMapTexture != INVALID_GL_VALUE)
		glDeleteTextures(1, &m_depthCubeMapTexture);
	
	// reset values
	m_depthTexture = INVALID_GL_VALUE;
	m_stencilTexture = INVALID_GL_VALUE;
	m_frameBufferHandle = INVALID_GL_VALUE;
	m_depthCubeMapTexture = INVALID_GL_VALUE;
}

void CVK::FBO::resize(int width, int height)
{
	int numColorTextures = m_colorTextures.size();
	bool depthTexture = false;
	if(m_depthTexture != INVALID_GL_VALUE)
		depthTexture = true;
	bool stencilTexture = false;
	if(m_stencilTexture != INVALID_GL_VALUE)
		stencilTexture = true;
	bool cubeMapTexture = false;
	if (m_depthCubeMapTexture != INVALID_GL_VALUE)
		cubeMapTexture = true;

	reset();
	create(width, height, numColorTextures, depthTexture, stencilTexture, cubeMapTexture);
}

void CVK::FBO::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
	glViewport( 0, 0, m_width, m_height);
}

void CVK::FBO::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint CVK::FBO::getColorTexture(unsigned int index)
{
	// does color buffer with given index exist ?
	if(index < m_colorTextures.size())
		return m_colorTextures[index];
	return 0;
}

GLuint CVK::FBO::getDepthTexture() const
{
	// does depth buffer exist ?
	if(m_depthTexture == INVALID_GL_VALUE)
		return 0;
	return m_depthTexture;
}



GLuint CVK::FBO::getDepthCubemapTexture() const
{
	if (m_depthCubeMapTexture == INVALID_GL_VALUE)
		return 0;
	return m_depthCubeMapTexture;
}

GLuint CVK::FBO::createTexture(bool isCubeMap) const
{
	// generate fresh texture in OpenGL
	GLuint textureID;
	glGenTextures( 1, &textureID);

	if (!isCubeMap)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	return textureID;
}
