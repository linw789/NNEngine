#include "Shader.h"
#include "Array.h"
#include "DebugUtilities.h"

namespace NNE{

GPUuint Shader::fromFile(const std::string &comp)
{
	Array<GPUuint> shaderList;
	shaderList.push_back(createShader(comp));

	return createProgram(shaderList);
}

GPUuint Shader::fromFile(const std::string &vert, const std::string &frag)
{
	Array<GPUuint> shaderList;
	shaderList.push_back(createShader(vert));
	shaderList.push_back(createShader(frag));

	return createProgram(shaderList);
}

GPUuint Shader::fromFile(const std::string &vert, const std::string &geom, const std::string &frag)
{
	Array<GPUuint> shaderList;
	shaderList.push_back(createShader(vert));
	shaderList.push_back(createShader(geom));
	shaderList.push_back(createShader(frag));

	GPUuint programId = createProgram(shaderList);

	return programId;
}

GPUuint Shader::createShader(const std::string &filename)
{
	//compare file's extension name
	std::string shaderName(filename);
	size_t found = shaderName.rfind('.');

	GLuint shaderId = 0;
	GLenum shaderType;
	if (shaderName.compare(found + 1, 4, "vert") == 0)
	{
		shaderType = GL_VERTEX_SHADER;
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderName.compare(found + 1, 4, "frag") == 0)
	{
		shaderType = GL_FRAGMENT_SHADER;
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else if (shaderName.compare(found + 1, 4, "geom") == 0)
	{
		shaderType = GL_GEOMETRY_SHADER;
		shaderId = glCreateShader(GL_GEOMETRY_SHADER);
	}
	else if (shaderName.compare(found + 1, 4, "comp") == 0)
	{
		shaderType = GL_COMPUTE_SHADER;
		shaderId = glCreateShader(GL_COMPUTE_SHADER);
	}
	else 
	{
		return 0;
	}

	const std::string shaderCode = parseShaderFile(filename);
	const char *code = shaderCode.c_str();

	glShaderSource(shaderId, 1, &code, NULL);
	glCompileShader(shaderId);

	GLint compileStatus;//whether compilation succeeds
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	if(GL_FALSE == compileStatus)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog);

		std::cerr << "Compile failure in " << shaderName << ": " << std::endl
			<< infoLog << std::endl << std::endl;

		delete [] infoLog;
	}

	return shaderId;
}

GPUuint Shader::createProgram(Array<GLuint> &shaderList)
{
	GPUuint programId = glCreateProgram();

	for (Array<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		GL_DEBUG(glAttachShader(programId, *it));
		GL_DEBUG(glDeleteShader((*it)));
	}

	GL_DEBUG(glLinkProgram(programId));

	GLint linkStatus;//whether link succeeds
	GL_DEBUG(glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus));
	if (GL_FALSE == linkStatus)
	{
		GLint infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength + 1];

		glGetProgramInfoLog(programId, infoLogLength, NULL, infoLog);

		std::cerr << "Link failure:" << std::endl
			<< infoLog << std::endl <<std::endl;

		delete []infoLog;
	}

	// detach shaders
	for (Array<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		GL_DEBUG(glDetachShader(programId, *it));
	}

	return programId;
}

const std::string Shader::parseShaderFile(const std::string &filename)
{
	ifstream infile(filename, ifstream::in);
	if (!infile.is_open())
	{
		std::cerr << "Cannot open the given file " << filename << std::endl;
		return "file error!";
	}

	const std::string codeStr((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
	//brilliant way to retrieve string from file, learned from web
	//mind the extra parenthesis in the declaration is necessary for correct parsing.

	infile.close();

	return codeStr;
}

} // namespace NNE