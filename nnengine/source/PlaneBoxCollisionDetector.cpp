#include "PlaneBoxCollisionDetector.h"
#include "InfinitePlaneShape.h"
#include "BoxShape.h"
#include "Math.h"

namespace NNE{

NNEbool PlaneBoxCollisionDetector::testCollision(ConvexShape *shape0, const Mat4 &transform0,
					ConvexShape *shape1, const Mat4 &transform1, ContactPoint *cp)
{
	InfinitePlaneShape *plane;
	//Mat4 planeTransform;
	BoxShape *box;
	Mat4 boxTransform;

	if (CollideShape::BOX == shape0->getShapeId())
	{
		box = static_cast<BoxShape *>(shape0);
		boxTransform = transform0;
		plane = static_cast<InfinitePlaneShape *>(shape1);
		//planeTransform = transform1;
	}
	else
	{
		box = static_cast<BoxShape *>(shape1);
		boxTransform = transform1;
		plane = static_cast<InfinitePlaneShape *>(shape0);
		//planeTransform = transform0;
	}

	Vec3 boxCenter = boxTransform.getPos();
	Vec3 planeNormal = plane->getNormal();
	NNEfloat planeDist = plane->getDistance();

	// d0 > 0, boxCenter is on the side that the normal points to
	NNEfloat d0 = Vec3::dot(planeNormal, boxCenter) - planeDist;
	Vec3 searchDir;
	Mat3 boxInvRotate = Mat3::transpose(boxTransform.getRotateMat3());

	// convert searchDir into box space
	if (d0 >= 0)
	{
		// if the boxCenter is on the normal side 
		// searchDir points opposite to the normal
		searchDir = boxInvRotate * -planeNormal;
	}
	else
	{
		searchDir = boxInvRotate * planeNormal;
	}

	Vec3 supportPoint_local = box->furthestPointInDir(searchDir);
	Vec3 supportPoint_world = boxTransform * supportPoint_local;
	NNEfloat d1 = Vec3::dot(planeNormal, supportPoint_world) - planeDist;

	if (d0 > 0 && d1 >= 0) // d1 == 0, means touching
	{
		return false;
	}
	else if (d0 < 0 && d1 <= 0)
	{
		return false; 
	}

	//cp->depth = Math::abs(d1);

	//// make sure the contact normal always points from object0 to obejct1
	//if (CollideShape::INFINITE_PLANE == shape0->getShapeId())
	//{
	//	cp->normal = d0 > 0 ? planeNormal : -planeNormal;
	//	cp->cpws1 = supportPoint_world;
	//	cp->cp1 = supportPoint_local;
	//	cp->cpws0 = cp->cpws1 - cp->depth * cp->normal;
	//	cp->cp0 = cp->cpws0;
	//}
	//else
	//{
	//	cp->normal = d0 > 0 ? -planeNormal : planeNormal;
	//	cp->cpws1 = supportPoint_world;
	//	cp->cp1 = supportPoint_local;
	//	cp->cpws0 = cp->cpws1 - cp->depth * cp->normal;
	//	cp->cp0 = cp->cpws0;
	//}

	//return true;

	Vec3 boxHalfExtents = box->getHalfExtents(); 
	Vec3 boxAxisX = boxTransform.getAxisX();
	Vec3 boxAxisY = boxTransform.getAxisY();
	Vec3 boxAxisZ = boxTransform.getAxisZ();

	// the points lie in the middle between supportpoint_world and its adjacent points
	Vec3 midPoint0_world;
	Vec3 edge0_local;
	if (supportPoint_local.x > 0)
	{
		midPoint0_world = supportPoint_world - boxHalfExtents.x * boxAxisX;
		edge0_local = Vec3(-boxHalfExtents.x, 0, 0);
	}
	else
	{
		midPoint0_world = supportPoint_world + boxHalfExtents.x * boxAxisX;
		edge0_local = Vec3(boxHalfExtents.x, 0, 0);
	}

	Vec3 midPoint1_world;
	Vec3 edge1_local;
	if (supportPoint_local.y > 0)
	{
		midPoint1_world = supportPoint_world - boxHalfExtents.y * boxAxisY;
		edge1_local = Vec3(0, -boxHalfExtents.y, 0);
	}
	else
	{
		midPoint1_world = supportPoint_world + boxHalfExtents.y * boxAxisY;
		edge1_local = Vec3(0, boxHalfExtents.y, 0);
	}

	Vec3 midPoint2_world;
	Vec3 edge2_local;
	if (supportPoint_local.z > 0)
	{
		midPoint2_world = supportPoint_world - boxHalfExtents.z * boxAxisZ;
		edge2_local = Vec3(0, 0, -boxHalfExtents.z);
	}
	else
	{
		midPoint2_world = supportPoint_world + boxHalfExtents.z * boxAxisZ;
		edge2_local = Vec3(0, 0, boxHalfExtents.z);
	}

	Vec3 edge0 = midPoint0_world - supportPoint_world;
	Vec3 edge1 = midPoint1_world - supportPoint_world;
	Vec3 edge2 = midPoint2_world - supportPoint_world;

	NNEfloat dot0 = Vec3::dot(planeNormal, edge0);
	NNEfloat dot1 = Vec3::dot(planeNormal, edge1);
	NNEfloat dot2 = Vec3::dot(planeNormal, edge2);

	if (0 == dot0)
	{
		// box face is parallel with the plane
		if (0 == dot1)
		{
			cp->cpws0 = cp->cpws1 = supportPoint_world + edge0 + edge1;
			cp->cp0 = cp->cp1 = supportPoint_local + edge0_local + edge1_local;
		}
		else if (0 == dot2)
		{
			cp->cpws0 = cp->cpws1 = supportPoint_world + edge0 + edge2;
			cp->cp0 = cp->cp1 = supportPoint_local + edge0_local + edge2_local;
		}
		// the edge0 is parallel with the plane
		else
		{
			cp->cpws0 = cp->cpws1 = midPoint0_world;
			cp->cp0 = cp->cp1 = supportPoint_local + edge0_local;
		}
	}
	else if (0 == dot1)
	{
		if (0 == dot2)
		{
			cp->cpws0 = cp->cpws1 = supportPoint_world + edge1 + edge2;
			cp->cp0 = cp->cp1 = supportPoint_local + edge1_local + edge2_local;
		}
		else
		{
			cp->cpws0 = cp->cpws1 = midPoint1_world;
			cp->cp0 = cp->cp1 = supportPoint_local + edge1_local;
		}
	}
	else if (0 == dot2)
	{
		cp->cpws0 = cp->cpws1 = midPoint2_world;
		cp->cp0 = cp->cp1 = supportPoint_local + edge2_local;
	}
	else
	{
		cp->cpws0 = cp->cpws1 = supportPoint_world;
		cp->cp0 = cp->cp1 = supportPoint_local;
	}

	// make sure the contact normal always points from object0 to obejct1
	if (CollideShape::INFINITE_PLANE == shape0->getShapeId())
	{
		cp->normal = d0 > 0 ? planeNormal : -planeNormal;
	}
	else
	{
		cp->normal = d0 > 0 ? -planeNormal : planeNormal;
	}
	cp->depth = Math::abs(d1);

	return true;
}

} // namespace NNE