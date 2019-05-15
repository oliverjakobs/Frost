#pragma once

//---------------------------------------------------------------------------------------------------------------------
// Taken from GCC4, Chapter 7, pages 183 - 185  
//---------------------------------------------------------------------------------------------------------------------

#include "Process.h"

class ProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;
	ProcessList m_processList;

public:
	// construction
	~ProcessManager();

	// interface
	unsigned int UpdateProcesses(float deltaTime);  // updates all attached processes
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);  // attaches a process to the process mgr
	void AbortAllProcesses(bool immediate);

	// accessors
	unsigned int GetProcessCount() const { return m_processList.size(); }

private:
	void ClearAllProcesses();  // should only be called by the destructor
};