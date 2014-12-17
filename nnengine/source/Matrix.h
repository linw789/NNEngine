#ifndef NNE_MATRIX_H
#define NNE_MATRIX_H

#include "PlatformSpecs.h"
#include "Vector.h"

namespace NNE{

/*
	n by n matrix stored as row major, matrix[row][column]
*/
template<NNEuint r, NNEuint c>
class Matrix
{
public:
	
	Matrix()
		:m_rows(r), m_columns(c)
	{}

	NNEvoid setZero()
	{
		memset(m_val, 0, m_rows * m_columns * sizeof(NNEfloat));
	}

	// get the number of rows and columns
	NNEvoid getSize(NNEuint *r, NNEuint *c) const
	{
		*r = m_rows;
		*c = m_columns;
	}	

	NNEfloat *operator[] (NNEuint i)
	{
		return m_val[i];
	}

	const NNEfloat *operator[] (NNEuint i) const
	{
		return m_val[i];
	}

	~Matrix()
	{}

private:

	NNEfloat m_val[r][c];

	NNEuint m_rows, m_columns;
};

template<NNEuint s>
class DiagonalMatrix
{
public:

	DiagonalMatrix()
		:m_size(s)
	{}

	~DiagonalMatrix()
	{}

	NNEvoid setZero()
	{
		memset(m_val, 0, m_size * sizeof(NNEfloat));
	}

	NNEfloat &operator[] (NNEuint i)
	{
		return m_val[i];
	}

	const NNEfloat &operator[] (NNEuint i) const
	{
		return m_val[i];
	}

	Vector<s> operator* (const Vector<s> &v) const
	{
		Vector<s> result;
		for (NNEuint i = 0; i < m_size; ++i)
		{
			result[i] = m_val[i] * v[i];
		}

		return result;
	}

private:

	NNEfloat m_val[s];

	NNEuint m_size;
};

template<NNEuint s>
inline Vector<s> operator* (const Vector<s> &v, const DiagonalMatrix<s> &m)
{
	return m * v;
}

} // manespace NNE

#endif // NNE_MATRIX_H