#include "aida64.h"

aida64::API::API(const std::string& shmName /*= "AIDA64_SensorValues"*/)
{
	if ((shm_ = OpenFileMappingA(FILE_MAP_READ, FALSE, shmName.c_str())) == INVALID_HANDLE_VALUE)
		throw std::exception("Shared Memory cannot be opened");
}

aida64::API::~API()
{
	if (shm_ != INVALID_HANDLE_VALUE)
		CloseHandle(shm_);
}

std::string aida64::API::read() {
	if (shm_ == INVALID_HANDLE_VALUE)
		throw std::exception("Invalid handle value");

	auto pStr = MapViewOfFile(shm_, FILE_MAP_READ, 0, 0, 0);
	if (pStr == nullptr)
		throw std::exception("Can't read shared memory");

	std::string data((const char *)pStr);
	UnmapViewOfFile(pStr);

	return data;
}
