#include <iostream>
#include "..\EasyDetour\EasyDetour.h"


_declspec(noinline) float DummyFun(int val)
{
	std::cout << "	DummyFun! " << val << std::endl;
	return 250.50F + val;
}

_declspec(noinline) void CallDummyFunByPtr(void* lpFun, int value)
{
	if (lpFun != &DummyFun)
		return;
	
	((float(*)(int))lpFun)(value);
}

_declspec(noinline) int Add500(int val)
{
	std::cout << "	Add500!: " << val << std::endl;
	return 500 + val;
}


class DummyClass
{
public:
	DummyClass()
		: value(0xFF)
	{}

	float DetourTest(float(*lpFun)(int), int* val)
	{
		std::cout << "Class detour function! \n";
		return lpFun(*val) + 50;
	}
	
	int value;
};

int main()
{
	DummyClass klass;
	

	/*
	*	Detour to a class function
	*/

	auto detour_ClassExample = EasyDetour::make_detour(&klass, &DummyFun);

	if (!detour_ClassExample.HookFunction(&DummyClass::DetourTest))
		std::cerr << "Hook failed!" << std::endl;

	std::cout << "hooked DummyFun(25) result: " << DummyFun(25) << std::endl;
	std::cout << "CallDummyFunByPtr: " << std::endl;
	
	CallDummyFunByPtr((void*)&DummyFun, 25);

	if (!detour_ClassExample.UnHookFunction())
		std::cerr << "UnHook failed!" << std::endl;

	std::cout << "Unhooked DummyFun(25) result:" << DummyFun(25) << std::endl;
	

	/*
	*	Detour to a lambda function
	*/

	auto detour_LambdaExample = EasyDetour::make_detour(&klass, &Add500);
	
	const bool result = detour_LambdaExample.HookFunction([](auto klass, int(*lpOriginalFun)(int), int* val)
	{
		klass->value = *val;
		*val = 1000;
		std::cout << "lambda detour function" << std::endl;
		return lpOriginalFun(*val);
	});

	if (!result)
		std::cerr << "Hook failed!" << std::endl;

	std::cout << "hooked Add500(100) result: " << Add500(100) << std::endl;

	if (!detour_LambdaExample.UnHookFunction())
		std::cout << "Unhook failed!" << std::endl;

	std::cout << "Unhooked Add500(100) result: " << Add500(100) << std::endl;

	return 0;
}

