#ifndef NNE_GJK_EPA_H
#define NNE_GJK_EPA_H

#include "PlatformSpecs.h"
#include "Vec3.hpp"
#include "Array.h"
#include "Matrix4.hpp"
#include "Matrix3.hpp"
#include "CollisionAlgorithm.h"
#include "ContactPoint.h"

namespace NNE{

class ConvexShape;

struct GJK_EPA_Result
{
	// contact points in world space
	Vec3 cpws0, cpws1;

	// 2 contact points in each collider space
	Vec3 cp0, cp1;

	// contact normal in world space
	Vec3 normal;

	// minimum penetration depth
	NNEfloat min_depth;
};


/*
	Gilbert-Johnson-Keerthi Algorithm

	reference:http://www.codezealot.org/archives/88#gjk-intro
			  http://mollyrocket.com/849
*/
class GJK
{
friend class EPA;

public:

	// a wrapper of support points from Minkowski sum, convex0 and convex1 respectively
	struct SP3
	{
		SP3()
		{}

		SP3(const Vec3 &p, const Vec3 &p0, const Vec3 &p1)
			:minkowski(p), sp0(p0), sp1(p1)
		{}

		Vec3 minkowski, sp0, sp1;
	};

	// Voronoi region of triangle
	// v0, v1, v2(new point) forms the triangle counter-clock wise when look
	// at the direction opposite to normal. 
	enum
	{
		INSIDE_TRIANGLE = 0,
		NORMAL_SIDE = 1,
		OPPOSITE_NORMAL_SIDE = 2,
		// voronoi region of edge20
		OUTSIDE_PRISM_L = 3,
		// voronoi region of edge21
		OUTSIDE_PRISM_R = 4,
		// voronoi region of edge01
		OUTSIDE_PRISM_M = 5,

		MAX_GJK_ITERATION = 19,
	};
	
	GJK()
		:m_pointNum(0),
		m_convex0(NULL), m_convex1(NULL)
	{}

	~GJK()
	{}

	NNEbool evaluate(ConvexShape *c0, const Mat4 &transform0, 
					ConvexShape *c1, const Mat4 &transform1);


	NNEvoid reset()
	{
		m_pointNum = 0;
		m_convex0 = NULL;
		m_convex1 = NULL;
	}

private:

	// helper function used when originSimplexTest needs to do origin tetrahedron test,
	// v2 is the newly adde point to the tetrahedron.
	// 3 points passed in CCW order with triangle normal pointing outward the tetradedron
	NNEuint originTriangleTest(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2,
								Vec3 &sd);

	// test whether the origin is in the simplex
	// if not, might reduce the simplex to lower dimension and update the search direction
	NNEbool originSimplexTest();

	// get the furthest point of the Minkowski Sum in world space
	// get vertices from convex0 and convex1 respectively
	SP3 suppportPoint(const Vec3 &sd);

	// 
	NNEvoid buildHull();

	// debug use
	NNEbool isOriginInHull();

	// search direcion
	Vec3 m_sd;

	// points been added to the simplex
	// the last point(m_p[m_num - 1]) is always the newly added one
	SP3 m_p[4];

	// number of points already been added to the simplex,
	// the maximum number is 4, because the tetrahedron is the largest
	// simplex we need to form in our case
	NNEuint m_pointNum;

	ConvexShape *m_convex0;
	Mat4 m_transform0;
	Mat3 m_invTransf0;

	ConvexShape *m_convex1;
	Mat4 m_transform1;
	Mat3 m_invTransf1;
};

/*
	Expanding Polytope Algorithm
*/
class EPA
{
public:

	enum 
	{
		// iteration terminate flag
		DO_ITERATION = 0,
		REACH_CONVERGENCE,
		EXCEED_VERTEX_NUM,
		EXCEED_EDGE_NUM,
		EXCEED_TRIANGLE_NUM,
		OUTSIDE_HULL,
		INVALID,

		MAX_VERTEX_NUM = 15,

		// odd number of triangles is not possible
		MAX_TRIANGLE_NUM = 35,

		MAX_EDGE_NUM = 3 * MAX_TRIANGLE_NUM / 2,

		INSIDE_TRIANGLE = 0,
		OUTSIDE_TRIANGLE = 1,
	};

	struct Edge
	{
		Edge()
			:a(0), b(0)
		{}

		Edge(NNEuint aa, NNEuint bb)
			:a(aa), b(bb)
		{}

		// vertex indices
		NNEint a, b;
	};

	struct Triangle
	{
		Triangle()
			:next(NULL), parent(NULL)
		{}

		// contain vertex indices
		Triangle(NNEuint aa, NNEuint bb, NNEuint cc, GJK::SP3 vert[]);

		// vertex indices
		NNEuint a, b, c;

		// 0: the projection of the origin lies within the triangle
		NNEuint inside;

		// triangles stored as free list
		Triangle *next;
		Triangle *parent;

		// distance to the origin
		NNEfloat dist2Origin;

		// cache unit normal pointing outward the polytope
		Vec3 n_unit;
	};

	EPA();

	~EPA(){}

	NNEuint evaluate(GJK &gjk, ContactPoint &cm);

	NNEvoid reset();

private:

	// orthogonally project the origin onto the triangle plane.
	// return true if the point is projected inside the triangle.
	// return barycentric coordinate
	// return a direction from p to projected p
	NNEbool project(Triangle *tri, NNEfloat &ra, NNEfloat &rb, NNEfloat &rc);

	// given the new support point and triangle index based on which the support 
	// point is found, expand the original polytope by first eliminate all the 
	// faces whose outward facing normals point toward the new support point, 
	// add all edges of eliminated triangles to m_edges, be careful to preserve the
	// winding order that (edge.a - nsp) x (edge.b - nsp) points ourward.
	NNEuint expandPolytope(Triangle *tri, const GJK::SP3 &nsp);

	// detect duplicating edge by check if two edges have parallel but opposite directions
	NNEuint addEdge(NNEuint a, NNEuint b);

	NNEuint addTriangle(NNEuint aa, NNEuint bb, NNEuint cc);

	NNEvoid removeTriangle(Triangle *tri);

	GJK::SP3 m_vertices[MAX_VERTEX_NUM];
	NNEuint m_vertCount;

	// used for generating new triangle faces 
	// edge.a == -1 means this edge should be ignored;
	Edge m_edges[MAX_EDGE_NUM];
	// reset to zero after every expandPloytope call
	NNEuint m_edgeCount;

	Triangle m_tris[MAX_TRIANGLE_NUM];
	Triangle *m_triRoot;
	Triangle *m_triFreelistHead;
};

class GjkEpaCollisionDetector: public CollisionAlgorithm
{

public:

	GjkEpaCollisionDetector()
	{}

	~GjkEpaCollisionDetector()
	{}

	NNEbool testCollision(ConvexShape *shape0, const Mat4 &transform0,
					ConvexShape *shape1, const Mat4 &transform1, 
					ContactPoint *cp);

	NNEvoid reset()
	{
		m_gjk.reset();
		m_epa.reset();
	}

private:

	GJK m_gjk;

	EPA m_epa;
};

} // manespace NNE

#endif // NNE_GJK_EPA_H