#include "Resource.h"
#include "AssimpLoader.h"
#include "Material.h"
#include "GPUProgram.h"
#include "Texture.h"
#include "Mesh.h"
#include "ClothMesh.h"
#include "Skin.h"
#include "Animation.h"
#include "HeightMap.h"
#include "Font.h"
#include "String.h"

namespace NNE{

NNEbool Resource::init()
{
	// the first 10 element of NNE_BindingPointPool will be true
	for (NNEint i = 0; i < 10; ++i)
	{
		m_bindingPointPool[i] = true;
	}

	for (NNEint i = 10; i < MAX_UNIFORM_BINDING_POINTS; ++i)
	{
		m_bindingPointPool[i] = false;
	}

	// always set as the first to be added to m_GPUProgArray
	createGPUProgram("debugLine", "..//Assets/Shader/drawLine.vert", "..//Assets/Shader/drawLine.geom", "..//Assets/Shader/drawLine.frag");
	// always set as the second to be added to m_GPUProgArray
	createGPUProgram("drawNormal", "..//Assets/Shader/drawNormal.vert", "..//Assets/Shader/drawNormal.geom", "..//Assets/Shader/drawNormal.frag");

	createGPUProgram("debugPoint", "..//Assets/Shader/drawDebugPoint.vert", "..//Assets/Shader/drawDebugPoint.geom", "..//Assets/Shader/drawDebugPoint.frag");

	createGPUProgram("shadowMappingFirstPass", "..//Assets/Shader/shadow_mapping_firstpass.vert", "..//Assets/Shader/shadow_mapping_firstpass.frag");

	createGPUProgram("BlinnPhongShadowMapping", "..//Assets/Shader/BlinnPhongShadowMapping.vert", "..//Assets/Shader/BlinnPhongShadowMapping.frag");

	createGPUProgram("VSM_genShadowMap", "..//Assets/Shader/VSM_genShadowMap.vert", "..//Assets/Shader/VSM_genShadowMap.frag");

	createGPUProgram("BlinnPhongVSM", "..//Assets/Shader/BlinnPhongVSM.vert", "..//Assets/Shader/BlinnPhongVSM.frag");

	createGPUProgram("vsm_GaussianBlurX", "..//Assets/Shader/vsm_GaussianBlurX.vert", "..//Assets/Shader/vsm_GaussianBlur.frag");

	createGPUProgram("vsm_GaussianBlurY", "..//Assets/Shader/vsm_GaussianBlurY.vert", "..//Assets/Shader/vsm_GaussianBlur.frag");

	createGPUProgram("skin_shadowmapfirstpass", "..//Assets/Shader/skin_shadowmapfirstpass.vert", "..//Assets/Shader/skin_shadowmapfirstpass.frag");

	createFont("default", "..//Assets/SDF_Font/automati.ttf_sdf");

	return true;
}

Mesh *Resource::createMesh(const NNEchar *name, const NNEchar *filename)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Mesh *>::iterator it = m_meshArray.begin(); it != m_meshArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "mesh name already exists");
		}
	);

	Mesh *amesh = new Mesh(sid, GPU_TRIANGLES);
	m_meshArray.push_back(amesh);

	AssimpLoader::loadOneMesh(filename, amesh);

	return amesh;
}

Mesh *Resource::createMesh(const NNEchar *name)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Mesh *>::iterator it = m_meshArray.begin(); it != m_meshArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	Mesh *amesh = new Mesh(sid, GPU_TRIANGLES);
	m_meshArray.push_back(amesh);

	return amesh;
}

ClothMesh *Resource::createClothMesh(const NNEchar *name, NNEuint w, NNEuint h, NNEfloat interval)
{
	Sid sid = HashString(name);

	ClothMesh *amesh = new ClothMesh(sid, GPU_TRIANGLES);
	amesh->buildMesh(w, h, interval);
	m_meshArray.push_back(amesh);

	return amesh;
}

Skin *Resource::createSkin(const NNEchar *name, const NNEchar *filename)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Skin *>::iterator it = m_skinArray.begin(); it != m_skinArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	Skin *askin = new Skin(sid);
	m_skinArray.push_back(askin);

	AssimpLoader::loadOneSkin(filename, askin);

	return askin;
}

Animation *Resource::createAnimation(const NNEchar *name, const NNEchar *filename)
{
	Sid sid = HashString(name);
	
#if defined(NNE_DEBUG)

	for (Array<Animation *>::iterator it = m_animArray.begin(); it != m_animArray.end(); ++it)
	{
		debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
	}

#endif

	Animation *anim = new Animation(sid);
	m_animArray.push_back(anim);

	AssimpLoader::loadOneAnimation(filename, anim);

	return anim;
}

//Mesh *Resource::createMeshFromHeightMap(const NNEchar *name, const NNEchar *filename)
//{
//	Sid sid = HashString(name);
//	debugExecute(
//		for (Array<Mesh *>::iterator it = m_meshArray.begin(); it != m_meshArray.end(); ++it)
//		{
//			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
//		}
//	);
//
//	Mesh *amesh = new Mesh(sid, GPU_TRIANGLES);
//	m_meshArray.push_back(amesh);
//
//	HeightMap::toMesh(filename, amesh);
//
//	return amesh;
//}

Material *Resource::createMaterial(const NNEchar *name)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Material *>::iterator it = m_mtrlArray.begin(); it != m_mtrlArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	Material *mtrl = new Material(sid);
	m_mtrlArray.push_back(mtrl);

	return mtrl;
}

GPUProgram *Resource::createGPUProgram(const NNEchar *name, const NNEchar *comp)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<GPUProgram *>::iterator it = m_GPUProgArray.begin(); it != m_GPUProgArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	GPUProgram *sprog = new GPUProgram(sid, comp);
	sprog->init();

	m_GPUProgArray.push_back(sprog);

	return sprog;
}

GPUProgram *Resource::createGPUProgram(const NNEchar *name, const NNEchar *vert, const NNEchar *frag)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<GPUProgram *>::iterator it = m_GPUProgArray.begin(); it != m_GPUProgArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	GPUProgram *sprog = new GPUProgram(sid, vert, frag);
	sprog->init();

	m_GPUProgArray.push_back(sprog);

	return sprog;
}

GPUProgram *Resource::createGPUProgram(const NNEchar *name, const NNEchar *vert, const NNEchar *geom, const NNEchar *frag)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<GPUProgram *>::iterator it = m_GPUProgArray.begin(); it != m_GPUProgArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	GPUProgram *sprog = new GPUProgram(sid, vert, geom, frag);
	sprog->init();

	m_GPUProgArray.push_back(sprog);

	return sprog;
}

Texture *Resource::createTexture(const NNEchar *name, const NNEchar *filename, NNEuint unit /*= 0*/, NNEuint flag/*= 0*/)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Texture *>::iterator it = m_textureArray.begin(); it != m_textureArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	Texture *tex = new Texture(sid, unit, GL_TEXTURE_2D);
	m_textureArray.push_back(tex);
	debugAssert(NULL != tex->load(filename, flag));

	return tex;
}

Texture *Resource::createCubeMap(const NNEchar *name,
						   const NNEchar *x_pos_file,
						   const NNEchar *x_neg_file,
						   const NNEchar *y_pos_file,
						   const NNEchar *y_neg_file,
						   const NNEchar *z_pos_file,
						   const NNEchar *z_neg_file,
						   NNEuint unit/*= 0*/,
						   NNEuint flags/*= 0*/)
{
	Sid sid = HashString(name);
	debugExecute(
		for (Array<Texture *>::iterator it = m_textureArray.begin(); it != m_textureArray.end(); ++it)
		{
			debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
		}
	);

	Texture *tex = new Texture(sid, unit, GL_TEXTURE_CUBE_MAP);
	m_textureArray.push_back(tex);
	debugAssert(NULL != tex->load_cubemap(x_pos_file,
										x_neg_file,
										y_pos_file,
										y_neg_file,
										z_pos_file,
										z_neg_file,
										flags));

	return tex;
}

Font *Resource::createFont(const NNEchar *name, const NNEchar *filename)
{
	Sid sid = HashString(name);

#if defined(NNE_DEBUG)
	for (Array<Font *>::iterator it = m_fontArray.begin(); it != m_fontArray.end(); ++it)
	{
		debugAssertMsg((*it)->getNameSid() != sid, "name already exists");
	}

#endif

	Font *font = new Font(sid);

	NNEchar tmpname[64];
	strcpy(tmpname, filename);
	strcat(tmpname, ".jpg");
	Texture *tex = createTexture(name, tmpname);

	font->setTexture(tex);

	strcpy(tmpname, filename);
	strcat(tmpname, ".txt");
	font->loadFontInfo(tmpname);

	m_fontArray.push_back(font);

	return font;
}

Mesh *Resource::findMesh(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<Mesh *>::iterator it = m_meshArray.begin(); it != m_meshArray.end(); ++it)
	{
		if ((*it)->getNameSid() == sid)
		{
			return (*it);
		}
	}

	debugAssertMsg(false, "can't find Mesh");

	return NULL;
}

Material *Resource::findMaterial(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<Material *>::iterator it = m_mtrlArray.begin(); it != m_mtrlArray.end(); ++it)
	{
		if ((*it)->getNameSid() == sid)
		{
			return (*it);
		}
	}

	debugAssertMsg(false, "can't find Material");

	return NULL;
}

GPUProgram *Resource::findGPUProgram(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<GPUProgram *>::iterator it = m_GPUProgArray.begin(); it != m_GPUProgArray.end(); ++it)
	{
		if ((*it)->getNameSid() == sid)
		{
			return (*it);
		}
	}

	debugAssertMsg(false, "can't find GPUProgram");

	return NULL;
}

Texture *Resource::findTexture(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<Texture *>::iterator it = m_textureArray.begin(); it != m_textureArray.end(); ++it)
	{
		if ((*it)->getNameSid() == sid)
		{
			return (*it);
		}
	}

	debugAssertMsg(false, "can't find Texture");

	return NULL;
}

// have to define it once, let the compiler know it has already existed
Resource gResourceMngr;

} // namespace NNE