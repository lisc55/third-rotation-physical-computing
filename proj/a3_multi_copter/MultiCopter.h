#ifndef __Drone_h__
#define __Drone_h__
#include "Common.h"
#include "RigidBody.h"

//////////////////////////////////////////////////////////////////////////
////Drone simulator
template<int d> class MultiCopter
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;
public:
	RigidBody<d> rigid_body;
	Array<VectorD> rotor_pos;
	Array<VectorD> rotor_dir;

	virtual void Initialize()
	{
	}

	virtual void Advance(const real dt)
	{
		rigid_body.position+=VectorD::Unit(1)*(real).1;
	}

	void Add_Rotor(const Vector3& pos,const Vector3& dir)
	{
		rotor_pos.push_back(pos);
		rotor_dir.push_back(dir);
	}

	Vector3 World_Coord(const Vector3& local_pos)
	{
		return rigid_body.position+local_pos;	////TODO: add rotation
	}
};

#endif