#ifndef NNE_CONSTRAINT_H
#define NNE_CONSTRAINT_H

#include "PlatformSpecs.h"

namespace NNE{

class Constraint
{
public:
	
	Constraint()
	{}

	~Constraint()
	{}

	virtual NNEvoid buildJacobian() = 0;

	virtual NNEvoid resolve(NNEfloat inv_dt) = 0;
};

} // manespace NNE

#endif // NNE_CONSTRAINT_H