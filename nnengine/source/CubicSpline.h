#ifndef NNE_CUBICSPLINE_H
#define NNE_CUBICSPLINE_H

#include "PlatformSpecs.h"
#include "Matrix4.hpp"
#include "Vec3.hpp"
#include "Array.h"

namespace NNE{

/*
	[u^3  u^2  u  1] * Base coefficient(mat4)
*/
class CubicSpline
{

public:
	
	CubicSpline()
	{}

	~CubicSpline()
	{}

	NNEvoid init(const Mat4 &b)
	{
		m_base = b;
	}

	// s is the variable in the base matrix of Catmull-Rom spline
	// it effects the curvature of the spline
	NNEvoid initAsCatmullRom(NNEfloat s)
	{
		m_base[0][0] = -s;    m_base[1][0] = 2 - s; m_base[2][0] = s - 2;     m_base[3][0] = s;
		m_base[0][1] = 2 * s; m_base[1][1] = s - 3; m_base[2][1] = 3 - 2 * s; m_base[3][1] = -s;
		m_base[0][2] = -s;    m_base[1][2] = 0;     m_base[2][2] = s;         m_base[3][2] = 0.0f;
		m_base[0][3] = 0.0f;  m_base[1][3] = 1.0f;  m_base[2][3] = 0.0f;      m_base[3][3] = 0.0f;
	}

	// 0 <= u <= 1, the curve determined by four control value
	NNEfloat interpolate(NNEfloat u, NNEfloat v0, NNEfloat v1, NNEfloat v2, NNEfloat v3)
	{
		return Vec4::dot(Vec4(u * u * u, u * u, u, 1) * m_base, Vec4(v0, v1, v2, v3));
	}

	// get tangent value of interpolation
	NNEfloat tangentLerp(NNEfloat u, NNEfloat v0, NNEfloat v1, NNEfloat v2, NNEfloat v3)
	{
		return Vec4::dot(Vec4(3 * u * u, 2 * u, 1, 0) * m_base, Vec4(v0, v1, v2, v3));
	}

private:

	// base matrix
	Mat4 m_base;
};

class CubicSpline3D
{
public:

	CubicSpline3D()
	{}

	~CubicSpline3D()
	{}

	NNEvoid initAsCatmullRom(NNEfloat s)
	{
		m_cubicSpline.initAsCatmullRom(s);
	}

	// set four control values to represent the curve, for example
	// 2 positions and 2 tangents for the Hermite spline,
	// 4 control points for the Bezier spline
	NNEvoid setControlValue(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
	{
		m_ctrl0 = v0;
		m_ctrl1 = v1;
		m_ctrl2 = v2;
		m_ctrl3 = v3;
	}

	Vec3 interpolate(NNEfloat u)
	{
		return Vec3(m_cubicSpline.interpolate(u, m_ctrl0.x, m_ctrl1.x, m_ctrl2.x, m_ctrl3.x),
					m_cubicSpline.interpolate(u, m_ctrl0.y, m_ctrl1.y, m_ctrl2.y, m_ctrl3.y),
					m_cubicSpline.interpolate(u, m_ctrl0.z, m_ctrl1.z, m_ctrl2.z, m_ctrl3.z));
	}

	Vec3 tangnetLerp(NNEfloat u)
	{
		Vec3 t(m_cubicSpline.tangentLerp(u, m_ctrl0.x, m_ctrl1.x, m_ctrl2.x, m_ctrl3.x),
				m_cubicSpline.tangentLerp(u, m_ctrl0.y, m_ctrl1.y, m_ctrl2.y, m_ctrl3.y),
				m_cubicSpline.tangentLerp(u, m_ctrl0.z, m_ctrl1.z, m_ctrl2.z, m_ctrl3.z));

		return Vec3::normalize(t);
	}

	// create the curve as a set of points by recursively divide the curve into many small lines
	// each of which is smaller than a shreshold specified.
	NNEvoid createCurve(Array<Vec3> &curve, Array<Vec3> &tangent, 
		NNEfloat start, NNEfloat end, NNEfloat maxLength);

private:

	CubicSpline m_cubicSpline;

	Vec3 m_ctrl0;
	Vec3 m_ctrl1;
	Vec3 m_ctrl2;
	Vec3 m_ctrl3;
};

} // manespace NNE

#endif // NNE_CUBICSPLINE_H