/*
 * OgreBulletConversions.h
 *
 *  Created on: Oct 20, 2012
 *      Author: coky
 */

#ifndef OGREBULLETCONVERSIONS_H_
#define OGREBULLETCONVERSIONS_H_

#include <bullet/LinearMath/btTransform.h>
#include <OGRE/OgreMatrix4.h>

class OgreBulletConversions
{
public:

	static btTransform convertOgreMatrixToBulletTf(const Ogre::Matrix4& mat)
	{
		Ogre::Quaternion q;
		Ogre::Vector3 p, scale;
		mat.decomposition(p,scale,q);
		return btTransform(btQuaternion(q.x,q.y,q.z,q.w),btVector3(p.x,p.y,p.z));
	}

	static Ogre::Matrix4 convertBulletTfToOgreMatrix(const btTransform &transform)
	{
		Ogre::Matrix4 mat = Ogre::Matrix4(convertBulletQuatToOgreQuat(transform.getRotation()));
		mat.setTrans(convertBulletVec3ToOgreVec3(transform.getOrigin()));
		return mat;
	}

	static Ogre::Vector3 convertBulletVec3ToOgreVec3(const btVector3 &p)
	{
		return Ogre::Vector3(p.x(),p.y(),p.z());
	}

	static Ogre::Quaternion convertBulletQuatToOgreQuat(const btQuaternion &q)
	{
		return Ogre::Quaternion(q.w(),q.x(),q.y(),q.z());
	}

	static btVector3 convertOgreVecToBulletVec(const Ogre::Vector3 &p)
	{
		return btVector3(p.x,p.y,p.z);
	}

	static btQuaternion convertOgreQuatToBulletQuat(const Ogre::Quaternion &q)
	{
		return btQuaternion(q.x,q.y,q.z,q.w);
	}
};

#endif /* OGREBULLETCONVERSIONS_H_ */
