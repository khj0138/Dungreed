#include "hjRscManager.h"

namespace hj
{
	std::map<std::wstring, Resource*> RscManager::mRscManager;
	UINT RscManager::rIndex = 0;
}