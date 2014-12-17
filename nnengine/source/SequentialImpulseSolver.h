#ifndef NNE_SEQUENTIAL_IMPULSE_SOLVER_H
#define NNE_SEQUENTIAL_IMPULSE_SOLVER_H

#include "PlatformSpecs.h"
#include "ContactPoint.h"
#include "Matrix.h"
#include "Vector.h"

namespace NNE{

class SequentialImpulseSolver
{
public:
	
	SequentialImpulseSolver()
	{}

	~SequentialImpulseSolver()
	{}

	/*
	P(impulse) = deltaTime * Force = deltaVelocity * M
	v2 = v1 + P / M
	P = J * lamda
	J * v2 + baumgart = 0
	*/
	NNEbool resolve(ContactPoint *cm);

private:

	Vector<12> m_jacobian;

	DiagonalMatrix<12> m_diagInvMass;

	Vector<12> m_oldVel;

};

} // manespace NNE

#endif // NNE_SEQUENTIAL_IMPULSE_SOLVER_H