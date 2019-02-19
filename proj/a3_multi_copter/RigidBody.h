#ifndef __RigidBody_h__
#define __RigidBody_h__
#include "Common.h"

//////////////////////////////////////////////////////////////////////////
////Rigid body simulator
template<int d> class RigidBody
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using MatrixD=Matrix<real,d>;
public:
	VectorD position=VectorD::Zero();
	MatrixD orientation=MatrixD::Identity();

	virtual void Initialize()
	{
	}

	virtual void Advance(const real dt)
	{
	}
};

#endif