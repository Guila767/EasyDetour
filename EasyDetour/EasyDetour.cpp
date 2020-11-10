#include "EasyDetour.h"

namespace easydetour_internals
{
	std::map<void*, void*> __easy_detour_Iternal_class::easydetourMap;
	std::mutex __easy_detour_Iternal_class::mtx;
}

void easydetour_internals::__easy_detour_Iternal_class::_addEasyDetourInstance(void* pInstance, void* pDetourTargetFunction)
{
	easydetourMap.insert({ pDetourTargetFunction, pInstance });
}

void easydetour_internals::__easy_detour_Iternal_class::_removeDetourInstance(void* pDetourTargetFunction)
{
	auto it = easydetourMap.find(pDetourTargetFunction);
	if (it == easydetourMap.end())
		return;
	easydetourMap.erase(it);
}

void* easydetour_internals::__easy_detour_Iternal_class::_getDetourInstance(void* pDetourTargetFunction)
{
	mtx.lock();
	auto it = easydetourMap.find(pDetourTargetFunction);
	if (it == easydetourMap.end())
	{
		mtx.unlock();
		return nullptr;
	}
	mtx.unlock();
	return it->second;
};