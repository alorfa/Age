#include "CubeMap.hpp"
#include "Age/LL/opengl.h"
#include <utility>
#include "Age/Math/Math.hpp"
#include "Texture2D.hpp"
#include "Age/Math/mat4.hpp"
#include "Age/egd.hpp"
#include "Age/LL/Pipeline.hpp"
#include "Age/LL/Buffers/FrameBuffer2D.hpp"

namespace a_game_engine
{
	CubeMap::CubeMap()
	{
	}
	CubeMap::CubeMap(CubeMap&& other)
	{
		operator=(std::move(other));
	}
	CubeMap& CubeMap::operator=(CubeMap&& other)
	{
		std::swap(_id, other._id);
		std::swap(_size, other._size);
		return *this;
	}
	CubeMap::~CubeMap()
	{
		destroy();
	}
	void CubeMap::destroy()
	{
		if (_id)
		{
			glDeleteTextures(1, &_id);
			_id = 0;
		}
	}
	void CubeMap::create(const Settings& s)
	{
		destroy();
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);

		_size = s.size;

		glTextureStorage2D(_id, s.mipmaps, TexEnums::toOglFormat(s.format), _size, _size);
		setWrap(s.sampler.wrapX, s.sampler.wrapY, s.sampler.wrapZ);
		setFiltering(s.sampler.min, s.sampler.mag);
		if (s.images == nullptr)
			return;

		bool hasData = false;
		for (uint i = 0; i < 6; i++)
		{
			const auto& img = s.images[i];
			if (img.data)
			{
				hasData = true;
				int outerType, outerFormat;
				TexEnums::toOglOuterFormat(img.format, outerFormat, outerType);
				if (img.data)
					glTextureSubImage3D(_id, 0, 0, 0, i, _size, _size, 1,
						outerFormat, outerType, img.data);
			}
		}
		if (s.mipmaps > 1 && hasData)
			generateMipmaps();
	}
	void CubeMap::createFromPanorama(const PanoramaSettings& s)
	{
		const vec3 x = { 1.f, 0.f, 0.f };
		const vec3 y = { 0.f, 1.f, 0.f };
		const vec3 z = { 0.f, 0.f, 1.f };
		mat4 proj;
		proj.setPerspective(Math::rad(90.f), 1.f, 0.1f, 10.f);
		mat4 view[6];
		view[0].setViewMatrix({ 0.f }, -x, -y, -z);
		view[1].setViewMatrix({ 0.f }, x, -y, z);
		view[2].setViewMatrix({ 0.f }, y, -z, x);
		view[3].setViewMatrix({ 0.f }, -y, z, x);
		view[4].setViewMatrix({ 0.f }, -z, -y, x);
		view[5].setViewMatrix({ 0.f }, z, -y, -x);

		auto& shader = egd.shaders.loadRaw(egd.res / "shader/tex2cubemap.rasl");

		//Pipeline::set2DContext();

		CubeMap::Settings cubeSettings{ nullptr, s.size, s.format, s.sampler, s.mipmaps };
		
		create(cubeSettings);

		FrameBuffer2D fb;

		for (uint i = 0; i < 6; i++)
		{
			fb.setCubemapTexture(0, *this, CubeMap::Face(i));
			fb.use();
			shader.use();
			shader.setUniform(shader.getLocation("panorama"), *s.panorama, 0);
			shader.setUniform(shader.getLocation("projection"), proj);
			shader.setUniform(shader.getLocation("view"), view[i]);
			const auto& skyboxMesh = egd.models.load(egd.res / "model/skybox.obj");
			skyboxMesh.meshes[0]->buffer.draw();
		}
		if (s.mipmaps > 1)
			generateMipmaps();
	}
	void CubeMap::createSpecularMap(const CubeMap& cubemap)
	{
	}
	void CubeMap::createDiffuseMap(const CubeMap& cubemap)
	{
	}
	void CubeMap::activate(int number) const
	{
		glBindTextureUnit(number, _id);
	}
	void CubeMap::setWrap(TextureWrap x, TextureWrap y, TextureWrap z)
	{
		glTextureParameteri(_id, GL_TEXTURE_WRAP_S, TexEnums::toOglWrap(x));
		glTextureParameteri(_id, GL_TEXTURE_WRAP_T, TexEnums::toOglWrap(y));
		glTextureParameteri(_id, GL_TEXTURE_WRAP_R, TexEnums::toOglWrap(z));
	}
	void CubeMap::setWrap(TextureWrap wrap)
	{
		setWrap(wrap, wrap, wrap);
	}
	void CubeMap::setFiltering(TextureFiltering min, TextureFiltering mag)
	{
		glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, TexEnums::toOglFilter(min));
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, TexEnums::toOglFilter(TexEnums::removeMipmaps(mag)));
	}
	void CubeMap::setFiltering(TextureFiltering filter)
	{
		setFiltering(filter, filter);
	}
	void CubeMap::generateMipmaps()
	{
		glGenerateTextureMipmap(_id);
	}

	CubeMap::Settings::Settings(const ImageInfo* images, uint size, TextureFormat format,
		const SamplerCubeInfo& sampler, int mipmaps)
		: images(images), size(size), format(format), sampler(sampler)
	{
		const int maxMips = TexEnums::computeMipLevels(size);
		if (mipmaps < 1)
			this->mipmaps = maxMips;
		else
			this->mipmaps = Math::min(mipmaps, maxMips);

		if (images)
		{
			this->format = TexEnums::chooseInternalFormat(images->format, format);
		}
	}
	CubeMap::PanoramaSettings::PanoramaSettings(const Texture2D& panorama, uint size,
		TextureFormat format, const SamplerCubeInfo& sampler, int mipmaps)
		: panorama(&panorama), sampler(sampler), format(format)
	{
		this->size = size == 0 ? panorama.getSize().y / 2 : size;

		const int maxMips = TexEnums::computeMipLevels(this->size);
		if (mipmaps < 1)
			this->mipmaps = maxMips;
		else
			this->mipmaps = Math::min(mipmaps, maxMips);

		this->format = TexEnums::chooseInternalFormat(panorama.getFormat(), format);
	}
}
