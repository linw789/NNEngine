#ifndef NNE_STRING_H
#define NNE_STRING_H

#include "PlatformSpecs.h"
#include <cstdio>

namespace NNE{

// string id, the hashed value of a string
typedef unsigned int Sid;

Sid HashString(const NNEchar *str);

} // manespace NNE

#endif // NNE_STRING_H