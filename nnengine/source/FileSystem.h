#ifndef NNE_FILESYSTEM_H
#define NNE_FILESYSTEM_H

#include "source/PlatformSpecs.h"

namespace NNE{

class FileSystem
{
public:
	
	FileSystem()
	{}

	~FileSystem()
	{}

private:

	NNEchar *m_meshPath;

	NNEchar *m_shaderPath;

	NNEchar *m_texturePath;
};

} // manespace NNE

#endif // NNE_FILESYSTEM_H