#include "gl_profile.h"

using namespace el;

GLProfileBusyWait::GLProfileBusyWait() : 
	_cpuTime(0.0), 
	_gpuTime(0.0)
{
	_query[0] = 0;
	_query[1] = 0;
	GL_CHECK(glGenQueries(2, _query));
}

GLProfileBusyWait::~GLProfileBusyWait()
{
	GL_CHECK(glDeleteQueries(2, _query));
}

void GLProfileBusyWait::start()
{
	_cpuTimePoint[0] = std::chrono::high_resolution_clock::now();
	GL_CHECK(glQueryCounter(_query[0], GL_TIMESTAMP));
}

void GLProfileBusyWait::end()
{
	_cpuTimePoint[1] = std::chrono::high_resolution_clock::now();

	auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>( _cpuTimePoint[1] - _cpuTimePoint[0]);
	_cpuTime = static_cast<float>(timeElapsed.count() / 1000.0);

	GL_CHECK(glQueryCounter(_query[1], GL_TIMESTAMP));

	GLint stopTimerAvailable = 0;
	while(!stopTimerAvailable) {
		GL_CHECK(glGetQueryObjectiv(_query[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable));
	}

	// get query results
	GLuint64 startTime = 0, stopTime = 0;
	GL_CHECK(glGetQueryObjectui64v(_query[0], GL_QUERY_RESULT, &startTime));
	GL_CHECK(glGetQueryObjectui64v(_query[1], GL_QUERY_RESULT, &stopTime));

	_gpuTime = static_cast<float>((stopTime - startTime) / 1000000.0);
}

void GLProfileBusyWait::setName(std::string name)
{
	_name = std::move(name);
}

const std::string& GLProfileBusyWait::getName() const
{
	return _name;
}

float GLProfileBusyWait::getCpuTime() const
{
	return _cpuTime;
}

float GLProfileBusyWait::getGpuTime() const
{
	return _gpuTime;
}
