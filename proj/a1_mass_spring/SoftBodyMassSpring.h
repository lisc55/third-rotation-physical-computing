//#####################################################################
// Mass-spring deformable model
// Dartmouth COSC 89.18/189.02: Computational Methods for Physical Systems, Assignment starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################

#ifndef __SoftBodyMassSpring_h__
#define __SoftBodyMassSpring_h__
#include "Common.h"
#include "Particles.h"

template<int d> class SoftBodyMassSpring
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using MatrixD=Matrix<real,d>;
public:
	////Spring parameters
	Particles<d> particles;
	Array<Vector2i> springs;
	Array<real> rest_length;
	Array<real> ks;
	Array<real> kd;

	////Boundary nodes
	Hashtable<int,VectorD> boundary_nodes;

	////Body force
	VectorD g=VectorD::Unit(1)*(real)-1.;
	
	enum class TimeIntegration{ExplicitEuler,ImplicitEuler} time_integration=TimeIntegration::ExplicitEuler;

	////Implicit time integration
	SparseMatrixT K;
	VectorX u,b;

	virtual void Initialize()
	{
		////Initialize default spring parameters for standard tests
		real ks_0=(real)1,kd_0=(real)1;
		switch(time_integration){
		case TimeIntegration::ExplicitEuler:{
			ks_0=(real)5e2;
			kd_0=(real)1e1;
		}break;
		case TimeIntegration::ImplicitEuler:{
			ks_0=(real)1e5;
			kd_0=(real)1e1;			
		}break;}

		////Allocate arrays for springs and parameters
		rest_length.resize(springs.size());
		for(int i=0;i<(int)springs.size();i++){const Vector2i& s=springs[i];
			rest_length[i]=(particles.X(s[0])-particles.X(s[1])).norm();}
		ks.resize(springs.size(),ks_0);
		kd.resize(springs.size(),kd_0);

		////Allocate sparse matrix if using implicit time integration 
		////This function needs to be called for only once since the mesh doesn't change during the simulation)
		if(time_integration==TimeIntegration::ImplicitEuler)
			Initialize_Implicit_K_And_b();
	}

	virtual void Advance(const real dt)
	{
		switch(time_integration){
		case TimeIntegration::ExplicitEuler:
			Advance_Explicit_Euler(dt);break;
		case TimeIntegration::ImplicitEuler:
			Advance_Implicit_Euler(dt);break;}
	}
	
	////Set boundary nodes
	void Set_Boundary_Node(const int p,const VectorD v=VectorD::Zero()){boundary_nodes[p]=v;}
	
	bool Is_Boundary_Node(const int p){return boundary_nodes.find(p)!=boundary_nodes.end();}
	
	void Enforce_Boundary_Conditions()
	{
		for(auto p:boundary_nodes){
			int idx=p.first;					////get boundary particle index
			const VectorD& v=p.second;			////get boundary particle velocity
			particles.V(idx)=v;					////set boundary particle velocity
			particles.F(idx)=VectorD::Zero();}	////clear boundary particle force
	}

	//////////////////////////////////////////////////////////////////////////
	////P1 TASK: explicit Euler integration and spring force calculation

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P1 TASK): explicit Euler time integration 
	void Advance_Explicit_Euler(const real dt)
	{
		Particle_Force_Accumulation();

		////Update particle velocity and position
		/* Your implementation start */

		/* Your implementation end */
	}

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P1 TASK): compute spring force f_ij=f_s+f_d 
	VectorD Spring_Force_Calculation(const int idx)
	{
		/* Your implementation start */

		/* Your implementation end */

		//return VectorD::Zero();	////replace this line with your implementation
	}

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P1 TASK): accumulate spring forces to particles
	void Particle_Force_Accumulation()
	{
		////Clear forces on particles
		for(int i=0;i<particles.Size();i++){particles.F(i)=VectorD::Zero();}

		////Accumulate body forces
		for(int i=0;i<particles.Size();i++){
			particles.F(i)+=particles.M(i)*g;}

		////Accumulate spring forces
		/* Your implementation start */

		/* Your implementation end */

		////Enforce boundary conditions
		Enforce_Boundary_Conditions();
	}

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P2 TASK): 
	////Construct K, step 1: initialize the matrix structure 
	void Initialize_Implicit_K_And_b()
	{
		int n=d*particles.Size();
		K.resize(n,n);u.resize(n);u.fill((real)0);b.resize(n);b.fill((real)0);
		Array<TripletT> elements;
		for(int s=0;s<(int)springs.size();s++){int i=springs[s][0];int j=springs[s][1];
			Add_Block_Triplet_Helper(i,i,elements);
			Add_Block_Triplet_Helper(i,j,elements);
			Add_Block_Triplet_Helper(j,i,elements);
			Add_Block_Triplet_Helper(j,j,elements);}
		K.setFromTriplets(elements.begin(),elements.end());
		K.makeCompressed();	
	}

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P2 TASK): 
	////Construct K, step 2: fill nonzero elements in K
	void Update_Implicit_K_And_b(const real dt)
	{
		////Clear K and b
		K.setZero();
		b.fill((real)0);

		/* Your implementation start */

		/* Your implementation end */
	}

	//////////////////////////////////////////////////////////////////////////
	////P2 TASK: Implicit Euler time integration

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P2 TASK): 
	////Construct K, step 2.1: compute spring force derivative
	void Compute_Ks_Block(const int s,MatrixD& Ks)
	{
		/* Your implementation start */

		/* Your implementation end */
	}

	//////////////////////////////////////////////////////////////////////////
	////YOUR IMPLEMENTATION (P2 TASK): 
	////Construct K, step 2.2: compute damping force derivative
	void Compute_Kd_Block(const int s,MatrixD& Kd)
	{
		/* Your implementation start */

		/* Your implementation end */
	}

	////Implicit Euler time integration
	void Advance_Implicit_Euler(const real dt)
	{
		Particle_Force_Accumulation();
		Update_Implicit_K_And_b(dt);

		for(int i=0;i<particles.Size();i++){
			for(int j=0;j<d;j++)u[i*d+j]=particles.V(i)[j];}	////set initial guess to be the velocity from the last time step

		SparseSolver::CG(K,u,b);	////solve Ku=b using Conjugate Gradient

		for(int i=0;i<particles.Size();i++){
			VectorD v;for(int j=0;j<d;j++)v[j]=u[i*d+j];
			particles.V(i)=v;
			particles.X(i)+=particles.V(i)*dt;}
	}

protected:
	////Add block nonzeros to sparse matrix elements (for initialization)
	void Add_Block_Triplet_Helper(const int i,const int j,Array<TripletT>& elements)
	{for(int ii=0;ii<d;ii++)for(int jj=0;jj<d;jj++)elements.push_back(TripletT(i*d+ii,j*d+jj,(real)0));}

	////Add block Ks to K_ij
	void Add_Block_Helper(SparseMatrixT& K,const int i,const int j,const MatrixD& Ks)
	{
		SparseFunc::Add_Block<d,MatrixD>(K,i,i,Ks);
		SparseFunc::Add_Block<d,MatrixD>(K,j,j,Ks);
		if(!Is_Boundary_Node(i)&&!Is_Boundary_Node(j)){
			SparseFunc::Add_Block<d,MatrixD>(K,i,j,-Ks);
			SparseFunc::Add_Block<d,MatrixD>(K,j,i,-Ks);}
	}

	////Set block values on a vector
	void Set_Block(VectorX& b,const int i,const VectorD& bi)
	{for(int ii=0;ii<d;ii++)b[i*d+ii]=bi[ii];}

	////Add block values to a vector
	void Add_Block(VectorX& b,const int i,const VectorD& bi)
	{for(int ii=0;ii<d;ii++)b[i*d+ii]+=bi[ii];}
};

#endif
