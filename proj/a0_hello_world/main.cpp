//#####################################################################
// Main
// SimpleX, Bo Zhu
//#####################################################################
#include <iostream>
#include "Common.h"
#include "Particles.h"
#include "Mesh.h"
#include "File.h"

void Test_Eigen()
{
	std::cout<<"--- Test_Eigen ---"<<std::endl;
    Vector3 v(1,2,3);
    Vector2 v2(1,2);
    std::cout<<"v: "<<v.transpose()<<", "<<v2.transpose()<<std::endl;

    Vector3d vd(1.,2.,3.);
    Vector3i vi=vd.cast<int>();
    Vector3d vt=vi.cast<real>();
    std::cout<<"vi: "<<vi.transpose()<<", vt: "<<vt.transpose()<<std::endl;

    real array[5]={1,2,3,4,5};
    for(auto x:array)std::cout<<x<<", ";std::cout<<std::endl;
}

void Test_Particles_And_Mesh()
{
	std::cout<<"--- Test_Particles_And_Mesh ---"<<std::endl;
	const int d=3;
	using VectorD=Vector<real,d>;
	using VectorDi=Vector<int,d>;

    Particles<d> particles;
	particles.Resize(3);

    particles.Add_Element();
    particles.X(0)=Vector3::Ones();
    particles.V(0)=Vector3::Ones()*(real)2;
    std::cout<<"[Particle] #particles: "<<particles.Size()<<std::endl;
    std::cout<<"[Particle] X(0): "<<particles.X(0).transpose()<<", V(0): "<<particles.V(0).transpose()<<std::endl;

    Array<VectorD> vtx={VectorD::Zero(),VectorD::Unit(0),VectorD::Unit(1)};
	TriangleMesh<3> triangle_mesh;
	for(auto& v:vtx)triangle_mesh.Vertices().push_back(v);
    triangle_mesh.Elements().push_back(VectorDi(0,1,2));
    std::cout<<"[Triangle mesh] #vertices: "<<triangle_mesh.Vertices().size()<<", #triangles: "<<triangle_mesh.Elements().size()<<std::endl;
}

void Test_IO()
{
	std::cout<<"--- Test_IO ---"<<std::endl;
	int x=1;
	if(!File::Directory_Exists("test_folder")){
		File::Create_Directory("test_folder");}
	std::string file_name="test_folder/test";
	File::Write_Binary_To_File(file_name,x);
	int y=0;
	File::Read_Binary_From_File(file_name,y);
	std::cout<<"[Write_Binary_To_File] write x="<<x<<std::endl;
	std::cout<<"[Read_Binary_To_File] read y="<<y<<std::endl;
}

int main()
{
    Test_Eigen();
    Test_Particles_And_Mesh();
	Test_IO();

    return 0;
}
