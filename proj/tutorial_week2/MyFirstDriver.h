#ifndef __MyFirstDriver_h__
#define __MyFirstDriver_h__
#include "Common.h"
#include "Driver.h"
#include "Particles.h"
#include "Mesh.h"

template<int d>
class MyFirstDriver : public Driver
{
	using VectorD=Vector<real,d>;
	using Base=Driver;
public:
	Particles<d> particles;
	real r=(real)1;
	VectorD g=VectorD::Unit(1)*(real)-10;
	SegmentMesh<d> segment_mesh;

	virtual void Initialize()
	{
		std::cout<<"initialize my first driver"<<std::endl;
		particles.Resize(1);
		particles.X(0)=VectorD::Unit(1)*(real)2;
		particles.V(0)=VectorD::Unit(1)*(real)-1;
	}

	virtual void Advance_One_Time_Step(const real dt,const real time)
	{
		std::cout<<"my first driver, step "<<dt<<std::endl;
		//particles.X(0)+=VectorD::Unit(0)*dt;
		particles.V(0)+=g*dt;
		particles.X(0)+=particles.V(0)*dt;

		if(particles.X(0)[1]<r){
			particles.X(0)[1]=r;
			particles.V(0)[1]=abs(particles.V(0)[1]);
		}
	}

	virtual void Write_Output_Files(const int frame)
	{
		std::cout<<"Write output files for frame "<<frame<<std::endl;
		Base::Write_Output_Files(frame);
		std::string file_name=frame_dir+"/particles";
		particles.Write_To_File_3d(file_name);

		Update_Segment_Mesh();
		std::cout<<"segment mesh: "<<segment_mesh.Vertices().size()
			<<", "<<segment_mesh.Elements().size()<<std::endl;
		file_name=frame_dir+"/segment_mesh";
		segment_mesh.Write_To_File_3d(file_name);
	}

protected:
	void Update_Segment_Mesh()
	{
		segment_mesh.Vertices().clear();
		segment_mesh.Elements().clear();

		int n=32;
		real pi=3.1415927;
		for(int i=0;i<n;i++){
			real theta=2.*pi*(real)i/(real)n;
			VectorD pos=VectorD::Zero();
			pos[0]=particles.X(0)[0]+r*cos(theta);
			pos[1]=particles.X(0)[1]+r*sin(theta);
			segment_mesh.Vertices().push_back(pos);}
		for(int i=0;i<n-1;i++)
			segment_mesh.Elements().push_back(Vector2i(i,i+1));
		segment_mesh.Elements().push_back(Vector2i(n-1,0));

		segment_mesh.Vertices().push_back(-VectorD::Unit(0));
		segment_mesh.Vertices().push_back(VectorD::Unit(0));
		int m=(int)segment_mesh.Vertices().size();
		segment_mesh.Elements().push_back(Vector2i(m-1,m-2));
	}
};

#endif
