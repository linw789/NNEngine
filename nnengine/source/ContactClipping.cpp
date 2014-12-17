#include "ContactClipping.h"

namespace NNE{

NNEvoid ContactClipping::clipAgainstPlane(const Array<Vec3> &vertIn, Array<Vec3> &vertOut, 
	const Vec3 &normal, NNEfloat dist)
{
	// the normal of the plane is considered pointing outward and vertices
	// on the out side of the plane will be clipped

	NNEuint vertInSize = vertIn.size();
	if (vertInSize < 2)
		return;

	Vec3 v_start = vertIn[vertInSize - 1];
	Vec3 v_end;

	// d_start and d_end are signed distances between v_start, v_end and the plane
	// if d_start > 0, v_start is on the side the normal points to, outside clipped
	NNEfloat d_start = Vec3::dot(normal, v_start) - dist;
	NNEfloat d_end = 0;

	for (NNEuint i = 0; i < vertInSize; ++i)
	{
		v_end = vertIn[i];
		d_end = Vec3::dot(normal, v_end) - dist;

		if (d_start > 0)
		{
			if (d_end == 0)
			{
				vertOut.push_back(v_end);
			}
			else if (d_end < 0)
			{
				NNEfloat r_start = d_start / (d_start - d_end);
				NNEfloat r_end = 1 - r_start;
				// add intersection 
				vertOut.push_back(v_end * r_start + v_start * r_end);
				vertOut.push_back(v_end);
			}
		}
		else if (d_start < 0)
		{
			if (d_end <= 0)
			{
				vertOut.push_back(v_end);
			}
			else
			{
				NNEfloat r_end = d_end / (d_end - d_start);
				NNEfloat r_start = 1 - r_end;
				vertOut.push_back(v_end * r_start + v_start * r_start);
			}
		}

		v_start = v_end;
		d_start = d_end;
	}
}

} // namespace NNE