#include "GJK_EPA.h"
#include "ConvexShape.h"
#include "Vec4.hpp"
#include "DebugUtilities.h"
#include "Math.h"
#include "DebugRenderer.h"

#include <limits>

namespace NNE{

NNEuint GJK::originTriangleTest(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2,
								Vec3 &sd)
{
	Vec3 v20 = v0 - v2;
	Vec3 v21 = v1 - v2;
	Vec3 v01 = v1 - v0;

	// triangle normal without normalization
	// triangle winding order is CCW when look in the direction 
	// opposite to tn
	Vec3 tn = Vec3::cross(v20, v21);

	// vector perpendicular to v20, pointing outside the triangle,
	// on the triangle plane
	Vec3 n20 = Vec3::cross(v20, tn);
	// vector perpendicular to v21, pointing outside the triangle,
	// on the triangle plane
	Vec3 n21 = Vec3::cross(tn, v21);

	Vec3 n01 = Vec3::cross(v01, tn);

	// If d < 0, the origin lies on the side
	// that the normal points to, otherwise the origin lies on
	// the opposite side.
	NNEfloat d = Vec3::dot(tn, v2);

	// q20 < 0, origin is outside the triangle prism and on the edge v20 side 
	NNEfloat q20 = Vec3::dot(n20, v2);
	NNEfloat q21 = Vec3::dot(n21, v2);
	NNEfloat q01 = Vec3::dot(n01, v0);

	if (d > 0)
	{
		sd = -tn;
		return OPPOSITE_NORMAL_SIDE;
	}
	else
	{
		if (q20 < 0)
		{
			if (0 == d)
			{
				// recalculate search direction
				// we aleady know the origin lies on the triangle plane
				sd = n20;
			}
			else
			{
				sd = Vec3::vectorTriple(v20, -v2, v20);
			}
			return OUTSIDE_PRISM_L;
		}
		else if (q21 < 0)
		{
			if (0 == d)
			{
				sd = n21;
			}
			else 
			{
				sd = Vec3::vectorTriple(v21, -v2, v21);
			}
			return OUTSIDE_PRISM_R;
		}
		else if (q01 < 0)
		{
	
			if (d < 0)
			{
				sd = Vec3::vectorTriple(v01, -v0, v01); 
				return OUTSIDE_PRISM_M;
			}
			else // will not happen
			{
			//	// happens when there is obtuse angle between faces
			//	// not exactly INSIDE_TRIANGLE, probably inside tetrahedron
				return OPPOSITE_NORMAL_SIDE;
			}
		}
		else
		{
			if (0 == d)
			{
				// the origin lies inside the triangle
				return INSIDE_TRIANGLE;
			}
			else/* if(d < 0)*/
			{
				sd = tn;
				return NORMAL_SIDE;
			}
			//else
			//{
			//	sd = -tn;
			//	return OPPOSITE_NORMAL_SIDE;
			//}
		}
	}

	
}

NNEbool GJK::originSimplexTest()
{
	// 2-dimension simplex, a line
	// we can deduct that the origin is either on the line or in the line
	// feature Voronoi region(explained in Casey's video, Casey said it could also be
	// in the newly added point feature Voronoi region).
	if (2 == m_pointNum)
	{
		Vec3 edge01 = m_p[1].minkowski - m_p[0].minkowski;
		Vec3 n = Vec3::cross(edge01, -m_p[0].minkowski);

#if defined(NNE_DEBUG)
		// theoratically impossible condition
		if (Vec3::dot(m_p[1].minkowski, edge01) < 0)
		{
			debugAssert(false);
		}
#endif

		// if the origin is on the line segment
		if (n.isZero())
		{
			buildHull();
			return true;
		}
		else
		{
			// search direction should be perpendicular to vec1_0 and 
			// pointing to origin in the plane formed by simplex[0], simplex[1] and origin
			m_sd = Vec3::cross(n, edge01);
			return false;
		}
	}
	// m_p[2] is the newly added point
	else if (3 == m_pointNum)
	{
		SP3 *v0 = m_p + 0;
		SP3 *v1 = m_p + 1;
		SP3 *v2 = m_p + 2;
		Vec3 v20 = v0->minkowski - v2->minkowski;
		Vec3 v21 = v1->minkowski - v2->minkowski;

		// triangle normal without normalization
		// triangle winding order is CCW when look in the direction 
		// opposite to tn
		Vec3 tn = Vec3::cross(v20, v21);

		// vector perpendicular to v20, pointing outside the triangle,
		// on the triangle plane
		Vec3 n20 = Vec3::cross(v20, tn);
		// vector perpendicular to v21, pointing outside the triangle,
		// on the triangle plane
		Vec3 n21 = Vec3::cross(tn, v21);

		// Ax + By + Cz + D = 0
		// d != D because here the normal is not unit length
		// However we still can determin which side of the plane the origin is on.
		// If d < 0, the origin lies on the side
		// that the normal points to, otherwise the origin lies on
		// the opposite side.
		NNEfloat d = Vec3::dot(tn, v2->minkowski);

		// q20 < 0, origin is outside the triangle prism and on the edge v20 side 
		NNEfloat q20 = Vec3::dot(n20, v2->minkowski);
		NNEfloat q21 = Vec3::dot(n21, v2->minkowski);

		if (q20 < 0)
		{
			if (0 == d)
			{
				// recalculate search direction
				// we aleady know the origin lies on the triangle plane
				m_sd = n20;
			}
			else
			{
				// cost fewer operations than (v20 x -m_p[2].minkowski) x v20
				m_sd = Vec3::vectorTriple(v20, -v2->minkowski, v20);
			}

			// reduce the simplex to line v20
			m_p[1] = m_p[2];
			m_pointNum--;

			return false;
		}
		else if (q21 < 0)
		{
			if (0 == d)
			{
				m_sd = n21;
			}
			else 
			{
				m_sd = Vec3::vectorTriple(v21, -v2->minkowski, v21);
			}

			// reduce the simplex to line v21
			m_p[0] = m_p[2];
			m_pointNum--;

			return false;
		}
		else
		{
#if defined(NNE_DEBUG)
			Vec3 v01 = v1->minkowski - v0->minkowski;
			Vec3 n01 = Vec3::cross(v01, tn);
			NNEfloat q01 = Vec3::dot(n01, v0->minkowski);
			if (q01 < 0)
			{
				// theoratically impossible condition
				debugAssert(false);
			}
#endif

			if (0 == d)
			{
				// the origin lies inside the triangle

				buildHull();

				return true;
			}
			else if(d < 0)
			{
				m_sd = tn;

				// adjust vertex order so that it's CCW when look at
				// the direction along the m_sd, normal points away from the origin
				SP3 tmp = m_p[2];
				m_p[2] = m_p[1];
				m_p[1] = tmp;

				return false;
			}
			else
			{
				m_sd = -tn;

				return false;
			}
		}
	}
	// m_p[0, 1, 2] forms the old triangle, m_p[3] is newly added
	// old triangle's CCW normal(e01 x e02) points in the opposite 
	// direction to the search vector
	else // 4 == m_num
	{
		Vec3 sd023, sd213, sd103;
		NNEuint vr023 = originTriangleTest(m_p[0].minkowski, m_p[2].minkowski, m_p[3].minkowski, sd023);
		NNEuint vr213 = originTriangleTest(m_p[2].minkowski, m_p[1].minkowski, m_p[3].minkowski, sd213);
		NNEuint vr103 = originTriangleTest(m_p[1].minkowski, m_p[0].minkowski, m_p[3].minkowski, sd103);

		if (INSIDE_TRIANGLE == vr023
			|| INSIDE_TRIANGLE == vr213
			|| INSIDE_TRIANGLE == vr103)
		{
			return true;
		}
		else if (OPPOSITE_NORMAL_SIDE == vr023
			&& OPPOSITE_NORMAL_SIDE == vr213
			&& OPPOSITE_NORMAL_SIDE == vr103)
		{
			return true;
		}
		else if (NORMAL_SIDE == vr023)
		{
			// reduce the simplex to triangle
			m_p[1] = m_p[3];
			m_pointNum--;
			m_sd = sd023;
			return false;
		}
		else if (NORMAL_SIDE == vr103)
		{
			m_p[2] = m_p[3];
			m_pointNum--;
			m_sd = sd103;
			return false;
		}
		else if (NORMAL_SIDE == vr213)
		{
			m_p[0] = m_p[3];
			m_pointNum--;
			m_sd = sd213;
			return false;
		}
		else if (OUTSIDE_PRISM_M == vr023)
		{
			m_p[1] = m_p[2];
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			return false;
		}
		else if (OUTSIDE_PRISM_M == vr103)
		{
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			return false;
		}
		else if (OUTSIDE_PRISM_M == vr213)
		{
			m_p[0] = m_p[1];
			m_p[1] = m_p[2];
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			m_sd = sd213;
			return false;
		}
		else if (OUTSIDE_PRISM_L == vr023 && OUTSIDE_PRISM_R == vr103)
		{
			m_p[1] = m_p[3];
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			return false;
		}
		else if (OUTSIDE_PRISM_R == vr023 && OUTSIDE_PRISM_L == vr213)
		{
			m_p[0] = m_p[2];
			m_p[1] = m_p[3];
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			return false;
		}
		else if (OUTSIDE_PRISM_L == vr103 && OUTSIDE_PRISM_R == vr213)
		{
			m_p[0] = m_p[3];
			m_pointNum -= 2;
			Vec3 v01 = m_p[1].minkowski - m_p[0].minkowski;
			m_sd = Vec3::vectorTriple(v01, -m_p[0].minkowski, v01);
			return false;
		}
		else 
		{
			// impossible condition
			debugAssert(false);
			return true;
		}
	}
}

GJK::SP3 GJK::suppportPoint(const Vec3 &sd)
{
	// should use left multiplication
	// world = R * local --> inv(R) * world = local
	Vec3 sd_local0 = m_invTransf0 * sd;
	Vec3 sd_local1 = m_invTransf1 * (-sd);

	Vec3 p0 = m_convex0->furthestPointInDir(sd_local0);
	Vec3 p1 = m_convex1->furthestPointInDir(sd_local1);

	return SP3(m_transform0 * p0 - m_transform1 * p1,
			 p0, p1);
}

NNEvoid GJK::buildHull()
{
	NNEfloat epsilon = 1e-4f;
	NNEfloat sqrEpsilon = 1e-8f;
	
	const Vec3 e_sd[6] = {Vec3(1.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f),
					Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f),
					Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f)};

	switch (m_pointNum)
	{
	// original feature is a line
	// the origin lying on the line makes EPA hard to converge
	case 2: 
	{
		Vec3 unit_e01 = Vec3::normalize((m_p[1].minkowski - m_p[0].minkowski));

		for (NNEuint i = 0; i < 6; ++i)
		{
			SP3 sp = suppportPoint(e_sd[i]);
			Vec3 d0 = sp.minkowski - m_p[0].minkowski;
			NNEfloat d1 = Vec3::dot(unit_e01, d0);
			Vec3 d2 = unit_e01 * d1 - d0;
			if (d2.sqaureLength() > sqrEpsilon)
			{
				m_p[2] = sp;
				m_pointNum++;
				break;
			}
		}
	}
	// no break, go through to case 3

	case 3:
	{
		Vec3 n = Vec3::normalize(Vec3::cross(m_p[1].minkowski - m_p[0].minkowski, 
			m_p[2].minkowski - m_p[0].minkowski));
		SP3 sp = suppportPoint(n);
		Vec3 d0 = sp.minkowski - m_p[0].minkowski;
		NNEfloat d1 = Vec3::dot(n, d0);
		if (d1 > epsilon)
		{
			// preserve the winding order
			m_p[3] = m_p[1];
			m_p[1] = m_p[2];
			m_p[2] = m_p[3];
			m_p[3] = sp;
			m_pointNum++;
		}
		else
		{
			sp = suppportPoint(-n);
			//d0 = sp - m_p[0];
			//d1 = Vec3::dot(n, d0);
			//if (d1 < epsilon && d1 > -epsilon)
			//{
			//	debugAssert(false);
			//}
			m_p[3] = sp;
			m_pointNum++;
		}
	}
	}
}

NNEbool GJK::isOriginInHull()
{
	Vec3 edge01 = m_p[1].minkowski - m_p[0].minkowski;
	Vec3 edge02 = m_p[2].minkowski - m_p[0].minkowski;
	Vec3 tn = Vec3::cross(edge01, edge02);
	NNEfloat d = Vec3::dot(tn, m_p[0].minkowski);
	if (d < 0)
	{
		return false;
	}
	else
	{
		Vec3 edge03 = m_p[3].minkowski - m_p[0].minkowski;
		tn = Vec3::cross(edge03, edge01);
		d = Vec3::dot(tn, m_p[0].minkowski);
		if (d < 0)
		{
			return false;
		}
		else
		{
			tn = Vec3::cross(edge02, edge03);
			d = Vec3::dot(tn, m_p[0].minkowski);
			if (d < 0)
			{
				return false;
			}
			else
			{
				Vec3 edge13 = m_p[3].minkowski - m_p[1].minkowski;
				Vec3 edge12 = m_p[2].minkowski - m_p[1].minkowski;
				tn = Vec3::cross(edge13, edge12);
				d = Vec3::dot(tn, m_p[1].minkowski);
				if (d < 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}
	}
}

NNEbool GJK::evaluate(ConvexShape *c0, const Mat4 &transform0, 
					ConvexShape *c1, const Mat4 &transform1)
{
	// setup
	m_convex0 = c0;
	m_convex1 = c1;
	m_transform0 = transform0;
	m_transform1 = transform1;
	m_invTransf0 = Mat3::inverse(m_transform0.getRotateMat3());
	m_invTransf1 = Mat3::inverse(m_transform1.getRotateMat3());

	// starting point
	// every support point is on Minkowski Sum's boundary
	SP3 s = suppportPoint(Vec3(0.7f, 0.7f, 0.7f));
	m_p[m_pointNum++] = s;

	// search direction start from starting point to origin
	m_sd = -s.minkowski;

	// if m_sd's length is too short, take it as intersected
	if (m_sd.sqaureLength() < 1e-8f)
	{
		return true;
	}

	// the maximum number of iteration GJK takes to converge to a result
	NNEint iterateNum = MAX_GJK_ITERATION;

	while (iterateNum-- > 0)
	{
		// new support point
		SP3 np = suppportPoint(m_sd);

		// if new furthest point is not further along the search direction than the origin
		// return false no intersection
		if (Vec3::dot(np.minkowski, m_sd) < 0) 
		{
			return false;
		}

		m_p[m_pointNum++] = np;

		if (originSimplexTest())
		{
			/*if (isOriginInHull())
			{
				return true;
			}
			else
			{
				return false;
			}*/
			return true;
		}
	}

	// we assume no intersection if GJK can't converge within limited iterations
	return false;
}



EPA::Triangle::Triangle(NNEuint aa, NNEuint bb, NNEuint cc, GJK::SP3 vert[])
	:a(aa), b(bb), c(cc), next(NULL), parent(NULL)
{
	// triangle normal
	n_unit = Vec3::normalize(
		Vec3::cross(vert[b].minkowski - vert[a].minkowski, vert[c].minkowski - vert[a].minkowski));

	dist2Origin = Vec3::dot(n_unit, vert[a].minkowski);

	// compute whether the project of the origin on the triangle plane lies within the triangle
	//Vec3 ac = vert[c].minkowski - vert[a].minkowski;
	//// normal of the vector ac, lying in the triangle plane, pointing outward triangle
	//Vec3 nac = Vec3::cross(n_unit, ac);
	//NNEfloat d = Vec3::dot(vert[a].minkowski, nac);
	//if (d < 0.0f)
	//{
	//	inside = OUTSIDE_TRIANGLE;
	//}
	//else
	//{
	//	Vec3 cb = vert[b].minkowski - vert[c].minkowski;
	//	Vec3 ncb = Vec3::cross(n_unit, cb);
	//	d = Vec3::dot(vert[c].minkowski, ncb);
	//	if (d < 0.0f)
	//	{
	//		inside = OUTSIDE_TRIANGLE;
	//	}
	//	else
	//	{
	//		Vec3 ba = vert[a].minkowski - vert[b].minkowski;
	//		Vec3 nba = Vec3::cross(n_unit, ba);
	//		d = Vec3::dot(vert[b].minkowski, nba);
	//		if (d < 0.0f)
	//		{
	//			inside = OUTSIDE_TRIANGLE;
	//		}
	//		else
	//		{
	//			inside = INSIDE_TRIANGLE;
	//		}
	//	}
	//}
}

EPA::EPA()
	:m_vertCount(0), m_edgeCount(0), 
	m_triRoot(NULL)
{
	m_triFreelistHead = &m_tris[0];

	for (NNEuint i = 0; i < MAX_TRIANGLE_NUM - 1; ++i)
	{
		m_tris[i].next = &m_tris[i + 1];
	}
	m_tris[MAX_TRIANGLE_NUM - 1].next = NULL;
}

NNEvoid EPA::reset()
{
	m_vertCount = 0;
	m_edgeCount = 0;

	m_triRoot = NULL;
	m_triFreelistHead = &m_tris[0];
	for (NNEuint i = 0; i < MAX_TRIANGLE_NUM - 1; ++i)
	{
		m_tris[i].next = &m_tris[i + 1];
		m_tris[i].parent = NULL;
	}
	m_tris[MAX_TRIANGLE_NUM - 1].next = NULL;
	m_tris[MAX_TRIANGLE_NUM - 1].parent = NULL;
}

NNEbool EPA::project(Triangle *tri, NNEfloat &ra, NNEfloat &rb, NNEfloat &rc)
{
	// Real-Time Collision Detection (p137)
	// area(abc) is proportional to dot(triNormal, cross(ab, ac))
	// using Lagrange identity to replace cross product with dot product

	// edges, p is the origin
	Vec3 ab = m_vertices[tri->b].minkowski - m_vertices[tri->a].minkowski;
	Vec3 ac = m_vertices[tri->c].minkowski - m_vertices[tri->a].minkowski;
	Vec3 ap = -m_vertices[tri->a].minkowski;
	// intermediate values
	NNEfloat d0 = Vec3::dot(ab, ab);
	NNEfloat d1 = Vec3::dot(ac, ac);
	NNEfloat d2 = Vec3::dot(ab, ac);
	NNEfloat d3 = Vec3::dot(ab, ap);
	NNEfloat d4 = Vec3::dot(ac, ap);

	NNEfloat inv_denom = 1.0f / (d0 * d1 - d2 * d2);
	rc = (d0 * d4 - d3 * d2) * inv_denom;
	rb = (d3 * d1 - d2 * d4) * inv_denom;
	ra = 1.0f - rc - rb;

	//Vec3 proj_p = m_vertices[tri->a].minkowski * ra + m_vertices[tri->b].minkowski * rb
	//				+ m_vertices[tri->c].minkowski * rc;

	//NNEfloat len = proj_p.length();
	//tri->dist2Origin = len;
	//NNEfloat inv_len = 1.0f / len;
	//tri->n_unit = proj_p * inv_len;

	return ra >= 0 && ra <= 1.0f
		&& rb >= 0 && rb <= 1.0f
		&& rc >= 0 && rc <= 1.0f;
}

NNEuint EPA::expandPolytope(Triangle *tri, const GJK::SP3 &nsp)
{
	NNEuint newVertIndex = m_vertCount;
	if (m_vertCount < MAX_VERTEX_NUM)
	{
		m_vertices[newVertIndex] = nsp;
		m_vertCount++;
	}
	else
	{
		return EXCEED_VERTEX_NUM;
	}

	addEdge(tri->a, tri->b);
	addEdge(tri->b, tri->c);
	addEdge(tri->c, tri->a);
	removeTriangle(tri);

	Triangle *tmp = m_triRoot;
	while (NULL != tmp)
	{
		Triangle *remove = tmp;
		tmp = tmp->next;

		// if new support point is on the side of the triangle that the normal points to
		if (Vec3::dot(remove->n_unit, nsp.minkowski - m_vertices[remove->a].minkowski) > 0)
		{
			addEdge(remove->a, remove->b);
			addEdge(remove->b, remove->c);
			addEdge(remove->c, remove->a);

			removeTriangle(remove);
		}
	}

	for (NNEuint i = 0; i < m_edgeCount; ++i)
	{
		if (-1 != m_edges[i].a)
		{
			addTriangle(newVertIndex, m_edges[i].a, m_edges[i].b);
		}
	}
	m_edgeCount = 0;

	return DO_ITERATION;
}

NNEuint EPA::addEdge(NNEuint a, NNEuint b)
{
	debugAssert(m_edgeCount < MAX_EDGE_NUM);

	NNEbool exsits = false;
	for (NNEuint i = 0; i < m_edgeCount; ++i)
	{
		if (m_edges[i].a == b && m_edges[i].b == a)
		{
			exsits = true;
			m_edges[i].a = -1;
			break;
		}
	}

	if (!exsits)
	{
		m_edges[m_edgeCount++] = Edge(a, b);
	}

	return DO_ITERATION;
}

NNEuint EPA::addTriangle(NNEuint aa, NNEuint bb, NNEuint cc)
{
	debugAssertMsg(NULL != m_triFreelistHead, "Exceed maximum triangle number");

	Triangle *newtri = m_triFreelistHead;
	m_triFreelistHead = newtri->next;

	*newtri = Triangle(aa, bb, cc, m_vertices);
	newtri->next = m_triRoot;
	if (NULL != m_triRoot)
	{
		m_triRoot->parent = newtri;
	}
	m_triRoot = newtri;

	return DO_ITERATION;
}

NNEvoid EPA::removeTriangle(Triangle *tri)
{
	Triangle *nextPtr = tri->next;
	Triangle *parentPtr = tri->parent;
	if (NULL != parentPtr)
	{
		parentPtr->next = nextPtr;

		if (NULL != nextPtr)
		{
			nextPtr->parent = parentPtr;
		}
	}
	else
	{
		m_triRoot = nextPtr;
		if (NULL != nextPtr)
		{
			nextPtr->parent = NULL;
		}
	}
	tri->next = m_triFreelistHead;
	m_triFreelistHead = tri;
}

NNEuint EPA::evaluate(GJK &gjk, ContactPoint &cp)
{
	NNEuint terminate = DO_ITERATION;

	// initialize the original convex hull
	m_vertices[0] = gjk.m_p[0];
	m_vertices[1] = gjk.m_p[1];
	m_vertices[2] = gjk.m_p[2];
	m_vertices[3] = gjk.m_p[3];
	m_vertCount += 4;

	terminate = addTriangle(0, 1, 2);
	terminate = addTriangle(0, 2, 3);
	terminate = addTriangle(0, 3, 1);
	terminate = addTriangle(2, 1, 3);

	while (DO_ITERATION == terminate)
	{
		// test origin to each triangle face, get the face that's
		// closest to the origin meanwhile the origin's projection 
		// on the triangle plane lies within the triangle

		//NNEfloat barya = 0.0f, baryb = 0.0f, baryc = 0.0f;
		// the pointer of the closest triangle 
		Triangle *closestTri = NULL;

		// shortest length from the origin to triangle
		NNEfloat slen = std::numeric_limits<NNEfloat>::infinity();

		Triangle *tmpTri = m_triRoot;
		while (NULL != tmpTri)
		{
			if (/*INSIDE_TRIANGLE == tmpTri->inside
				&&*/ tmpTri->dist2Origin < slen && tmpTri->dist2Origin > 0)
			{
				slen = tmpTri->dist2Origin;
				closestTri = tmpTri;
			}

			tmpTri = tmpTri->next;
		}

		Vec3 sd_unit = closestTri->n_unit;
		// find another furthest point along the search direction
		GJK::SP3 fp = gjk.suppportPoint(sd_unit);

		// fp = fp - origin, if the length of projection of fp on sd 
		// is not longer by some epsilon than the length of sd then
		// return sd as penetration vector
		NNEfloat len_fp = Vec3::dot(sd_unit, fp.minkowski);
		NNEfloat diff = len_fp - slen;
		if (Math::abs(diff) < 1e-5f)
		{
			terminate = REACH_CONVERGENCE;

			NNEfloat ra, rb, rc;
			NNEbool inside = project(closestTri, ra, rb, rc); 
			//NNEbool inhull = gjk.isOriginInHull();

			cp.cp0 = m_vertices[closestTri->a].sp0 * ra + m_vertices[closestTri->b].sp0 * rb
				+ m_vertices[closestTri->c].sp0 * rc;
			cp.cp1 = m_vertices[closestTri->a].sp1 * ra + m_vertices[closestTri->b].sp1 * rb
				+ m_vertices[closestTri->c].sp1 * rc;

			cp.cpws0 = gjk.m_transform0 * cp.cp0;
			cp.cpws1 = gjk.m_transform1 * cp.cp1;

			cp.normal = sd_unit;
			cp.depth = slen;

			// make sure the contact normal always points from object0 to obejct1
			Vec3 v01 = gjk.m_transform1.getPos()
				- gjk.m_transform0.getPos();
			if (Vec3::dot(v01, cp.normal) < 0)
			{
				cp.normal = -cp.normal;
			}

			break;
		}

		// add new point to the polytope
		// preserve the winding so that the normal of triangle
		// point outward the polytope
		terminate = expandPolytope(closestTri, fp);
	}

	return terminate;
}

NNEbool GjkEpaCollisionDetector::testCollision(ConvexShape *shape0, const Mat4 &transform0, 
	ConvexShape *shape1, const Mat4 &transform1, ContactPoint *cp)
{
	//static NNEbool flag = true;
	//if (!flag)
	//{
	//	return false;
	//}
	//flag = false;

	NNEbool collided = m_gjk.evaluate(shape0, transform0, shape1, transform1);

	if (collided)
	{
		NNEuint test = m_epa.evaluate(m_gjk, *cp);
		
		//debugOutput("intersected \n" << test << '\n' << cm.min_depth);
	}
	//else
	//{
	//	debugOutput("no intersection");
	//}

	m_gjk.reset();
	m_epa.reset();
	
	return collided;
}

} // namespace NNE