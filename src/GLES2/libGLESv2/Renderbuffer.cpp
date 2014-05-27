// SwiftShader Software Renderer
//
// Copyright(c) 2005-2013 TransGaming Inc.
//
// All rights reserved. No part of this software may be copied, distributed, transmitted,
// transcribed, stored in a retrieval system, translated into any human or computer
// language by any means, or disclosed to third parties without the explicit written
// agreement of TransGaming Inc. Without such an agreement, no rights or licenses, express
// or implied, including but not limited to any patent rights, are granted to you.
//

// Renderbuffer.cpp: the Renderbuffer class and its derived classes
// Colorbuffer, Depthbuffer and Stencilbuffer. Implements GL renderbuffer
// objects and related functionality. [OpenGL ES 2.0.24] section 4.4.3 page 108.

#include "Renderbuffer.h"

#include "main.h"
#include "Texture.h"
#include "utilities.h"

namespace gl
{
RenderbufferInterface::RenderbufferInterface()
{
}

// The default case for classes inherited from RenderbufferInterface is not to
// need to do anything upon the reference count to the parent Renderbuffer incrementing
// or decrementing. 
void RenderbufferInterface::addProxyRef(const Renderbuffer *proxy)
{
}

void RenderbufferInterface::releaseProxy(const Renderbuffer *proxy)
{
}

GLuint RenderbufferInterface::getRedSize() const
{
	return sw2es::GetRedSize(getInternalFormat());
}

GLuint RenderbufferInterface::getGreenSize() const
{
	return sw2es::GetGreenSize(getInternalFormat());
}

GLuint RenderbufferInterface::getBlueSize() const
{
	return sw2es::GetBlueSize(getInternalFormat());
}

GLuint RenderbufferInterface::getAlphaSize() const
{
	return sw2es::GetAlphaSize(getInternalFormat());
}

GLuint RenderbufferInterface::getDepthSize() const
{
	return sw2es::GetDepthSize(getInternalFormat());
}

GLuint RenderbufferInterface::getStencilSize() const
{
	return sw2es::GetStencilSize(getInternalFormat());
}

///// RenderbufferTexture2D Implementation ////////

RenderbufferTexture2D::RenderbufferTexture2D(Texture2D *texture)
{
	mTexture2D.set(texture);
}

RenderbufferTexture2D::~RenderbufferTexture2D()
{
	mTexture2D.set(NULL);
}

// Textures need to maintain their own reference count for references via
// Renderbuffers acting as proxies. Here, we notify the texture of a reference.
void RenderbufferTexture2D::addProxyRef(const Renderbuffer *proxy)
{
    mTexture2D->addProxyRef(proxy);
}

void RenderbufferTexture2D::releaseProxy(const Renderbuffer *proxy)
{
    mTexture2D->releaseProxy(proxy);
}

// Increments refcount on surface.
// caller must release() the returned surface
Image *RenderbufferTexture2D::getRenderTarget()
{
	return mTexture2D->getRenderTarget(GL_TEXTURE_2D, 0);
}

GLsizei RenderbufferTexture2D::getWidth() const
{
	return mTexture2D->getWidth(GL_TEXTURE_2D, 0);
}

GLsizei RenderbufferTexture2D::getHeight() const
{
	return mTexture2D->getHeight(GL_TEXTURE_2D, 0);
}

GLenum RenderbufferTexture2D::getFormat() const
{
	return mTexture2D->getFormat(GL_TEXTURE_2D, 0);
}

sw::Format RenderbufferTexture2D::getInternalFormat() const
{
	return mTexture2D->getInternalFormat(GL_TEXTURE_2D, 0);
}

GLsizei RenderbufferTexture2D::getSamples() const
{
	return 0;
}

///// RenderbufferTextureCubeMap Implementation ////////

RenderbufferTextureCubeMap::RenderbufferTextureCubeMap(TextureCubeMap *texture, GLenum target) : mTarget(target)
{
	mTextureCubeMap.set(texture);
}

RenderbufferTextureCubeMap::~RenderbufferTextureCubeMap()
{
	mTextureCubeMap.set(NULL);
}

// Textures need to maintain their own reference count for references via
// Renderbuffers acting as proxies. Here, we notify the texture of a reference.
void RenderbufferTextureCubeMap::addProxyRef(const Renderbuffer *proxy)
{
    mTextureCubeMap->addProxyRef(proxy);
}

void RenderbufferTextureCubeMap::releaseProxy(const Renderbuffer *proxy)
{
    mTextureCubeMap->releaseProxy(proxy);
}

// Increments refcount on surface.
// caller must release() the returned surface
Image *RenderbufferTextureCubeMap::getRenderTarget()
{
	return mTextureCubeMap->getRenderTarget(mTarget, 0);
}

GLsizei RenderbufferTextureCubeMap::getWidth() const
{
	return mTextureCubeMap->getWidth(mTarget, 0);
}

GLsizei RenderbufferTextureCubeMap::getHeight() const
{
	return mTextureCubeMap->getHeight(mTarget, 0);
}

GLenum RenderbufferTextureCubeMap::getFormat() const
{
	return mTextureCubeMap->getFormat(mTarget, 0);
}

sw::Format RenderbufferTextureCubeMap::getInternalFormat() const
{
	return mTextureCubeMap->getInternalFormat(mTarget, 0);
}

GLsizei RenderbufferTextureCubeMap::getSamples() const
{
	return 0;
}

////// Renderbuffer Implementation //////

Renderbuffer::Renderbuffer(GLuint id, RenderbufferInterface *instance) : RefCountObject(id)
{
	ASSERT(instance != NULL);
	mInstance = instance;
}

Renderbuffer::~Renderbuffer()
{
	delete mInstance;
}

// The RenderbufferInterface contained in this Renderbuffer may need to maintain
// its own reference count, so we pass it on here.
void Renderbuffer::addRef()
{
    mInstance->addProxyRef(this);

    RefCountObject::addRef();
}

void Renderbuffer::release()
{
    mInstance->releaseProxy(this);

    RefCountObject::release();
}

// Increments refcount on surface.
// caller must Release() the returned surface
Image *Renderbuffer::getRenderTarget()
{
	return mInstance->getRenderTarget();
}

GLsizei Renderbuffer::getWidth() const
{
	return mInstance->getWidth();
}

GLsizei Renderbuffer::getHeight() const
{
	return mInstance->getHeight();
}

GLenum Renderbuffer::getFormat() const
{
	return mInstance->getFormat();
}

sw::Format Renderbuffer::getInternalFormat() const
{
	return mInstance->getInternalFormat();
}

GLuint Renderbuffer::getRedSize() const
{
	return mInstance->getRedSize();
}

GLuint Renderbuffer::getGreenSize() const
{
	return mInstance->getGreenSize();
}

GLuint Renderbuffer::getBlueSize() const
{
	return mInstance->getBlueSize();
}

GLuint Renderbuffer::getAlphaSize() const
{
	return mInstance->getAlphaSize();
}

GLuint Renderbuffer::getDepthSize() const
{
	return mInstance->getDepthSize();
}

GLuint Renderbuffer::getStencilSize() const
{
	return mInstance->getStencilSize();
}

GLsizei Renderbuffer::getSamples() const
{
	return mInstance->getSamples();
}

void Renderbuffer::setStorage(RenderbufferStorage *newStorage)
{
	ASSERT(newStorage != NULL);

	delete mInstance;
	mInstance = newStorage;
}

RenderbufferStorage::RenderbufferStorage()
{
	mWidth = 0;
	mHeight = 0;
	format = GL_RGBA4;
	internalFormat = sw::FORMAT_A8R8G8B8;
	mSamples = 0;
}

RenderbufferStorage::~RenderbufferStorage()
{
}

// Increments refcount on surface.
// caller must Release() the returned surface
Image *RenderbufferStorage::getRenderTarget()
{
	return NULL;
}

GLsizei RenderbufferStorage::getWidth() const
{
	return mWidth;
}

GLsizei RenderbufferStorage::getHeight() const
{
	return mHeight;
}

GLenum RenderbufferStorage::getFormat() const
{
	return format;
}

sw::Format RenderbufferStorage::getInternalFormat() const
{
	return internalFormat;
}

GLsizei RenderbufferStorage::getSamples() const
{
	return mSamples;
}

Colorbuffer::Colorbuffer(Image *renderTarget) : mRenderTarget(renderTarget)
{
	if(renderTarget)
	{
		renderTarget->addRef();
		
		mWidth = renderTarget->getWidth();
		mHeight = renderTarget->getHeight();
		internalFormat = renderTarget->getInternalFormat();
		format = sw2es::ConvertBackBufferFormat(internalFormat);
		mSamples = renderTarget->getMultiSampleDepth() & ~1;
	}
}

Colorbuffer::Colorbuffer(int width, int height, GLenum format, GLsizei samples) : mRenderTarget(NULL)
{
	Device *device = getDevice();

	sw::Format requestedFormat = es2sw::ConvertRenderbufferFormat(format);
	int supportedSamples = Context::getSupportedMultiSampleDepth(requestedFormat, samples);

	if(width > 0 && height > 0)
	{
		mRenderTarget = device->createRenderTarget(width, height, requestedFormat, supportedSamples, false);

		if(!mRenderTarget)
		{
			error(GL_OUT_OF_MEMORY);
			return;
		}
	}

	mWidth = width;
	mHeight = height;
	this->format = format;
	internalFormat = requestedFormat;
	mSamples = supportedSamples & ~1;
}

Colorbuffer::~Colorbuffer()
{
	if(mRenderTarget)
	{
		mRenderTarget->release();
	}
}

// Increments refcount on surface.
// caller must release() the returned surface
Image *Colorbuffer::getRenderTarget()
{
	if(mRenderTarget)
	{
		mRenderTarget->addRef();
	}

	return mRenderTarget;
}

DepthStencilbuffer::DepthStencilbuffer(Image *depthStencil) : mDepthStencil(depthStencil)
{
	if(depthStencil)
	{
		depthStencil->addRef();

		mWidth = depthStencil->getWidth();
		mHeight = depthStencil->getHeight();
		internalFormat = depthStencil->getInternalFormat();
		format = sw2es::ConvertDepthStencilFormat(internalFormat);
		mSamples = depthStencil->getMultiSampleDepth() & ~1;
	}
}

DepthStencilbuffer::DepthStencilbuffer(int width, int height, GLsizei samples)
{
	Device *device = getDevice();

	mDepthStencil = NULL;
	
	int supportedSamples = Context::getSupportedMultiSampleDepth(sw::FORMAT_D24S8, samples);

	if(width > 0 && height > 0)
	{
		mDepthStencil = device->createDepthStencilSurface(width, height, sw::FORMAT_D24S8, supportedSamples, false);

		if(!mDepthStencil)
		{
			error(GL_OUT_OF_MEMORY);
			return;
		}
	}

	mWidth = width;
	mHeight = height;
	format = GL_DEPTH24_STENCIL8_OES;
	internalFormat = sw::FORMAT_D24S8;
	mSamples = supportedSamples & ~1;
}

DepthStencilbuffer::~DepthStencilbuffer()
{
	if(mDepthStencil)
	{
		mDepthStencil->release();
	}
}

// Increments refcount on surface.
// caller must release() the returned surface
Image *DepthStencilbuffer::getRenderTarget()
{
	if(mDepthStencil)
	{
		mDepthStencil->addRef();
	}

	return mDepthStencil;
}

Depthbuffer::Depthbuffer(Image *depthStencil) : DepthStencilbuffer(depthStencil)
{
	if(depthStencil)
	{
		format = GL_DEPTH_COMPONENT16;   // If the renderbuffer parameters are queried, the calling function
		                                 // will expect one of the valid renderbuffer formats for use in 
		                                 // glRenderbufferStorage
	}
}

Depthbuffer::Depthbuffer(int width, int height, GLsizei samples) : DepthStencilbuffer(width, height, samples)
{
	if(mDepthStencil)
	{
		format = GL_DEPTH_COMPONENT16;   // If the renderbuffer parameters are queried, the calling function
		                                 // will expect one of the valid renderbuffer formats for use in 
		                                 // glRenderbufferStorage
	}
}

Depthbuffer::~Depthbuffer()
{
}

Stencilbuffer::Stencilbuffer(Image *depthStencil) : DepthStencilbuffer(depthStencil)
{
	if(depthStencil)
	{
		format = GL_STENCIL_INDEX8;   // If the renderbuffer parameters are queried, the calling function
		                              // will expect one of the valid renderbuffer formats for use in 
		                              // glRenderbufferStorage
	}
}

Stencilbuffer::Stencilbuffer(int width, int height, GLsizei samples) : DepthStencilbuffer(width, height, samples)
{
	if(mDepthStencil)
	{
		format = GL_STENCIL_INDEX8;   // If the renderbuffer parameters are queried, the calling function
		                              // will expect one of the valid renderbuffer formats for use in 
		                              // glRenderbufferStorage
	}
}

Stencilbuffer::~Stencilbuffer()
{
}

}
