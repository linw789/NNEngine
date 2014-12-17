#ifndef NNE_VECTOR_H
#define NNE_VECTOR_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"

namespace NNE{

/*
	general vector type with various size
*/
template<NNEuint s>
class Vector
{
public:
	
	Vector()
		:m_size(s)
	{}

	~Vector()
	{}

	NNEvoid setZero()
	{
		memset(m_val, 0, m_size * sizeof(NNEfloat));
	}

	NNEint getSize() const
	{
		return m_size;
	}

	// left value
	NNEfloat & operator[] (NNEuint i)
	{
		debugAssertMsg(i < m_size, "out of bound");
		return m_val[i];
	}

	// right value
	const NNEfloat & operator[] (NNEuint i) const
	{
		debugAssertMsg(i < m_size, "out of bound");
		return m_val[i];
	}

	NNEfloat operator* (const Vector<s> &v) const
	{
		NNEfloat result = 0;
		for (NNEuint i = 0; i < m_size; ++i)
		{
			result += m_val[i] * v.m_val[i];
		}

		return result;
	}

	Vector<s> operator * (NNEfloat f) const
	{
		Vector<s> v;
		for (NNEuint i = 0; i < m_size; ++i)
		{
			v[i] = m_val[i] * f;
		}
		return v;
	}

	Vector<s> operator + (const Vector<s> &other)
	{
		Vector<s> v;
		for (NNEuint i = 0; i < m_size; ++i)
		{
			v[i] = m_val[i] + other.m_val[i];
		}
		return v;
	}

private:

	NNEfloat m_val[s];

	NNEuint m_size;
};

} // manespace NNE

#endif // NNE_VECTOR_H