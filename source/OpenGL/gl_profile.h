#pragma once

#include <string>
#include <chrono>

#include "gl.h"

namespace el {

class GLProfileBusyWait final
{
public:

	using time_point = std::chrono::high_resolution_clock::time_point;

	GLProfileBusyWait();
	~GLProfileBusyWait();

	void start();
	void end();

	void setName(std::string name);
	const std::string& getName() const;

	float getCpuTime() const;
	float getGpuTime() const;

private:

	GLuint _query[2];
	float _cpuTime, _gpuTime;

	std::string _name;
	time_point _cpuTimePoint[2];
};

} // namespace el {
