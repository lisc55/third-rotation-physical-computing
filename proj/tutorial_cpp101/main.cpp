#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <Eigen/Dense>

void Test_Vector()
{
	std::vector<int> array={1,2,3,4};
	
	array.resize(10,1);

	array.push_back(11);

	std::cout<<"array size: "<<array.size()<<std::endl;
	std::cout<<"array ele: ";
	for(auto& a:array)
		std::cout<<a<<", ";
	std::cout<<std::endl;

	for(int i=0;i<array.size();i++)
		std::cout<<array[i]<<", ";
	std::cout<<std::endl;
}

void Test_Set()
{
	std::cout<<"Test set"<<std::endl;

	std::set<int> set={1,2,2,3};
	set.insert(4);
	set.erase(2);

	int e=5;
	auto iter=set.find(e);
	if(iter==set.end())
		std::cout<<"cannot find element "<<e<<std::endl;
	else 
		std::cout<<"find element "<<e<<std::endl;

	std::cout<<"set size: "<<set.size()<<std::endl;
	std::cout<<"set ele: ";

	for(auto& s:set)
		std::cout<<s<<", ";
}

void Test_Map()
{
	std::cout<<"Test map"<<std::endl;

	std::map<int,std::string> map;
	map[1]="a";
	map[2]="b";
	map[3]="c";

	std::cout<<"map size: "<<map.size()<<std::endl;
	std::cout<<"map ele: ";
	for(auto& s:map)
		std::cout<<"["<<s.first<<", "<<s.second<<"], ";
	std::cout<<std::endl;

	auto x=map.find(2);
	if(x!=map.end())
		std::cout<<"find "<<x->first<<", "<<x->second<<std::endl;
}

void Test_Eigen()
{
	using Vector2=Eigen::Matrix<float,2,1>;
	Vector2 v1=Vector2(1,2);
	std::cout<<"v1="<<v1.transpose()<<std::endl;
	Vector2 v2=Vector2(0,1);
	float length=v1.norm();
	Vector2 normal=v1.normalized();

	std::cout<<"dot: "<<v1.dot(v2)<<std::endl;
	std::cout<<"length:\n"<<length<<std::endl;
	std::cout<<"normal:\n"<<normal<<std::endl;

	////TODO: add, minus, scalar product, access element, unit vector, zero vector, all one vector
}

template<int d> using Vector=Eigen::Matrix<float,d,1>;

template<int d>
class Sphere
{
public:
	Vector<d> center;
	float radius;
	Vector<d> velocity;
};

template<int d> bool Check_Collision(const Sphere<d>& s1,const Sphere<d>& s2)
{
	return (s1.center-s2.center).norm()<(s1.radius+s2.radius);
}

template<int d> bool Check_Wall(const Sphere<d>& s,float wall_left,float wall_right)
{
	return (s.center[0]<wall_left||s.center[0]>wall_right);
}

void Test_Template()
{
	const int d=3;
	Sphere<d> s1={Vector<d>::Unit(0)*1.f,1.f,Vector<d>::Unit(0)*1.f};
	Sphere<d> s2={Vector<d>::Unit(0)*8.f,1.f,Vector<d>::Unit(0)*-1.f};
	std::cout<<"Check collision: "<<Check_Collision(s1,s2)<<std::endl;

	float wall_left=0.f;
	float wall_right=10.f;
	float dt=1;
	for(int frame=0;frame<100;frame++){
		s1.center+=s1.velocity*dt;
		s2.center+=s2.velocity*dt;
		std::cout<<"Frame "<<frame<<": "<<s1.center[0]<<", "<<s2.center[0]<<std::endl;

		if(Check_Collision(s1,s2)){
			std::cout<<"S1 collides S2!"<<std::endl;
			s1.velocity*=-1.f;
			s2.velocity*=-1.f;
		}
		if(Check_Wall(s1,wall_left,wall_right)){
			std::cout<<"S1 collides wall"<<std::endl;
			s1.velocity*=-1.f;
		}
		if(Check_Wall(s2,wall_left,wall_right)){
			std::cout<<"S2 collides wall"<<std::endl;
			s2.velocity*=-1.f;
		}
	}
}

int main()
{
	std::cout<<"COSC 89.18/189.02: C++11 and Eigen tutorial!"<<std::endl;
	Test_Vector();
	Test_Set();
	Test_Map();
	Test_Eigen();
	Test_Template();
}
