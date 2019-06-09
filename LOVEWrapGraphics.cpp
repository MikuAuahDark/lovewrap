/**
 * Copyright (c) 2040 Dark Energy Processor
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

// love
#include "modules/graphics/Graphics.h"

// lovewrap
#include "LOVEWrap.h"

namespace lovewrap
{
namespace graphics
{

void circle(Graphics::DrawMode mode, float x, float y, float r)
{
	getInstance()->circle(mode, x, y, r);
}

void circle(Graphics::DrawMode mode, float x, float y, float r, int segments)
{
	getInstance()->circle(mode, x, y, r, segments);
}

void clear(love::Colorf color, bool clearstencil, bool cleardepth)
{
	getInstance()->clear(love::graphics::OptionalColorf(color), clearstencil, cleardepth);
}

void draw(Drawable *drawable, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky)
{
	getInstance()->draw(drawable, love::Matrix4(x, y, r, sx, sy, ox, oy, kx, ky));
}

void draw(Drawable *drawable, love::math::Transform *transform)
{
	getInstance()->draw(drawable, transform->getMatrix());
}

void draw(Texture *texture, Quad *quad, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky)
{
	getInstance()->draw(texture, quad, love::Matrix4(x, y, r, sx, sy, ox, oy, kx, ky));
}

void draw(Texture *texture, Quad *quad, love::math::Transform *transform)
{
	getInstance()->draw(texture, quad, transform->getMatrix());
}

void present()
{
	getInstance()->present(nullptr);
}

void print(const std::string &text, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky)
{
	getInstance()->print({{text, {1.0f, 1.0f, 1.0f, 1.0f}}}, love::Matrix4(x, y, r, sx, sy, ox, oy, kx, ky));
}

void rectangle(Graphics::DrawMode mode, float x, float y, float w, float h)
{
	getInstance()->rectangle(mode, x, y, w, h);
}

void rectangle(Graphics::DrawMode mode, float x, float y, float w, float h, float rx, float ry)
{
	getInstance()->rectangle(mode, x, y, w, h, rx, ry);
}

void push(Graphics::StackType type)
{
	return getInstance()->push(type);
}

void rotate(float f)
{
	return getInstance()->rotate(f);
}

void scale(float x, float y)
{
	return getInstance()->scale(x, y);
}

void scale(float s)
{
	return getInstance()->scale(s, s);
}

void translate(float x, float y)
{
	return getInstance()->translate(x, y);
}

void shear(float kx, float ky)
{
	return getInstance()->shear(kx, ky);
}

void origin()
{
	return getInstance()->origin();
}

void pop()
{
	return getInstance()->pop();
}

Canvas *newCanvas()
{
	Canvas::Settings setting;
	auto inst = getInstance();
	setting.width = inst->getWidth();
	setting.height = inst->getHeight();
	setting.dpiScale = inst->getScreenDPIScale();
	return getInstance()->newCanvas(setting);
}

Canvas *newCanvas(Canvas::Settings &setting)
{
	return getInstance()->newCanvas(setting);
}

Font *newFont(love::font::Rasterizer *rast)
{
	return getInstance()->newFont(rast);
}

Font *newFont(int32_t size)
{
	auto finst = lovewrap::font::getInstance();
	auto vera = finst->newTrueTypeRasterizer(size, love::font::TrueTypeRasterizer::HINTING_NORMAL);
	auto ret = getInstance()->newFont(vera);

	vera->release();
	return ret;
}

Image *newImage(const std::string& filename, const Image::Settings *settings)
{
	auto lfs = lovewrap::filesystem::getInstance();
	auto f = lfs->newFile(filename.c_str());
	auto out = newImage(f, settings);
	f->release();
	return out;
}

Image *newImage(love::filesystem::File *file, const Image::Settings *settings)
{
	// Open file
	if (!file->isOpen())
	{
		if (!file->open(love::filesystem::File::Mode::MODE_READ))
			// File can't be opened
			return nullptr;
	}
	else
		file->seek(0);
	
	// Load ImageData
	love::filesystem::FileData *filedata = file->read();
	if (filedata == nullptr)
		return nullptr;

	auto out = newImage(filedata, settings);
	filedata->release();
	return out;
}

Image *newImage(love::filesystem::FileData *filedata, const Image::Settings *settings)
{
	auto li = lovewrap::image::getInstance();

	// Try CompressedImageData
	try
	{
		love::image::CompressedImageData *img = li->newCompressedData(filedata);
		auto out = newImage(img, settings);
		img->release();
		return out;
	}
	// Try ImageData
	catch (love::Exception &)
	{
		love::image::ImageData *img = li->newImageData(filedata);
		auto out = newImage(img, settings);
		img->release();
		return out;
	}
}

Image *newImage(love::image::ImageData *imagedata, const Image::Settings *settings)
{
	auto lg = getInstance();
	
	Image::Settings def;
	if (settings == nullptr)
	{
		settings = &def;
		def.dpiScale = (float) lovewrap::graphics::getDPIScale();
		def.linear = false;
		def.mipmaps = false;
	}

	Image::Slices slices(TEXTURE_2D);
	slices.set(0, 0, imagedata);
	auto out = lg->newImage(slices, *settings);
	return out;
}

Image *newImage(love::image::CompressedImageData *cimgd, const Image::Settings *settings)
{
	auto lg = getInstance();
	
	Image::Settings def;
	if (settings == nullptr)
	{
		settings = &def;
		def.dpiScale = (float) lovewrap::graphics::getDPIScale();
		def.linear = false;
		def.mipmaps = false;
	}

	Image::Slices slices(TEXTURE_2D);
	slices.add(cimgd, 0, 0, false, settings->mipmaps);
	auto out = lg->newImage(slices, *settings);
	return out;
}

Mesh *newMesh(const std::vector<Mesh::AttribFormat> &vertexformat, int vertexcount, PrimitiveType drawmode, vertex::Usage usage)
{
	return getInstance()->newMesh(vertexformat, vertexcount, drawmode, usage);
}

Mesh *newMesh(const std::vector<Mesh::AttribFormat> &vertexformat, const void *data, size_t datasize, PrimitiveType drawmode, vertex::Usage usage)
{
	return getInstance()->newMesh(vertexformat, data, datasize, drawmode, usage);
}

namespace shader
{

void sendBools(Shader *shader, const std::string &name, std::initializer_list<bool> values)
{
	const Shader::UniformInfo *info = shader->getUniformInfo(name);
	if (info == nullptr)
		throw love::Exception("Shader uniform '%s' does not exist.\nA common error is to define but not use the variable.", name.c_str());
	else if (info->baseType != Shader::UNIFORM_BOOL)
		throw love::Exception("Uniform does not accept boolean.");

	// Convert int to bool
	std::vector<int> boolArray;
	for (bool b: values)
		boolArray.push_back((int) b);

	// Check size
	if (boolArray.size() % ((size_t) info->components) != 0)
		throw love::Exception("Passed value is not divisible by %d", info->components);

	// Then copy
	int maxsize = std::min(info->count * info->components, (int) boolArray.size());
	memcpy(info->ints, boolArray.data(), maxsize * sizeof(int));
	shader->updateUniform(info, maxsize / info->components);
}

void sendFloats(Shader *shader, const std::string &name, std::initializer_list<float> values)
{
	const Shader::UniformInfo *info = shader->getUniformInfo(name);
	if (info == nullptr)
		throw love::Exception("Shader uniform '%s' does not exist.\nA common error is to define but not use the variable.", name.c_str());
	else if (info->baseType != Shader::UNIFORM_FLOAT)
		throw love::Exception("Uniform does not accept float.");

	std::vector<float> floatArray = values;

	// Check size
	if (floatArray.size() % ((size_t) info->components) != 0)
		throw love::Exception("Passed value is not divisible by %d", info->components);
	
	// Then copy
	int maxsize = std::min(info->count * info->components, (int) floatArray.size());
	memcpy(info->floats, floatArray.data(), maxsize * sizeof(float));
	shader->updateUniform(info, maxsize / info->components);
}

void sendMatrix(Shader *shader, const std::string &name, int rows, int columns, const float *data)
{
	const Shader::UniformInfo *info = shader->getUniformInfo(name);
	if (info == nullptr)
		throw love::Exception("Shader uniform '%s' does not exist.\nA common error is to define but not use the variable.", name.c_str());
	else if (info->baseType != Shader::UNIFORM_MATRIX)
		throw love::Exception("Uniform does not accept matrix.");
	else if (info->matrix.rows != rows || info->matrix.columns != columns)
		throw love::Exception("Matrix does not have %d rows or %d columns", rows, columns);
	
	// Then copy
	memcpy(info->floats, data, rows * columns * sizeof(float));
	shader->updateUniform(info, info->count);
}

void sendMat3(Shader *shader, const std::string &name, const love::Matrix3 &matrix)
{
	return sendMatrix(shader, name, 3, 3, matrix.getElements());
}

void sendMat4(Shader *shader, const std::string &name, const love::Matrix4 &matrix)
{
	return sendMatrix(shader, name, 4, 4, matrix.getElements());
}

void sendTextures(Shader *shader, const std::string &name, std::initializer_list<Texture*> values)
{
	const Shader::UniformInfo *info = shader->getUniformInfo(name);
	if (info == nullptr)
		throw love::Exception("Shader uniform '%s' does not exist.\nA common error is to define but not use the variable.", name.c_str());
	else if (info->baseType != Shader::UNIFORM_SAMPLER)
		throw love::Exception("Uniform does not accept texture.");

	std::vector<Texture*> textures;
	textures.reserve((size_t) info->count);

	int i = 0;
	for (Texture *tex: values)
	{
		++i;

		if (tex->getTextureType() != info->textureType)
			throw love::Exception("Invalid texture type for uniform at #%d", i);

		textures.push_back(tex);
	}

	shader->sendTextures(info, textures.data(), (int) textures.size());
}

} // shader

love::Colorf getBackgroundColor()
{
	return getInstance()->getBackgroundColor();
}

Graphics::BlendMode getBlendMode(Graphics::BlendAlpha &alphaMode)
{
	return getInstance()->getBlendMode(alphaMode);
}

double getDPIScale()
{
	return getInstance()->getScreenDPIScale();
}

int getHeight()
{
	return getInstance()->getHeight();
}

int getWidth()
{
	return getInstance()->getWidth();
}

bool isActive()
{
	return getInstance()->isActive();
}

void setBlendMode(Graphics::BlendMode blendMode, Graphics::BlendAlpha alphaMode)
{
	getInstance()->setBlendMode(blendMode, alphaMode);
}

void setCanvas(Canvas *canvas)
{
	auto inst = getInstance();
	inst->stopDrawToStencilBuffer();

	if (canvas == nullptr)
		inst->setCanvas();
	else
	{
		Graphics::RenderTargets rt = Graphics::RenderTargets();
		rt.colors.push_back(canvas);
		inst->setCanvas(rt);
	}
}

void setCanvas(Graphics::RenderTargetsStrongRef &rts)
{
	auto inst = getInstance();
	inst->stopDrawToStencilBuffer();
	inst->setCanvas(rts);
}

void setCanvas(Graphics::RenderTargets &rts)
{
	auto inst = getInstance();
	inst->stopDrawToStencilBuffer();
	inst->setCanvas(rts);
}

void setColor(float r, float g, float b, float a)
{
	setColor(love::Colorf(r, g, b, a));
}

void setColor(love::Colorf color)
{
	getInstance()->setColor(color);
}

void setDepthMode(CompareMode mode, bool value)
{
	getInstance()->setDepthMode(mode, value);
}

void setDepthMode()
{
	getInstance()->setDepthMode();
}

void setMeshCullMode(CullMode mode)
{
	getInstance()->setMeshCullMode(mode);
}

void setShader(Shader *shader)
{
	getInstance()->setShader(shader);
}

void setWireframe(bool wireframe)
{
	getInstance()->setWireframe(wireframe);
}

} // graphics
} // lovewrap
