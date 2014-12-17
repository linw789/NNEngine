#ifndef NNE_MATRIX4_H
#define NNE_MATRIX4_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Matrix3.hpp"
#include "Quaternion.h"

namespace NNE{

/*
	The 4x4 matrix's memory layout is set to column major.
	If m is a matrix4, m[0][0 - 3] form the first colum, 
	m[3][0 - 3] form the fourth column. 
	It can be visually represented as follow.
	
	| m00  m10  m20  m30 | 
	| m01  m11  m21  m31 |
	| m02  m12  m22  m32 |	
	| m03  m13  m23  m33 |
							
	| ic  jc  kc  tc |		
	| 0   0   0   1  |			

	ic is the unit basis vector along the child space x-axis, expressed in world space.
	jc is the unit basis vector along the child space y-axis, expressed in world space.
	kc is the unit basis vector along the child space z-axis, expressed in world space.
	tc is the translation of he child coordinate system relative to parent space.

	The above rule enforces the pre-multiplication( mat4x4 * vec4 ) when we need to
	transform a point or a directional vector from child space to parent space.

	reference on OpenGL official site:
	For programming purposes, OpenGL matrices are 16-value arrays with base vectors 
	laid out contiguously in memory. The translation components occupy the 13th, 14th, 
	and 15th elements of the 16-element matrix, where indices are numbered from 1 to 16 
	as described in section 2.11.2 of the OpenGL 2.1 Specification.
*/
class Mat4
{
public:

	// It's not necessary to make Mat4's destructor a virtual function,
	// because Mat4 will never manipulate its child class(Node).
	// virtual function would add a virtual table to this class, therefore
	// the first memory address of the instance of the class is virtual table instead
	// of the m_val
	/*virtual */~Mat4()
	{}

	// constructors
	Mat4()
	{
		memset(m_val, 0, sizeof(NNEfloat) * 16);
	}

	Mat4(float val00, float val10, float val20, float val30, // first row
		 float val01, float val11, float val21, float val31, // second row
		 float val02, float val12, float val22, float val32, // third row
		 float val03, float val13, float val23, float val33) // fourth row
	{
		// first column
		m_val[0][0] = val00; m_val[0][1] = val01; m_val[0][2] = val02; m_val[0][3] = val03;
		// second column
		m_val[1][0] = val10; m_val[1][1] = val11; m_val[1][2] = val12; m_val[1][3] = val13;
		// third column
		m_val[2][0] = val20; m_val[2][1] = val21; m_val[2][2] = val22; m_val[2][3] = val23;
		// fourth column
		m_val[3][0] = val30; m_val[3][1] = val31; m_val[3][2] = val32; m_val[3][3] = val33;
	}

	Mat4(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2, const Vec4 &v3)
	{
		// first column
		m_val[0][0] = v0.x; m_val[0][1] = v0.y; m_val[0][2] = v0.z; m_val[0][3] = v0.w;
		// second column
		m_val[1][0] = v1.x; m_val[1][1] = v1.y; m_val[1][2] = v1.z; m_val[1][3] = v1.w;
		// third column
		m_val[2][0] = v2.x; m_val[2][1] = v2.y; m_val[2][2] = v2.z; m_val[2][3] = v2.w;
		// fourth column
		m_val[3][0] = v3.x; m_val[3][1] = v3.y; m_val[3][2] = v3.z; m_val[3][3] = v3.w;
	}

	Mat4(const float *val)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_val[i][j] = val[i * 4 + j];
			}
		}
	}

	Mat4(const Mat4 &mat)
	{
		// first column
		m_val[0][0] = mat[0][0]; m_val[0][1] = mat[0][1]; m_val[0][2] = mat[0][2]; m_val[0][3] = mat[0][3];
		// second column
		m_val[1][0] = mat[1][0]; m_val[1][1] = mat[1][1]; m_val[1][2] = mat[1][2]; m_val[1][3] = mat[1][3];
		// third column
		m_val[2][0] = mat[2][0]; m_val[2][1] = mat[2][1]; m_val[2][2] = mat[2][2]; m_val[2][3] = mat[2][3];
		// fourth column
		m_val[3][0] = mat[3][0]; m_val[3][1] = mat[3][1]; m_val[3][2] = mat[3][2]; m_val[3][3] = mat[3][3];
	}

	Mat4(const Mat3 &m3)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m_val[i][j] = m3[i][j];
			}
		}

		m_val[3][0] = 0.0f;
		m_val[3][1] = 0.0f;
		m_val[3][2] = 0.0f;
		m_val[0][3] = 0.0f;
		m_val[1][3] = 0.0f;
		m_val[2][3] = 0.0f;
		m_val[3][3] = 1.0f;
	}

	Mat4(NNEfloat val)
	{
		m_val[0][0] = val; m_val[0][1] = val; m_val[0][2] = val; m_val[0][3] = val;
		m_val[1][0] = val; m_val[1][1] = val; m_val[1][2] = val; m_val[1][3] = val;
		m_val[2][0] = val; m_val[2][1] = val; m_val[2][2] = val; m_val[2][3] = val;
		m_val[3][0] = val; m_val[3][1] = val; m_val[3][2] = val; m_val[3][3] = val;
	}

	Mat4(const Quaternion &quat)
	{
		Quaternion q = Quaternion::normalize(quat);

		NNEfloat x2 = q.x * q.x;
		NNEfloat y2 = q.y * q.y;
		NNEfloat z2 = q.z * q.z;
		NNEfloat w2 = q.w * q.w;
		NNEfloat xy = q.x * q.y;
		NNEfloat yz = q.y * q.z;
		NNEfloat zx = q.z * q.x;
		NNEfloat wx = q.w * q.x;
		NNEfloat wy = q.w * q.y;
		NNEfloat wz = q.w * q.z;

		m_val[0][0] = 1- 2 * y2 - 2 * z2; m_val[1][0] = 2 * xy - 2 * wz;     m_val[2][0] = 2 * zx + 2 * wy;
		m_val[0][1] = 2 * xy + 2 * wz;    m_val[1][1] = 1 - 2 * x2 - 2 * z2; m_val[2][1] = 2 * yz - 2 * wx;
		m_val[0][2] = 2 * zx - 2 * wy;    m_val[1][2] = 2 * yz + 2 * wx;     m_val[2][2] = 1 - 2 * x2 - 2 * y2; 

		m_val[0][3] = 0.0f; m_val[1][3] = 0.0f; m_val[2][3] = 0.0f;
		m_val[3][0] = 0.0f; m_val[3][1] = 0.0f; m_val[3][2] = 0.0f; m_val[3][3] = 1.0f;
	}

	NNEvoid identity()
	{
		m_val[0][0] = 1.0f; m_val[0][1] = 0.0f; m_val[0][2] = 0.0f; m_val[0][3] = 0.0f;
		m_val[1][0] = 0.0f; m_val[1][1] = 1.0f; m_val[1][2] = 0.0f; m_val[1][3] = 0.0f;
		m_val[2][0] = 0.0f; m_val[2][1] = 0.0f; m_val[2][2] = 1.0f; m_val[2][3] = 0.0f;
		m_val[3][0] = 0.0f; m_val[3][1] = 0.0f; m_val[3][2] = 0.0f; m_val[3][3] = 1.0f;
	}

	static Mat4 fromScale(Vec3 s)
	{
		Mat4 res;
		res.identity();

		res[0][0] = s.x;
		res[1][1] = s.y;
		res[2][2] = s.z;
		res[3][3] = 1.0f;

		return res;
	}

	static Mat4 fromUniScale(NNEfloat s)
	{
		Mat4 res;
		res.identity();

		res[0][0] = s;
		res[1][1] = s;
		res[2][2] = s;
		res[3][3] = 1.0f;

		return res;
	}

	static Mat4 fromTranslate(Vec3 t)
	{
		Mat4 res;
		res.identity();

		res[3][0] = t.x;
		res[3][1] = t.y;
		res[3][2] = t.z;
		res[3][3] = 1.0f;

		return res;
	}

	// override operators
	Mat4 & operator = (const Mat4 &mat)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_val[i][j] = mat.m_val[i][j];
			}
		}

		return *this;
	}

	Mat4 operator * (const Mat4 &other) const
	{
		// caution: result has to be initialize as zero matrix
		Mat4 result;

		for (int r = 0; r < 4; ++r) 
		{
			for (int c = 0; c < 4; ++c)
			{
				for (int i = 0; i < 4; ++i) 
				{
					result.m_val[c][r] += m_val[i][r] * other.m_val[c][i];
				}
			}
		}

		return result;
	}

	Vec3 operator * (const Vec3 &v3) const 
	{
		return Vec3(m_val[0][0] * v3.x + m_val[1][0] * v3.y + m_val[2][0] * v3.z + m_val[3][0],
					m_val[0][1] * v3.x + m_val[1][1] * v3.y + m_val[2][1] * v3.z + m_val[3][1],
					m_val[0][2] * v3.x + m_val[1][2] * v3.y + m_val[2][2] * v3.z + m_val[3][2]);
	}

	Vec4 operator * (const Vec4 &v4) const 
	{
		return Vec4(m_val[0][0] * v4.x + m_val[1][0] * v4.y + m_val[2][0] * v4.z + m_val[3][0] * v4.w,
					m_val[0][1] * v4.x + m_val[1][1] * v4.y + m_val[2][1] * v4.z + m_val[3][1] * v4.w,
					m_val[0][2] * v4.x + m_val[1][2] * v4.y + m_val[2][2] * v4.z + m_val[3][2] * v4.w,
					m_val[0][3] * v4.x + m_val[1][3] * v4.y + m_val[2][3] * v4.z + m_val[3][3] * v4.w);
	}

	// this also allow [][] indexing, because the return value can be indexed again.
	const NNEfloat* operator[](NNEint c) const 
	{
        debugAssertMsg(c >= 0, "indexing out bound");
        debugAssertMsg(c < 4, "indexing out bound");
        return (const NNEfloat *)&m_val[c];
    } 

	// return value above will be used as right value, this will be used as left value
	NNEfloat* operator[](NNEint c)
	{
        debugAssertMsg(c >= 0, "indexing out bound");
        debugAssertMsg(c < 4, "indexing out bound");
        return (NNEfloat *)&m_val[c];
    } 

	// implicitly convert the Mat4 to float *,
	// or explicitly (float *)amatrix
	operator NNEfloat *()
	{
		return &m_val[0][0];
	}

	const NNEfloat *getDataPtr() const
	{
		return (NNEfloat *)m_val;
	}

	NNEvoid zero()
	{
		memset(m_val, 0, sizeof(NNEfloat) * 16);
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

	NNEvoid set(const Mat3 &m3, const Vec3 &v3)
	{
		m_val[0][0] = m3[0][0]; m_val[0][1] = m3[0][1]; m_val[0][2] = m3[0][2];
		m_val[1][0] = m3[1][0]; m_val[1][1] = m3[1][1]; m_val[1][2] = m3[1][2];
		m_val[2][0] = m3[2][0]; m_val[2][1] = m3[2][1]; m_val[2][2] = m3[2][2];

		m_val[3][0] = v3.x;
		m_val[3][1] = v3.y;
		m_val[3][2] = v3.z;
		m_val[3][3] = 1.0f;

		m_val[1][3] = m_val[1][3] = m_val[2][3] = 0;
	}

	NNEvoid set(const Mat4 &m4)
	{

	}

	Vec4 getColumn4() const
	{
		return Vec4(m_val[3][0], m_val[3][1], m_val[3][2], 1.0f);
	}

	NNEvoid setColumn4(Vec4 v)
	{
		m_val[3][0] = v.x;
		m_val[3][1] = v.y;
		m_val[3][2] = v.z;
		m_val[3][3] = v.w;
	}

	NNEvoid setPos(NNEfloat x, NNEfloat y, NNEfloat z)
	{
		m_val[3][0] = x;
		m_val[3][1] = y;
		m_val[3][2] = z;
		m_val[3][3] = 1.0f;
	}

	NNEvoid setPos(const Vec3 &pos)
	{
		setPos(pos.x, pos.y, pos.z);
	}

	Vec3 getPos() const
	{
		return Vec3(m_val[3][0], m_val[3][1], m_val[3][2]);
	}

	Vec3 getAxisX() const
	{
		return Vec3(m_val[0][0], m_val[0][1], m_val[0][2]);
	}

	NNEvoid setAxisX(const Vec3 &v3)
	{
		m_val[0][0] = v3.x;
		m_val[0][1] = v3.y;
		m_val[0][2] = v3.z;
	}

	Vec3 getAxisY() const
	{
		return Vec3(m_val[1][0], m_val[1][1], m_val[1][2]);
	}

	NNEvoid setAxisY(const Vec3 &v3)
	{
		m_val[1][0] = v3.x;
		m_val[1][1] = v3.y;
		m_val[1][2] = v3.z;
	}

	Vec3 getAxisZ() const
	{
		return Vec3(m_val[2][0], m_val[2][1], m_val[2][2]);
	}

	NNEvoid setAxisZ(const Vec3 &v3)
	{
		m_val[2][0] = v3.x;
		m_val[2][1] = v3.y;
		m_val[2][2] = v3.z;
	}

	Mat4 getRotateMat4() const
	{
		Mat4 result = *this;
		result[3][0] = 0.0f;
		result[3][1] = 0.0f;
		result[3][2] = 0.0f;
		result[3][3] = 1.0f;

		return result;
	}

	Mat3 getRotateMat3() const
	{
		return Mat3(m_val[0][0], m_val[0][1], m_val[0][2],
					m_val[1][0], m_val[1][1], m_val[1][2],
					m_val[2][0], m_val[2][1], m_val[2][2]);
	}

	NNEvoid setRotate(const Mat4 &m4)
	{
		/*setAxisX(m4.getAxisX());
		setAxisY(m4.getAxisY());
		setAxisZ(m4.getAxisZ());*/
		m_val[0][0] = m4[0][0]; m_val[0][1] = m4[0][1]; m_val[0][2] = m4[0][2];
		m_val[1][0] = m4[1][0]; m_val[1][1] = m4[1][1]; m_val[1][2] = m4[1][2];
		m_val[2][0] = m4[2][0]; m_val[2][1] = m4[2][1]; m_val[2][2] = m4[2][2];
	}

	NNEvoid setRotate(const Mat3 &m3)
	{
		//setAxisX(m3.getAxisX());
		//setAxisY(m3.getAxisY());
		//setAxisZ(m3.getAxisZ());
		m_val[0][0] = m3[0][0]; m_val[0][1] = m3[0][1]; m_val[0][2] = m3[0][2];
		m_val[1][0] = m3[1][0]; m_val[1][1] = m3[1][1]; m_val[1][2] = m3[1][2];
		m_val[2][0] = m3[2][0]; m_val[2][1] = m3[2][1]; m_val[2][2] = m3[2][2];
	}

	NNEvoid translate(NNEfloat x, NNEfloat y, NNEfloat z)
	{
		/*Mat4 transl;
		transl.identity();
		transl.setPos(x, y, z);
		*this = transl * (*this);*/
		Vec3 tmp = getPos() + Vec3(x, y, z);
		setPos(tmp);
	}

	NNEvoid scale(NNEfloat x, NNEfloat y, NNEfloat z)
	{
		m_val[0][0] *= x;
		m_val[1][1] *= y;
		m_val[2][2] *= z;
	}

	// rotate means spin around self's axes
	// revolve means orbit around other objects
	NNEvoid rotateX(NNEfloat degree)
	{
		Quaternion rot(getAxisX(), degree);
		setAxisY(rot.rotateVector(getAxisY()));
		setAxisZ(rot.rotateVector(getAxisZ()));
	}

	NNEvoid rotateY(NNEfloat degree)
	{
		Quaternion rot(getAxisY(), degree);
		setAxisX(rot.rotateVector(getAxisX()));
		setAxisZ(rot.rotateVector(getAxisZ()));
	}

	NNEvoid rotateZ(NNEfloat degree)
	{
		Quaternion rot(getAxisZ(), degree);
		setAxisX(rot.rotateVector(getAxisX()));
		setAxisY(rot.rotateVector(getAxisY()));
	}

	NNEvoid rotate(NNEfloat deg, const Vec3 &axis)
	{
		Quaternion rot(axis, deg);
		setAxisX(rot.rotateVector(getAxisX()));
		setAxisY(rot.rotateVector(getAxisY()));
		setAxisZ(rot.rotateVector(getAxisZ()));
	}

	NNEvoid revolve(NNEfloat deg, const Vec3 &axis)
	{
		Mat4 rot(Quaternion(axis, deg));
		*this = rot * (*this);
	}

	NNEvoid moveInAxisZ(NNEfloat z)
	{
		Vec3 newpos = getPos() + z * getAxisZ();
		setPos(newpos);
	}

	NNEvoid moveInAxisX(NNEfloat x)
	{
		Vec3 newpos = getPos() + x * getAxisX();
		setPos(newpos);
	}

	NNEvoid moveInAxisY(NNEfloat y)
	{
		Vec3 newpos = getPos() + y * getAxisY();
		setPos(newpos);
	}

	// zfar and znear are both positive
	static Mat4 orthoProjection(NNEfloat xpos, NNEfloat xneg, NNEfloat ypos, NNEfloat yneg, NNEfloat zfar, NNEfloat znear)
	{
		Mat4 m4;
		NNEfloat diffx_inv = 1.0f / (xpos - xneg);
		NNEfloat diffy_inv = 1.0f / (ypos - yneg);
		NNEfloat diffz_inv = 1.0f / (zfar - znear);
		m4[0][0] = 2.0f * diffx_inv;
		m4[1][1] = 2.0f * diffy_inv;
		m4[2][2] = -2.0f * diffz_inv;
		m4[3][3] = 1.0f;
		m4[3][0] = -(xpos + xneg) * diffx_inv;
		m4[3][1] = -(ypos + yneg) * diffy_inv;
		m4[3][2] = -(zfar + znear) * diffz_inv;

		return m4;
	}

	static Mat4 lookAt(Vec3 eye, Vec3 focus, Vec3 up)
	{
		Mat4 m4;

		Vec3 axisZ = Vec3::normalize(eye - focus);
		Vec3 axisX = Vec3::normalize(Vec3::cross(up, axisZ));
		Vec3 axisY = Vec3::normalize(Vec3::cross(axisZ, axisX));

		m4.setAxisX(Vec3(axisX.x, axisY.x, axisZ.x));
		// second column
		m4.setAxisY(Vec3(axisX.y, axisY.y, axisZ.y));
		// third column
		m4.setAxisZ(Vec3(axisX.z, axisY.z, axisZ.z));
		// translation
		m4[3][0] = -Vec3::dot(axisX, eye); 
		m4[3][1] = -Vec3::dot(axisY, eye); 
		m4[3][2] = -Vec3::dot(axisZ, eye);
		// homogeneous
		m4[3][3] = 1.0f;

		return m4;
	}

protected:

	NNEfloat m_val[4][4];
};

// override global operators
inline Vec4 operator * (const Vec4 &v4, const Mat4& mat)
{
	return Vec4(v4.x * mat[0][0] + v4.y * mat[0][1] + v4.z * mat[0][2] + v4.w * mat[0][3],
				v4.x * mat[1][0] + v4.y * mat[1][1] + v4.z * mat[1][2] + v4.w * mat[1][3],
				v4.x * mat[2][0] + v4.y * mat[2][1] + v4.z * mat[2][2] + v4.w * mat[2][3],
				v4.x * mat[3][0] + v4.y * mat[3][1] + v4.z * mat[3][2] + v4.w * mat[3][3]);
}

} // namesapce NNE

#endif // NNE_MATRIX4_H