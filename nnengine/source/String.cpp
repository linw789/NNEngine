#include "String.h"
#include "../dependencies/MurmurHash3.h"

namespace NNE{


// why the seed parameter of MurmurHash
// http://stackoverflow.com/questions/9241230/what-is-murmurhash3-seed-parameter
Sid HashString(const NNEchar *string)
{
	size_t length = strlen(string);
	NNEuint hashVal;
	MurmurHash3_x86_32(string, length, 655428, &hashVal);

	return hashVal;
}

} // namespace NNE