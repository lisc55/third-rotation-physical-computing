#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <Eigen/Dense>

using namespace std;
using my_int_set=std::set<int>;

void Test_Array()
{
	vector<int> array={1,2,3,4};
	array.resize(10,0);

	auto array2=array;

	int a=array2[3];
	std::cout<<"third element: "<<a<<std::endl;

	array2.clear();
	array2.resize(20,1);
	array2.push_back(2);

	std::cout<<"array size: "<<array.size()<<std::endl;
	std::cout<<"array element: ";
	for(auto& x:array2){
		std::cout<<x<<", ";}

	for(int i=0;i<array2.size();i++){
		std::cout<<array2[i]<<", ";}
}

void Test_Grammar()
{
	if(true){std::cout<<"this is true!"<<std::endl;}
	else{std::cout<<"this is false"<<std::endl;}

	////and: &&, or: ||, not: !, equal: ==, not equal: !=
	int i=0;

	////while loop
	while(i<10){
		std::cout<<"i = "<<i<<std::endl;
		i++;}

	////for loop
	for(int j=0;j<10;j++){
		std::cout<<"j = "<<j<<std::endl;}

	////switch
	int k=1;
	switch(k){
	case 0:
		std::cout<<"switch to k = 0"<<std::endl;
		break;
	case 1:
		std::cout<<"switch to k = 1"<<std::endl;
		break;
	default:
		std::cout<<"switch to default"<<std::endl;
		break;}
}

void Test_Container()
{
	my_int_set set;
	set.insert(1);
	set.insert(2);
	set.insert(3);
	set.insert(3);
	set.erase(2);

	std::cout<<"set #ele: "<<set.size()<<std::endl;
	std::cout<<"set ele: ";
	for(auto& s:set){
		cout<<s<<", ";
	}

	auto check=set.find(3);
	if(check!=set.end())
		std::cout<<"find 3"<<std::endl;

	std::cout<<"map"<<std::endl;
	map<int,string> mp;
	mp[1]="a";
	mp[2]="b";
	for(auto& iter:mp){
		std::cout<<"mp key: "<<iter.first<<", val: "<<iter.second<<std::endl;}

	////notice the returned value of find is a pointer, you need to use (* ) or -> to access its members
	auto iter=mp.find(1);
	std::cout<<"mp key: "<<(*iter).first<<", val: "<<iter->second<<std::endl;
}

void Test_Eigen()
{
	using Vector2=Eigen::Matrix<float,2,1>;
	Vector2 v=Vector2(1,2);
	Vector2 v2=Vector2(2,3);
	Vector2 v3=v+v2;
	float dot_prod=v.dot(v2);
	float norm=v.norm();
	Vector2 normal=v.normalized();
	v.normalize();
}

template<int d>
class Sphere
{
public:
	using VectorD=Eigen::Matrix<float,d,1>;
	VectorD center;
	float radius;

	Sphere(VectorD _c=VectorD::Zero(),float _r=1.f)
	{
		center=_c;
		radius=_r;
	}

	float Area(){return Area_Helper(*this);}
};

////use external function overloads to distinguish the implementations with specific template parameters
float Area_Helper(Sphere<2>& circle){std::cout<<"this is implementation for d=2"<<std::endl;return 3.14f*circle.radius*circle.radius;}
float Area_Helper(Sphere<3>& circle){std::cout<<"this is implementation for d=3"<<std::endl;return 4.f*3.14f*circle.radius*circle.radius;}

void main()
{
	std::cout<<"Hello world!"<<std::endl;

	Test_Container();
	Test_Eigen();

	using Vector2=Eigen::Matrix<float,2,1>;
	using Vector3=Eigen::Matrix<float,3,1>;
	Sphere<2> circle={Vector2(0,0),1.f};
	Sphere<3> sphere={Vector3(1,1,2),2.f};
	std::cout<<circle.Area()<<std::endl;
	std::cout<<sphere.Area()<<std::endl;	
}