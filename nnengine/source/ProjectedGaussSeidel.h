#ifndef NNE_PROJECTEDGAUSSSEIDEL_H
#define NNE_PROJECTEDGAUSSSEIDEL_H

#include "PlatformSpecs.h"
#include "Matrix.h"
#include "Vector.h"

namespace NNE{

class ProjectedGaussSeidel
{
	enum
	{
		MAX_PGS_ITERATION = 11,
	};

public:
	
	ProjectedGaussSeidel()
	{}

	~ProjectedGaussSeidel()
	{}
	
	// the Gauss-Seidel will converge when the A is positive definite or strictly diagonally dominant,
	// otherwise it may or may not converge
	template<NNEuint s>
	NNEbool solve(const Matrix<s, s> &A, const Vector<s> &b, const Vector<s> &guess, Vector<s> &result);
};

template<NNEuint s>
inline NNEbool ProjectedGaussSeidel::solve(const Matrix<s, s> &A, const Vector<s> &b, const Vector<s> &guess, Vector<s> &result)
{
	result = guess;
	NNEfloat tmpsum = 0;
	 
	for (NNEuint itr = 0; itr < MAX_PGS_ITERATION; ++itr)
	{
		for (NNEuint i = 0; i < s; ++i)
		{
			tmpsum = 0;
			for (NNEuint j = 0; j < i; ++j)
			{
				NNEfloat a = A[i][j];
				NNEfloat b = result[j];
				tmpsum += A[i][j] * result[j];
			}
			for (NNEuint k = i + 1; k < s; ++k)
			{
				NNEfloat a = A[i][k];
				NNEfloat b = result[k];
				tmpsum += A[i][k] * result[k];
			}

			result[i] = (b[i] - tmpsum) / A[i][i];
		}

		// test if converged

	}

	return true;
}

} // manespace NNE

#endif // NNE_PROJECTEDGAUSSSEIDEL_H