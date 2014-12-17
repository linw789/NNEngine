/*
	file: PlatformSpecs.h

	reference: G3D\platform.h
*/

#ifndef NNE_PLATFORMSPECS_H
#define NNE_PLATFORMSPECS_H

// detect 64-bit under various compiler
#if defined(_M_X64) || defined(_WIN64) || defined(__LP64__) || defined(_LP64)
#	define NNE_64BIT
#else
#	define NNE_32BIT
#endif

// detect compiler
#if defined(_MSC_VER)
#	define NNE_MSVC
#endif

// 
#if defined(WIN32) //&& defined(NNE_MSVC)
#	define NNE_WIN32
#endif

#if defined(WIN64)
#	define NNE_WIN64
#endif

#if defined(NNE_WIN32) || defined(NNE_WIN64)
#	define NNE_WINDOWS
#endif

// detect whether in debug mode
#if defined(_DEBUG)
#	define NNE_DEBUG
#   define NNE_DEBUG_GL_CALL
#elif defined(NDEBUG)
#	define NNE_RELEASE
#endif

#if defined(NNE_WIN32)

// eliminate unnecessary file in windows.h
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
// do not use min max macro defined in windows.h
#ifndef NOMINMAX
#	define NOMINMAX
#endif
// for windows version compatibility
// http://philippose.wordpress.com/2010/01/26/the-_win32_winnt-winver-ntddi_version-jumble/
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX

#endif

// if in debug mode, the application will associate a ternimal
// to the main display window for outputing debug message
#if defined(NNE_DEBUG)
#	define NNE_ASOCCIATE_TERMINAL
#endif

// using share_ptr 
#include <memory>

// compiler specific defines
#if defined(NNE_MSVC)
#	define NNE_FORCE_INLINE __forceinline
#endif

#define NNE_FLOAT_TOLERANCE 1e-8f
//keep BT_LARGE_FLOAT*BT_LARGE_FLOAT < FLT_MAX
#define NNE_LARGE_FLOAT 1e18f 

// using OpenGL API
#define NNE_OPENGL

#if defined(NNE_OPENGL)
#endif

namespace NNE
{
	//typedef enum NNEenum{};
	typedef unsigned int NNEuint;
	typedef void NNEvoid;
	typedef float NNEfloat;
	typedef int NNEint;
	typedef bool NNEbool;
	typedef unsigned char NNEuchar;
	typedef char NNEchar;
	typedef double NNEdouble;
	typedef __int64 NNEint64;

	enum IMAGE_FORMAT
	{
		NNE_AUTO = 0, 
		NNE_ONE_BYTE = 1,
		NNE_TWO_BYTE = 2,
		NNE_RGB = 3,
		NNE_RGBA = 4
	};

	enum IMAGE_OP
	{
		NNE_INVERT_Y = 1,
		NNE_MIPMAPS = 1 << 1,
	};

	// define vertex attribute index layout
	enum ATTRIB_TYPE
	{
		NNE_POSITION_ATTRIB  = 0,
		NNE_NORMAL_ATTRIB    = 1,
		NNE_TANGENT_ATTRIB   = 2,
		NNE_COLOR_ATTRIB     = 3,
		NNE_TEXCOORD0_ATTRIB = 4,
		NNE_TEXCOORD1_ATTRIB = 5,
		NNE_BONE_INDEX_ATTRIB = 6,
		NNE_BONE_WEIGHT_ATTRIB = 7
	};
}

#if defined(NNE_WINDOWS)
namespace NNE
{
	// NNEenum VK doesn't work
	enum VK
	{
		A = 0x41,
		D = 0x44,
		S = 0x53,
		W = 0x57,
	};

}
#endif

// typedef can be constrained in namespace

/*
	following #defines are intended to hide OpenGL API,
	so the engine could add DirectX support later in future.
*/
#if defined(NNE_OPENGL)
#include <GL/glew.h>

namespace NNE
{
	typedef GLenum  GPUenum;
	typedef GLint   GPUint;
	typedef GLuint  GPUuint;
	typedef GLvoid  GPUvoid;
	typedef GLsizei GPUsizei;
	typedef GLintptr GPUintptr;
	typedef GLsizeiptr GPUsizeiptr;
	typedef GLchar GPUchar;

	// first 10 binding points are reserved for the engine,
	// so the first 10 element of NNE_BindingPointPool will be true.
	enum UNIFORM_BINDING_POINT
	{
		SCENE_MATRICES = 0,
	};

	enum ENGINE_UNIFORM_LOCATION
	{
		PVW_MATRIX_UNIFORM_LOC = 0,
		MATERIAL_AMBIEMT_UNIFORM_LOC = 4,
		MATERIAL_DIFFUSE_UNIFORM_LOC = 5,
		MATERIAL_SPECULAR_UNIFORM_LOC = 6,
		MATERIAL_SHININESS_UNIFORM_LOC = 7
	};
}

#define GPU_FLOAT GL_FLOAT
#define GPU_UINT GL_UNSIGNED_INT

// ERROR CODE: INVALID_ENUM	= 0x0500
#define GPU_INVALID_ENUM 0x0500
#define GPU_INVALID_INDEX GL_INVALID_INDEX

// primitive type
#define GPU_TRIANGLES GL_TRIANGLES
#define GPU_TRIANGLE_TRIP GL_TRIANGLE_STRIP
#define GPU_LINES GL_LINES
#define GPU_LINE_STRIP GL_LINE_STRIP

// define tagert type of buffer object
#define GPU_VERTEX_BUFFER GL_ARRAY_BUFFER
#define	GPU_INDEX_BUFFER GL_ELEMENT_ARRAY_BUFFER
#define	GPU_UNIFORM_BUFFER GL_UNIFORM_BUFFER

// define usage hint of buffer object
#define	GPU_WRITE_ONCE GL_STATIC_DRAW
#define	GPU_WRITE_EVERY_FRAME GL_STREAM_DRAW
#define	GPU_WRITE_EVERY_FEW_FRAMES GL_DYNAMIC_DRAW

#endif

#endif // NNE_PLATFORMSPECS_H