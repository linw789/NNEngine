#ifndef NNE_SHADER_H
#define NNE_SHADER_H

#include "PlatformSpecs.h"
#include "Array.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

namespace NNE{

using std::ifstream;
using std::istreambuf_iterator;
using std::endl;

class Shader
{
public:

	Shader()
	{}

	~Shader()
	{}

	static GPUuint fromFile(const std::string &comp);

	// create shader program from vertex and fragment shader files
	static GPUuint fromFile(const std::string &vert, const std::string &frag);

	// create shader program from geometry, vertex and fragment shader files
	static GPUuint fromFile(const std::string &vert, const std::string &geom, const std::string &frag);

	static GPUuint createShader(const std::string &filename);

	// shaderList will call begin() which is not the const specified function
	static GPUuint createProgram(/*const */Array<GLuint> &shaderList);

	static const std::string parseShaderFile(const std::string &filename);
};

} // namespace NNE

#endif // NNE_SHADER_H
