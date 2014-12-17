#ifndef NNE_MATRIX3_H
#define NNE_MATRIX3_H

#include "PlatformSpecs.h"
#include "Vec3.hpp"
#include "Quaternion.h"

namespace NNE{

/*
	column-major
*/
class Mat3
{
public:

	Mat3()
	{
		memset(m_val, 0, 9 * sizeof(NNEfloat));
	}

	Mat3(const Vec3 &col0, const Vec3 &col1, const Vec3 &col2)
	{
		m_val[0][0] = col0[0]; m_val[0][1] = col0[1]; m_val[0][2] = col0[2];
		m_val[1][0] = col1[0]; m_val[1][1] = col1[1]; m_val[1][2] = col1[2];
		m_val[2][0] = col2[0]; m_val[2][1] = col2[1]; m_val[2][2] = col2[2];
	}

	Mat3(float val00, float val01, float val02, // first column
		 float val10, float val11, float val12, // second column
		 float val20, float val21, float val22) // third column
	{
		// first column
		m_val[0][0] = val00; m_val[0][1] = val01; m_val[0][2] = val02;
		// second column
		m_val[1][0] = val10; m_val[1][1] = val11; m_val[1][2] = val12;
		// third column
		m_val[2][0] = val20; m_val[2][1] = val21; m_val[2][2] = val22;
	}

	Mat3(const Mat3 &mat)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m_val[i][j] = mat.m_val[i][j];
			}
		}
	}

	Mat3(const Quaternion &q)
	{
		NNEfloat x2 = q.x * q.x;
		NNEfloat y2 = q.y * q.y;
		NNEfloat z2 = q.z * q.z;
		NNEfloat xy = q.x * q.y;
		NNEfloat yz = q.y * q.z;
		NNEfloat zx = q.z * q.x;
		NNEfloat wx = q.w * q.x;
		NNEfloat wy = q.w * q.y;
		NNEfloat wz = q.w * q.z;

		m_val[0][0] = 1 - 2 * y2 - 2 * z2;
		m_val[0][1] = 2 * xy + 2 * wz;
		m_val[0][2] = 2 * zx - 2 * wy;
		m_val[1][0] = 2 * xy - 2 * wz;
		m_val[1][1] = 1 - 2 * x2 - 2 * z2;
		m_val[1][2] = 2 * yz + 2 * wx;
		m_val[2][0] = 2 * zx + 2 * wy;
		m_val[2][1] = 2 * yz - 2 * wx;
		m_val[2][2] = 1 - 2 * x2 - 2 * y2;
	}

	~Mat3(){}

	// override operators
	Mat3 & operator = (const Mat3 &mat)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m_val[i][j] = mat.m_val[i][j];
			}
		}

		return *this;
	}

	Mat3 operator * (const Mat3 &other) const
	{
		Mat3 result;

		for (int r = 0; r < 3; ++r) 
		{
			for (int c = 0; c < 3; ++c)
			{
				for (int i = 0; i < 3; ++i) 
				{
					result.m_val[c][r] += m_val[i][r] * other.m_val[c][i];
				}
			}
		}

		return result;
	}

	Mat3 operator * (NNEfloat f) const
	{
		Mat3 result;
		for (NNEint i = 0; i < 3; ++i)
		{
			for (NNEint j = 0; j < 3; ++j)
			{
				result.m_val[i][j] = m_val[i][j] * f;
			}
		}

		return result;
	}

	NNEvoid identity()
	{
		// first column
		m_val[0][0] = 1.0f; m_val[0][1] = 0; m_val[0][2] = 0;
		// second column
		m_val[1][0] = 0; m_val[1][1] = 1.0f; m_val[1][2] = 0;
		// third column
		m_val[2][0] = 0; m_val[2][1] = 0; m_val[2][2] = 1.0f;
	}

	NNEvoid zero()
	{
		memset(m_val, 0, 9 * sizeof(NNEfloat));
	}

	NNEvoid setDiagonal(const Vec3 &d)
	{
		m_val[0][0] = d.x; m_val[1][1] = d.y; m_val[2][2] = d.z;
	}

	// this also allow [][] indexing, because the return value can be indexed again.
	const NNEfloat* operator[](NNEint c) const 
	{
        debugAssertMsg(c >= 0, "indexing out bound");
        debugAssertMsg(c < 3, "indexing out bound");
        return (const float*)&m_val[c];
    } 

	// return value above will be used as right value, this will be used as left value
	NNEfloat* operator[](NNEint c)
	{
        debugAssertMsg(c >= 0, "indexing out bound");
        debugAssertMsg(c < 3, "indexing out bound");
        return (float*)&m_val[c];
    } 

	NNEvoid transpose()
	{
		float tmp;
		tmp = m_val[0][1];
		m_val[0][1] = m_val[1][0];
		m_val[1][0] = tmp;

		tmp = m_val[0][2];
		m_val[0][2] = m_val[2][0];
		m_val[2][0] = tmp;

		tmp = m_val[1][2];
		m_val[1][2] = m_val[2][1];
		m_val[2][1] = tmp;
	}

	static Mat3 transpose(const Mat3 &m3)
	{
		Mat3 tmp = m3;
		tmp.transpose();
		return tmp;
	}

	static Mat3 inverse(const Mat3 m3)
	{
		//http://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

		NNEfloat tmp00 = m3[1][1] * m3[2][2] - m3[1][2] * m3[2][1];
		NNEfloat tmp01 = m3[1][0] * m3[2][2] - m3[1][2] * m3[2][0];
		NNEfloat tmp02 = m3[1][0] * m3[2][1] - m3[1][1] * m3[2][0];
		NNEfloat tmp10 = m3[0][1] * m3[2][2] - m3[0][2] * m3[2][1];
		NNEfloat tmp11 = m3[0][0] * m3[2][2] - m3[0][2] * m3[2][0];
		NNEfloat tmp12 = m3[0][0] * m3[2][1] - m3[0][1] * m3[2][0];
		NNEfloat tmp20 = m3[0][1] * m3[1][2] - m3[0][2] * m3[1][1];
		NNEfloat tmp21 = m3[0][0] * m3[1][2] - m3[0][2] * m3[1][0];
		NNEfloat tmp22 = m3[0][0] * m3[1][1] - m3[0][1] * m3[1][0];

		NNEfloat determinant = m3[0][0] * tmp00 - m3[1][0] * tmp10 + m3[2][0] * tmp20;
		NNEfloat oneOverD = 1 / determinant;

		return Mat3(tmp00 * oneOverD, -tmp10 * oneOverD, tmp20 * oneOverD,
					-tmp01 * oneOverD, tmp11 * oneOverD, -tmp21 * oneOverD,
				    tmp02 * oneOverD, -tmp12 * oneOverD, tmp22 * oneOverD);
	}

	NNEvoid inverse()
	{

	}

	Quaternion toQuaternion()
	{
		Quaternion q;

		// sum of diagonal
		NNEfloat trace = m_val[0][0] + m_val[1][1] + m_val[2][2];
		NNEfloat s;

		if (trace > 0)
		{
			s = sqrt(trace + 1.0f);
			q.w = 0.5f * s;
			s = 0.5f / s;
			q.x = (m_val[1][2] - m_val[2][1]) * s;
			q.y = (m_val[2][0] - m_val[0][2]) * s;
			q.z = (m_val[0][1] - m_val[1][0]) * s;
		}
		else
		{
			NNEint i = 0;

			if (m_val[1][1] > m_val[0][0])
				i = 1;
			if (m_val[2][2] > m_val[1][1])
				i = 2;

			switch(i)
			{
			case 0:
				s = sqrt((m_val[0][0] - (m_val[1][1] + m_val[2][2])) + 1);
				q.x = 0.5f * s;
				s = 0.5f / s;
				q.y = (m_val[1][0] + m_val[0][1]) * s;
				q.z = (m_val[0][2] + m_val[2][0]) * s;
				q.w = (m_val[1][2] - m_val[2][1]) * s;
				break;

			case 1:
				s = sqrt((m_val[1][1] - (m_val[2][2] + m_val[0][0])) + 1);
				q.y = 0.5f * s;
				s = 0.5f / s;
				q.z = (m_val[2][1] + m_val[1][2]) * s;
				q.x = (m_val[1][0] + m_val[0][1]) * s;
				q.w = (m_val[2][0] - m_val[0][2]) * s;
				break;

			case 2:
				s = sqrt((m_val[2][2] - (m_val[0][0] + m_val[1][1])) + 1);
				q.z = 0.5f * s;
				s = 0.5f / s;
				q.x = (m_val[0][2] + m_val[2][0]) * s;
				q.y = (m_val[2][1] + m_val[1][2]) * s;
				q.w = (m_val[0][1] - m_val[1][0]) * s;
			}
		}
		// q is already a unit length quaternion after above conversion

		return q;
	}

	Vec3 getAxisX() const
	{
		return Vec3(m_val[0][0], m_val[0][1], m_val[0][2]);
	}

	NNEvoid setAxisX(const Vec3 v3)
	{
		m_val[0][0] = v3.x;
		m_val[0][1] = v3.y;
		m_val[0][2] = v3.z;
	}

	Vec3 getAxisY() const
	{
		return Vec3(m_val[1][0], m_val[1][1], m_val[1][2]);
	}

	NNEvoid setAxisY(const Vec3 v3)
	{
		m_val[1][0] = v3.x;
		m_val[1][1] = v3.y;
		m_val[1][2] = v3.z;
	}

	Vec3 getAxisZ() const
	{
		return Vec3(m_val[2][0], m_val[2][1], m_val[2][2]);
	}

	NNEvoid setAxisZ(const Vec3 v3)
	{
		m_val[2][0] = v3.x;
		m_val[2][1] = v3.y;
		m_val[2][2] = v3.z;
	}

private:

	NNEfloat m_val[3][3];

};

// override global operators
inline Vec3 operator * (const Mat3& m3, const Vec3 &v3)
{
	return Vec3(m3[0][0] * v3.x + m3[1][0] * v3.y + m3[2][0] * v3.z,
				m3[0][1] * v3.x + m3[1][1] * v3.y + m3[2][1] * v3.z,
				m3[0][2] * v3.x + m3[1][2] * v3.y + m3[2][2] * v3.z);
}

inline Vec3 operator * (const Vec3 &v3, const Mat3 &mat)
{
	return Vec3(v3.x * mat[0][0] + v3.y * mat[0][1] + v3.z * mat[0][2],
				v3.x * mat[1][0] + v3.y * mat[1][1] + v3.z * mat[1][2],
				v3.x * mat[2][0] + v3.y * mat[2][1] + v3.z * mat[2][2]);
}

inline Mat3 operator * (NNEfloat f, const Mat3 &m3)
{
	return m3 * f;
}

}

#endif // NNE_MATRIX3_H