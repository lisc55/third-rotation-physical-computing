#ifndef __MultiCopterDriver_h__
#define __MultiCopterDriver_h__
#include <random>
#include "Common.h"
#include "Driver.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "MultiCopter.h"

template<int d> class MultiCopterDriver : public Driver, public OpenGLViewer
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using Base=Driver;
	real dt=.02;
	MultiCopter<d> copter;
	OpenGLSegmentMesh* opengl_copter=nullptr;

public:
	virtual void Initialize()
	{
		copter.Add_Rotor(VectorD::Unit(0),VectorD::Unit(1));
		copter.Add_Rotor(-VectorD::Unit(0),VectorD::Unit(1));
		copter.Add_Rotor(VectorD::Unit(2),VectorD::Unit(1));
		copter.Add_Rotor(-VectorD::Unit(2),VectorD::Unit(1));

		////viewer initialization, initialize visualization data
		OpenGLViewer::Initialize();
	}

	////synchronize simulation data to visualization data, called in OpenGLViewer::Initialize()
	virtual void Initialize_Data()
	{
		opengl_copter=Add_Interactive_Object<OpenGLSegmentMesh>();
		opengl_copter->mesh.elements.resize(2);
		opengl_copter->mesh.elements[0]=Vector2i(0,1);
		opengl_copter->mesh.elements[1]=Vector2i(2,3);
		opengl_copter->line_width=5.f;
		*opengl_copter->mesh.vertices=copter.rotor_pos;

		opengl_copter->Set_Data_Refreshed();
		opengl_copter->Initialize();
	}

	void Sync_Simulation_And_Visualization_Data()
	{
		for(int i=0;i<copter.rotor_pos.size();i++){
			(*opengl_copter->mesh.vertices)[i]=copter.World_Coord(copter.rotor_pos[i]);}
		opengl_copter->Set_Data_Refreshed();
	}

	////update simulation and visualization for each time step
	virtual void Toggle_Next_Frame()
	{
		copter.Advance(dt);
		Sync_Simulation_And_Visualization_Data();
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};
#endif