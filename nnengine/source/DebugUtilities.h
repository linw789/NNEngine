/*
	file: DebugAssert.h
	
	reference: G3D/debugAssert.h
*/

#ifndef NNE_DEBUGASSERT_H
#define NNE_DEBUGASSERT_H

// in order to use following include path, you have to
// add additional include path in project properties
// #include <GL/glew.h>
#include "../../thirdparty/include/GL/glew.h"
#include <iostream>

#include "PlatformSpecs.h"
#include <string>

// #define and other pre-processor directives are not
// restrict to namespace, so no point to add them in side
// namespace NNE{}

#	if defined(_MSC_VER)
#		define rawBreak() ::DebugBreak()
#	elif defined(__i386__) 
	    // gcc on intel
#		define rawBreak() __asm__ __volatile__ ("int $3")
#	else
		// some other gcc
#		define rawBreak() ::abort()
#	endif // 

#	define debugBreak() rawBreak()

// debug assert
// HAVE TO put a forward slash in the blank line
// DO NOT assert any character after the forward slash
#if defined(NNE_DEBUG)
#	define debugAssert(exp) \
	if (!(exp)) \
	{ \
		rawBreak(); \
	}

#	define debugAssertMsg(exp, message) \
	if(!(exp)) \
	{ \
		std::cerr << "\nDebug Error File: " << __FILE__ << " Line: " << __LINE__ << '\n' \
			<< message; \
			\
		rawBreak(); \
	}
#	define alwaysAssertMsg(exp, message) debugAssertMsg(exp, message)

#else // release mode // release assert
	// alwaysAssertMsg will be executed even in release mode
#	define alwaysAssertMsg(exp, message) \
		if(!(exp))\
		{\
			std::cerr << "*Debug Error\n*file: " << __FILE__ << "\n*line: " << __LINE__ << '\n'\
				<< message << '\n'\
				<< "************\n\n";\
			rawBreak();\
		}

#	define debugAssertMsg(exp, message)

#endif // NNE_DEBUG

// create a debug terminal(console), and direct IO to it
#if defined(NNE_DEBUG) && defined(NNE_WIN32)
#	include <stdio.h>	
#	define allocDebugConsole() \
		AllocConsole(); \
		freopen("CONOUT$", "w", stderr); 
#else
#	define allocDebugConsole()
#endif

// debug output
#if defined(NNE_DEBUG)
#	define debugOutput(message) \
		std::cerr << __FILE__ << "\nLine: " << __LINE__ << "\n" \
				  << message << "\n\n";

#	define debugOutputOnceIf(cond, message) \
		static NNEbool outputFlag = false; \
		if (cond) \
		{ \
			if (false == outputFlag) \
			{ \
				debugOutput(message); \
			} \
			outputFlag = true; \
		}

#else
#	define debugOutput(message)

#endif // NNE_DEBUG

// debug OpenGL calls
#if defined(NNE_DEBUG_GL_CALL)
namespace NNE{

inline std::string getGL_ERROR(GLenum err)
{
	switch(err)
	{
	case GL_INVALID_ENUM:
		return std::string("GL_INVALID_ENUM");
	case GL_INVALID_VALUE:
		return std::string("GL_INVALID_VALUE");
	case GL_INVALID_OPERATION:
		return std::string("GL_INVALID_OPERATION");
	case  GL_STACK_OVERFLOW:
		return std::string("GL_STACK_OVERFLOW");
	case GL_STACK_UNDERFLOW:
		return std::string("GL_STACK_UNDERFLOW");
	case GL_OUT_OF_MEMORY:
		return std::string("GL_OUT_OF_MEMORY");
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");
	case GL_TABLE_TOO_LARGE:
		return std::string("GL_TABLE_TOO_LARGE");
	default:
		return std::string("INVALIDE_ERROR_CODE");
	}
}

} // namespace

#	define GL_DEBUG(gl_call) \
	{ \
		gl_call; \
		GLenum glError = glGetError(); \
		debugAssertMsg(glError == GL_NO_ERROR, "OpenGL call failed! " << NNE::getGL_ERROR(glError)); \
	}
#else
#	define GL_DEBUG(gl_call) gl_call	
#endif

// execute the statement or expression only in debug mode
#if defined(NNE_DEBUG)
#	define debugExecute(expression) \
	{ \
		expression; \
	}
#else
#	define debugExecute(expression) 
#endif

#endif // NNE_DEBUGASSERT_H