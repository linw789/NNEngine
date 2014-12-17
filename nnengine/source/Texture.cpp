#include "DebugUtilities.h"
#include "Texture.h"
#include "../dependencies/stb_image.h"

namespace NNE{

NNEbool Texture::load(const NNEchar *filename, NNEuint flag)
{
	NNEint x, y, numComponents;
	// numComponents indicates the original format of the image
	NNEuchar *imgData = stbi_load(filename, &x, &y, &numComponents, 0);

	if (NULL == imgData)
	{
		debugAssertMsg(false, "image loading failed");
		return false;
	}

	m_widthHeight.w = x, m_widthHeight.h = y;

	GL_DEBUG(glActiveTexture(GL_TEXTURE0 + m_unitId));
	GL_DEBUG(glBindTexture(m_type, m_id));

	createTexture(imgData, m_type, x, y, numComponents, flag);
	stbi_image_free(imgData);

	// it's mandatory to specify the following to make texture appear
	GL_DEBUG(glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_DEBUG(glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	return true;
}

NNEbool Texture::load_cubemap(const NNEchar *x_pos_file,
							  const NNEchar *x_neg_file,
							  const NNEchar *y_pos_file,
							  const NNEchar *y_neg_file,
							  const NNEchar *z_pos_file,
							  const NNEchar *z_neg_file,
							  NNEuint flags)
{
	GL_DEBUG(glBindTexture(m_type, m_id));

	NNEint x, y, numComponents;
	// numComponents indicates the original format of the image
	NNEuchar *imgData = stbi_load(x_pos_file, &x, &y, &numComponents, 0);
	m_widthHeight.w = x, m_widthHeight.h = y;
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_POSITIVE_X, x, y, numComponents, flags);
	stbi_image_free(imgData);
	
	imgData = stbi_load(x_neg_file, &x, &y, &numComponents, 0);
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, x, y, numComponents, flags);
	stbi_image_free(imgData);

	imgData = stbi_load(y_pos_file, &x, &y, &numComponents, 0);
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, x, y, numComponents, flags);
	stbi_image_free(imgData);

	imgData = stbi_load(y_neg_file, &x, &y, &numComponents, 0);
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, x, y, numComponents, flags);
	stbi_image_free(imgData);

	imgData = stbi_load(z_pos_file, &x, &y, &numComponents, 0);
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, x, y, numComponents, flags);
	stbi_image_free(imgData);

	imgData = stbi_load(z_neg_file, &x, &y, &numComponents, 0);
	createTexture(imgData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, x, y, numComponents, flags);
	stbi_image_free(imgData);

	GL_DEBUG(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_DEBUG(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_DEBUG(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GL_DEBUG(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_DEBUG(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	return true;
}

NNEbool Texture::createTexture(NNEuchar *data, GLenum target, 
	NNEint w, NNEint h, NNEint channels, NNEuint flag)
{
	GLint imgFormat;
	switch (channels)
	{
	case 1:
		imgFormat = GL_RED;
		break;
	case 2:
		imgFormat = GL_RG;
		break;
	case 3:
		imgFormat = GL_RGB;
		break;
	case 4:
		imgFormat = GL_RGBA;
		break;
	default:
		imgFormat = GL_RGB;
	}

	// flip the image in the Y axis
	NNEuchar *imgData;
	if (flag & NNE_INVERT_Y)
	{
		// byte size of one width of the image
		size_t widthByte = w * channels * sizeof(NNEuchar);
		imgData = (NNEuchar *)::malloc(h * widthByte);

		for (NNEint i = 0; i < h; ++i)
		{
			memcpy(imgData + i * widthByte, data + (h - 1 - i) * widthByte, widthByte);
		}
	}
	else
	{
		imgData = data;
	}

	GLenum internalFormat = imgFormat;

	// seems like internal format has to has larger data size than the fromat indicated to OpenGL
	// always use the same format for texture object as that of the image
	GL_DEBUG(glTexImage2D(target, 0, internalFormat, w, h, 0, imgFormat,
		GL_UNSIGNED_BYTE, imgData));

	return true;
}

} // namespace NNE