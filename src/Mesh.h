//#####################################################################
// Mesh
// Dartmouth COSC 89.18/189.02: Computational Methods for Physical Systems, Assignment starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#ifndef __Mesh_h__
#define __Mesh_h__
#include <fstream>
#include "Common.h"
#include "File.h"

////Simplicial mesh
////template parameter d specifies the dimension of a vertex, e.g., d=2 -- 2d vertex; d=3 -- 3d vertex
////template parameter e_d specifies the dimension of an element (the number of vertices of a simplex), e.g., e_d=2 -- segment; e_d=3, triangle; e_d=4, tetrahedra
template<int d,int e_d> class SimplicialMesh
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using VectorEi=Vector<int,e_d>;
public:
	////Basic attributes
	ArrayPtr<VectorD> vertices=nullptr;
	Array<VectorEi> elements;

	////Constructors
	SimplicialMesh(const ArrayPtr<VectorD> _vertices=nullptr)
	{
		if(_vertices==nullptr)vertices=std::make_shared<Array<VectorD> >();
		else vertices=_vertices;
	}

	////Copy constructor
	SimplicialMesh(const SimplicialMesh<d,e_d>& copy){*this=copy;}
	SimplicialMesh<d,e_d>& operator=(const SimplicialMesh<d,e_d>& copy)
	{
		if(vertices==nullptr)vertices=std::make_shared<Array<VectorD> >();
		*vertices=*(copy.vertices);
		elements=copy.elements;
		return *this;
	}

	////Access attributes
	static constexpr int Dim() {return d;}
	static constexpr int Element_Dim() {return e_d;}
	virtual Array<VectorD>& Vertices(){return *vertices.get();}
	virtual const Array<VectorD>& Vertices() const {return *vertices.get();}
	virtual Array<VectorEi>& Elements(){return elements;}
	virtual const Array<VectorEi>& Elements() const {return elements;}

	virtual void Clear()
	{
		if(vertices)vertices->clear();
		elements.clear();
	}

	////IO
	virtual void Write_Binary(std::ostream& output) const
	{
		int vtx_n=(int)(*vertices).size();
		File::Write_Binary(output,vtx_n);
		File::Write_Binary_Array(output,&(*vertices)[0],vtx_n);
		int e_n=(int)elements.size();
		File::Write_Binary(output,e_n);
		if(e_n>0)File::Write_Binary_Array(output,&elements[0],e_n);
	}

	virtual void Read_Binary(std::istream& input)
	{
		int vtx_n=0;File::Read_Binary(input,vtx_n);
		(*vertices).resize(vtx_n);
		File::Read_Binary_Array(input,&(*vertices)[0],vtx_n);
		int e_n=0;File::Read_Binary(input,e_n);
		if(e_n>0){
			elements.resize(e_n);
			File::Read_Binary_Array(input,&elements[0],e_n);}
	}

	virtual void Write_To_File_3d(const std::string& file_name) const
	{
		SimplicialMesh<3,e_d> s3;Dim_Conversion(*this,s3);
		File::Write_Binary_To_File(file_name,s3);
	}
};


template<int d> class TetrahedronMesh : public SimplicialMesh<d,4>
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using VectorEi=Vector4i;using Base=SimplicialMesh<d,4>;
public:
	TetrahedronMesh(const ArrayPtr<VectorD> _vertices=nullptr):Base(_vertices){}
};

template<int d> class TriangleMesh : public SimplicialMesh<d,3>
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using VectorEi=Vector3i;using Base=SimplicialMesh<d,3>;
public:
	TriangleMesh(const ArrayPtr<VectorD> _vertices=nullptr):Base(_vertices){}
};

template<int d> class SegmentMesh : public SimplicialMesh<d,2>
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using VectorEi=Vector2i;using Base=SimplicialMesh<d,2>;
public:
	SegmentMesh(const ArrayPtr<VectorD> _vertices=nullptr):Base(_vertices){}
};

////Helper functions

////Dimension and type conversion
////The reason to do dimension conversion on meshes is to guarantee the data can be visualized by our OpenGL viewer.
////Our current OpenGL viewer reads 3D data only. So if you want to visualize a 2D mesh, you need to convert it to 3D before writing to files.
template<class MESH_T1,class MESH_T2> void Dim_Conversion(const MESH_T1& mesh2,/*result*/MESH_T2& mesh3)
{
	mesh3.vertices->resize((int)(mesh2.vertices->size()));
	for(size_type i=0;i<(*mesh3.vertices).size();i++)
		Dim_Conversion<real,MESH_T1::Dim(),MESH_T2::Dim()>((*mesh2.vertices)[i],(*mesh3.vertices)[i],(real)0);
	mesh3.elements.resize((int)mesh2.elements.size());
	for(size_type i=0;i<mesh2.elements.size();i++)
		Dim_Conversion<int,MESH_T1::Element_Dim(),MESH_T2::Element_Dim()>(mesh2.elements[i],mesh3.elements[i],(int)-1);
}

#endif

