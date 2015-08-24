#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace aida64 {
	class API {
		HANDLE shm_ = INVALID_HANDLE_VALUE;

	public:
		API(const std::string& shmName = "AIDA64_SensorValues");
		~API();

		std::string read();
	};
}
