#ifndef __DynamicSystemDriver_h__
#define __DynamicSystemDriver_h__
#include <random>
#include "Common.h"
#include "Driver.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"

class DynamicSystemDriver : public Driver, public OpenGLViewer
{using VectorD=Vector3;using VectorDi=Vector2i;using Base=Driver;
	////simulation data
	real dt=.02;
	VectorD position=VectorD::Unit(0);
	VectorD velocity=VectorD::Ones();

	////visualization data
	OpenGLSegmentMesh* opengl_trace=nullptr;							////vector field
	Array<OpenGLSphere*> opengl_spheres;								////spheres

public:
	virtual void Initialize(){OpenGLViewer::Initialize();}
	virtual void Run(){OpenGLViewer::Run();}

	////synchronize simulation data to visualization data, called in OpenGLViewer::Initialize()
	virtual void Initialize_Data()
	{
		////initialize a segment mesh to visualize the trace
		opengl_trace=Add_Interactive_Object<OpenGLSegmentMesh>();
		opengl_trace->mesh.Elements().resize(1);
		opengl_trace->mesh.Vertices().resize(1);
		opengl_trace->mesh.Vertices()[0]=position;
		opengl_trace->mesh.elements[0]=Vector2i(0,0);
		opengl_trace->Set_Data_Refreshed();
		opengl_trace->Initialize();

		////initialize a sphere to visualize the particle
		OpenGLSphere* opengl_sphere=Add_Interactive_Object<OpenGLSphere>();
		opengl_sphere->pos=Vector3::Zero();
		opengl_sphere->radius=(real).1;
		Set_Color(opengl_sphere,OpenGLColor(.0,1.,.0,1.));
		opengl_sphere->Set_Data_Refreshed();
		opengl_sphere->Initialize();
		opengl_spheres.push_back(opengl_sphere);

		////set OpenGL rendering environments
		auto dir_light=OpenGLUbos::Add_Directional_Light(glm::vec3(-1.f,-.1f,-.2f));
		OpenGLUbos::Set_Ambient(glm::vec4(.1f,.1f,.1f,1.f));
		OpenGLUbos::Update_Lights_Ubo();
	}

	///HW0 TODO: your implementation: update position and velocity of the particle for a timestep
	void Advance(const real dt)
	{
		//////////////////////////////////////////////////////////////////////////
		////sample code for a Lorenz system
		////reference: https://en.wikipedia.org/wiki/Lorenz_system
		real sigma=(real)10;
		real rho=28;
		real beta=(real)8/(real)3;

		velocity[0]=sigma*(position[1]-position[0]);
		velocity[1]=position[0]*(rho-position[2])-position[1];
		velocity[2]=position[0]*position[1]-beta*position[2];
		
		position+=velocity*dt;
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		////Implement your customized dynamic system; Comment out the previous part
		/* Your implementation*/

		/* Your implementation*/
	}

	void Sync_Simulation_And_Visualization_Data()
	{
		opengl_trace->mesh.Vertices().push_back(position);
		int n=opengl_trace->mesh.Elements().size();
		opengl_trace->mesh.Elements().push_back(Vector2i(n-1,n-2));
		opengl_trace->Set_Data_Refreshed();

		////update and sync data for spheres
		for(int i=0;i<opengl_spheres.size();i++){
			opengl_spheres[i]->pos=position;
			opengl_spheres[i]->Set_Data_Refreshed();}
	}

	////update simulation and visualization for each time step
	virtual void Toggle_Next_Frame()
	{
		Advance(dt);
		Sync_Simulation_And_Visualization_Data();
		OpenGLViewer::Toggle_Next_Frame();
	}
};
#endif