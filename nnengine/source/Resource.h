#ifndef NNE_RESOURCE_H
#define NNE_RESOURCE_H

#include "PlatformSpecs.h"
#include "Array.h"

namespace NNE{

class Mesh;
class ClothMesh;
class Skin;
class Animation;
class Material;
class Texture;
class GPUProgram;
class Texture;
class Font;

class Resource
{
	friend class AssimpLoader;
	friend class Game;

	enum
	{
		MAX_UNIFORM_BINDING_POINTS = 64,
	};

public:

	enum
	{
		DEBUG_DRAW_LINE_GPUP = 0,
		DRAW_NORMAL_GPUP = 1,
		DEBUG_DRAW_POINT_GPUP = 2,
	};

public:

	Resource()
	{}

	~Resource()
	{}

	bool init();

	// get a unused new uniform binding point
	NNEuint getNewUniformBindingPoint() 
	{
		for (NNEint i = 10; i < MAX_UNIFORM_BINDING_POINTS; ++i)
		{
			if (false == m_bindingPointPool[i])
			{
				m_bindingPointPool[i] = true;
				return i;
			}
		}
		 
		debugAssertMsg(false, "binding points are used up");
		return MAX_UNIFORM_BINDING_POINTS + 1;
	}

	Mesh *createMesh(const NNEchar *name, const NNEchar *filename);

	Mesh *createMesh(const NNEchar *name);

	ClothMesh *createClothMesh(const NNEchar *name, NNEuint w, NNEuint h, NNEfloat interval);

	Skin *createSkin(const NNEchar *name, const NNEchar *filename);

	Animation *createAnimation(const NNEchar *name, const NNEchar *filename);

	//Mesh *createMeshFromHeightMap(const NNEchar *name, const NNEchar *heightmap);

	Material *createMaterial(const NNEchar *name);

	GPUProgram *createGPUProgram(const NNEchar *name, const NNEchar *comp);

	GPUProgram *createGPUProgram(const NNEchar *name, const NNEchar *vert, const NNEchar *frag);

	GPUProgram *createGPUProgram(const NNEchar *name, const NNEchar *vert, const NNEchar *geom, const NNEchar *frag);

	Texture *createTexture(const NNEchar *name, const NNEchar *filename, NNEuint unit = 0, NNEuint flag = 0);

	Texture *createCubeMap(const NNEchar *name,
						   const NNEchar *x_pos_file,
						   const NNEchar *x_neg_file,
						   const NNEchar *y_pos_file,
						   const NNEchar *y_neg_file,
						   const NNEchar *z_pos_file,
						   const NNEchar *z_neg_file,
						   NNEuint unit = 0,
						   NNEuint flags = 0);

	Font *createFont(const NNEchar *name, const NNEchar *filename);

	Mesh *findMesh(const NNEchar *name);

	Material *findMaterial(const NNEchar *name);

	GPUProgram *findGPUProgram(const NNEchar *name);

	GPUProgram *findGPUProgram(NNEuint i)
	{
		return m_GPUProgArray[i];
	}

	Texture *findTexture(const NNEchar *name);

	GPUProgram *getDrawNormalGPUProgram()
	{
		// always in the second place
		return m_GPUProgArray[1];
	}

	Font *getDefaultFont()
	{
		return m_fontArray[0];
	}

private:

	// uncopyable
	Resource(const Resource &);
	Resource &operator = (const Resource &);

	Array<Mesh *> m_meshArray; 
	Array<Skin *> m_skinArray;
	Array<Animation *> m_animArray;
	Array<Material *> m_mtrlArray;
	Array<Texture *> m_textureArray;
	Array<GPUProgram *> m_GPUProgArray;
	Array<Font *> m_fontArray;

	// uniform binding pionts pool
	// first 10 binding points are reserved for the engine,
	// so the first 10 element of NNE_BindingPointPool will be true.
	NNEbool m_bindingPointPool[MAX_UNIFORM_BINDING_POINTS];
};

// Declare global objects so they can be accessed through out the engine.
// Declare extern in one header file and define them in one source file,
// then you can directly use them in other source files.
extern Resource gResourceMngr;

} // namespace NNE

#endif // NNE_RESOURCE_H