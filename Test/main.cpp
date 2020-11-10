#include <iostream>
#include "..\EasyDetour\EasyDetour.h"


void DummyFun(int val)
{
	std::cout << "DummyFun! \n";
}

class DummyClass
{
public:
	DummyClass() : value(0xFF)
	{

	}

	void DetourTest(int val)
	{
		std::cout << "DetourFun klass! \n";
	}
	
	int value;
};

int main()
{
	DummyClass klass;
	EasyDetour::EasyDetour<DummyClass, void, int> easyDet(&klass, DummyFun);
	
	//easyDet.HookFunction(&DummyClass::DetourTest);
	
	easyDet.HookFunction([](DummyClass* klass, int val)
	{
		std::cout << klass->value << std::endl;
	});
	
	DummyFun(4);

	return 0;
}