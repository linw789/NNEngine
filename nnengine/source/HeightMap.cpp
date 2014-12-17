#include "HeightMap.h"
#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "../dependencies/stb_image.h"

namespace NNE{

NNEvoid HeightMap::toMesh(const NNEchar *filename, Mesh *amesh, NNEfloat interval/* = 1.0f*/)
{
	NNEint w = 0, h = 0, numCompo = 0;
	stbi_uc *imgData = stbi_load(filename, &w, &h, &numCompo, 0);

	amesh->m_va.m_position.reserve(w * h);
	//amesh->m_va.m_color.reserve(w * h);
	// start from upper left corner
	for (NNEint i = 0; i < h; ++i) // per row
	{
		for (NNEint j = 0; j < w; ++j) // per column
		{
			amesh->m_va.m_position.push_back(Vec3((j - w / 2.0f) * interval,
											 (NNEfloat)imgData[i * w + j],
											 -(h - 1 - i - h / 2.0f) * interval));
		}
	}

	/*
	    +-------+-------+-------+
        |     .'|     .'|     .'|
        |   .'  |   .'  |   .'  |
        | .'    | .'    | .'    |
        +'------+'------+'------+
        |     .'|     .'|     .'|
        |   .'  |   .'  |   .'  |
        | .'    | .'    | .'    |
        +'------+'------+'------+
        |     .'|     .'|     .'|
        |   .'  |   .'  |   .'  |
        | .'    | .'    | .'    |
        +'------+'------+'------+
	*/
	// compute normal
	amesh->m_va.m_normal.reserve(h * w);
	// http://www.gamedev.net/topic/163625-fast-way-to-calculate-heightmap-normals/
	for (NNEint i = 0; i < h; ++i)
	{
		for (NNEint j = 0; j < w; ++j)
		{
			NNEfloat nx = (NNEfloat)imgData[i * w + (j < w - 1? j + 1 : j) ]
							- imgData[i * 2 + (j > 0? j - 1 : j)];
			NNEfloat nz = (NNEfloat)imgData[(i < h - 1? i + 1 : i) * w + j]
							- imgData[(i > 0? i - 1 : i) * w + j];

			Vec3 normal(nx * 2, 2, nz * 2);
			amesh->m_va.m_normal.push_back(Vec3::normalize(normal));
		}
	}


	//amesh->m_va.m_normal.assign(w * h, Vec3(0.0f, 0.0f, 0.0f));
	//for (NNEint i = 1; i < h - 1; ++i) // row 
	//{
	//	for (NNEint j = 1; j < w - 1; ++j) // column
	//	{
	//		// up neighbor
	//		Vec3 up = amesh->m_va.m_position[(i - 1) * w + j];
	//		// left neighbor
	//		Vec3 left = amesh->m_va.m_position[i * w + j - 1];
	//		// bottom left neighbor
	//		//Vec3 bottomleft = amesh->m_va.m_position[(i + 1) * w + j - 1];
	//		// bottom neighbor
	//		Vec3 bottom = amesh->m_va.m_position[(i + 1) * w + j];
	//		// right neighbor
	//		Vec3 right = amesh->m_va.m_position[i * w + j + 1];
	//		// up right neighbor
	//		//Vec3 upright = amesh->m_va.m_position[(i - 1) * w + j + 1];

	//		Vec3 me = amesh->m_va.m_position[i * w + j];

	//		//Vec3 normal0 = Vec3::normalize(Vec3::cross(up - me, left - me));
	//		//Vec3 normal1 = Vec3::normalize(Vec3::cross(left - me, bottomleft - me));
	//		//Vec3 normal2 = Vec3::normalize(Vec3::cross(bottomleft - me, bottom - me));
	//		//Vec3 normal3 = Vec3::normalize(Vec3::cross(bottom - me, right - me));
	//		//Vec3 normal4 = Vec3::normalize(Vec3::cross(right - me, upright - me));
	//		//Vec3 normal5 = Vec3::normalize(Vec3::cross(upright - me, up - me));

	//		//Vec3 normal = (normal0 + normal1 + normal2 + normal3 + normal4 + normal5) / 6;

	//		Vec3 normal0 = Vec3::normalize(Vec3::cross(up - me, left - me));
	//		Vec3 normal1 = Vec3::normalize(Vec3::cross(left - me, bottom - me));
	//		Vec3 normal2 = Vec3::normalize(Vec3::cross(bottom - me, right- me));
	//		Vec3 normal3 = Vec3::normalize(Vec3::cross(right - me, up - me));

	//		Vec3 normal = (normal0 + normal1 + normal2 + normal3) / 4;

	//		amesh->m_va.m_normal[i * w + j] = Vec3::normalize(-normal);
	//	}
	//}
	//// upper left corner normal
	//{
	//	Vec3 bottom = amesh->m_va.m_position[w];
	//	Vec3 right = amesh->m_va.m_position[1];
	//	Vec3 me = amesh->m_va.m_position[0];
	//	Vec3 normal = Vec3::normalize(Vec3::cross(bottom - me, right - me));
	//	amesh->m_va.m_normal[0] = normal;
	//}
	//// bottom left corner normal
	//{
	//	Vec3 right = amesh->m_va.m_position[(h - 1) * w + 1];
	//	Vec3 upright = amesh->m_va.m_position[(h - 2) * w + 1];
	//	Vec3 up = amesh->m_va.m_position[(h - 2) * w];
	//	Vec3 me = amesh->m_va.m_position[(h - 1) * w];
	//	
	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(right - me, upright - me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(upright - me, up - me));
	//	Vec3 normal = (normal0 + normal1) / 2;

	//	amesh->m_va.m_normal[(h - 1) * w] = Vec3::normalize(normal1);
	//}
	//// bottom right corner normal
	//{
	//	Vec3 up = amesh->m_va.m_position[(h - 2) * w + w - 1];
	//	Vec3 left = amesh->m_va.m_position[(h - 1) * w + w - 2];
	//	Vec3 me = amesh->m_va.m_position[(h - 1) * w + w - 1];
	//	Vec3 normal = Vec3::normalize(Vec3::cross(up - me, left - me));
	//	amesh->m_va.m_normal[(h - 1) * w + w - 1] = normal;
	//}
	//// up right corner normal
	//{
	//	Vec3 left = amesh->m_va.m_position[w - 2];
	//	Vec3 bottomleft = amesh->m_va.m_position[w + w - 2];
	//	Vec3 bottom = amesh->m_va.m_position[w + w - 1];
	//	Vec3 me = amesh->m_va.m_position[w - 1];
	//	
	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(left - me, bottomleft- me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(bottomleft - me, bottom - me));
	//	Vec3 normal = (normal0 + normal1) / 2;

	//	amesh->m_va.m_normal[w - 1] = Vec3::normalize(normal1);
	//}
	//// up edge
	//for (NNEint i = 1; i < w - 1; ++i)
	//{
	//	Vec3 left = amesh->m_va.m_position[i - 1];
	//	Vec3 bottomleft = amesh->m_va.m_position[w + i - 1];
	//	Vec3 bottom = amesh->m_va.m_position[w + i];
	//	Vec3 right = amesh->m_va.m_position[i + 1];
	//	Vec3 me = amesh->m_va.m_position[i];

	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(left - me, bottomleft - me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(bottomleft - me, bottom - me));
	//	Vec3 normal2 = Vec3::normalize(Vec3::cross(bottom - me, right - me));
	//	Vec3 normal = (normal0 + normal1 + normal2) / 3;

	//	amesh->m_va.m_normal[i] = Vec3::normalize(normal1);
	//}
	//// left edge
	//for (NNEint i = 1; i < h - 1; ++i)
	//{
	//	Vec3 bottom = amesh->m_va.m_position[(i + 1) * w];
	//	Vec3 right = amesh->m_va.m_position[i * w + 1];
	//	Vec3 upright = amesh->m_va.m_position[(i - 1) * w + 1];
	//	Vec3 up = amesh->m_va.m_position[(i - 1) * w];
	//	Vec3 me = amesh->m_va.m_position[i * w];

	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(bottom - me, right - me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(right - me, upright - me));
	//	Vec3 normal2 = Vec3::normalize(Vec3::cross(upright - me, up - me));
	//	Vec3 normal = (normal0 + normal1 + normal2) / 3;

	//	amesh->m_va.m_normal[i * w] = Vec3::normalize(normal2);
	//}
	//// bottom edge
	//for (NNEint i = 1; i < w - 1; ++i)
	//{
	//	Vec3 right = amesh->m_va.m_position[(h - 1) * w + i + 1];
	//	Vec3 upright = amesh->m_va.m_position[(h - 2) * w + i + 1];
	//	Vec3 up = amesh->m_va.m_position[(h - 2) * w + i];
	//	Vec3 left = amesh->m_va.m_position[(h - 1) * w + i - 1];
	//	Vec3 me = amesh->m_va.m_position[(h - 1) * w + i];

	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(right - me, upright - me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(upright - me, up - me));
	//	Vec3 normal2 = Vec3::normalize(Vec3::cross(up - me, left - me));
	//	Vec3 normal = (normal0 + normal1 + normal2) / 3;

	//	amesh->m_va.m_normal[(h - 1) * w + i] = Vec3::normalize(normal2);
	//}
	//// right edge
	//for (NNEint i = 1; i < h - 1; ++i)
	//{
	//	Vec3 up = amesh->m_va.m_position[(i - 1) * w + w - 1];
	//	Vec3 left = amesh->m_va.m_position[i * w + w - 2];
	//	Vec3 bottomleft = amesh->m_va.m_position[(i + 1) * w + w - 2];
	//	Vec3 bottom = amesh->m_va.m_position[(i + 1) * w + w - 1];
	//	Vec3 me = amesh->m_va.m_position[i * w + w - 1];

	//	Vec3 normal0 = Vec3::normalize(Vec3::cross(up - me, left- me));
	//	Vec3 normal1 = Vec3::normalize(Vec3::cross(left - me, bottomleft - me));
	//	Vec3 normal2 = Vec3::normalize(Vec3::cross(bottomleft - me, bottom - me));
	//	Vec3 normal = (normal0 + normal1 + normal2) / 3;

	//	amesh->m_va.m_normal[i * w + w - 1] = Vec3::normalize(normal2);
	//}

	// compute triangle index
	amesh->m_va.m_triIndex.reserve((w - 1) * (h - 1) * 2 * 3);
	for (NNEint i = 0; i < h - 1; ++i)
	{
		for (NNEint j = 0; j < w - 1; ++j)
		{
			amesh->m_va.m_triIndex.push_back(i * w + j);
			amesh->m_va.m_triIndex.push_back((i + 1) * w + j);
			amesh->m_va.m_triIndex.push_back(i * w + j + 1);
			amesh->m_va.m_triIndex.push_back(i * w + j + 1);
			amesh->m_va.m_triIndex.push_back((i + 1) * w + j);
			amesh->m_va.m_triIndex.push_back((i + 1) * w + j + 1);
		}
	}

	// delete the image
	stbi_image_free(imgData);
}

} // namespace NNE