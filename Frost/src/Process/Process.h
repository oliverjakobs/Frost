#pragma once

//---------------------------------------------------------------------------------------------------------------------
// Taken from GCC4, Chapter 7, pages 180 - 182  
//---------------------------------------------------------------------------------------------------------------------

#include "Utility/Utils.h"
#include "Utility/Debugger.h"

class Process;
typedef shared_ptr<Process> StrongProcessPtr;
typedef weak_ptr<Process> WeakProcessPtr;

//---------------------------------------------------------------------------------------------------------------------
// Process class
// 
// Processes are ended by one of three methods: Success, Failure, or Aborted.
//		- Success means the process completed successfully.  If the process has a child, it will be attached to 
//		  the process mgr.
//		- Failure means the process started but failed in some way.  If the process has a child, it will be 
//		  aborted.
//		- Aborted processes are processes that are canceled while not submitted to the process mgr.  Depending 
//		  on the circumstances, they may or may not have gotten an OnInit() call.  For example, a process can 
//		  spawn another process and call AttachToParent() on itself.  If the new process fails, the child will
//		  get an Abort() call on it, even though its status is RUNNING.
//---------------------------------------------------------------------------------------------------------------------
class Process
{
	friend class ProcessManager;

public:
	enum State
	{
		// Processes that are neither dead nor alive
		UNINITIALIZED = 0,  // created but not running
		REMOVED,  // removed from the process list but not destroyed; this can happen when a process that is already running is parented to another process

		// Living processes
		RUNNING,  // initialized and running
		PAUSED,  // initialized but paused

		// Dead processes
		SUCCEEDED,  // completed successfully
		FAILED,  // failed to complete
		ABORTED,  // aborted; may not have started
	};

private:
	State m_state;  // the current state of the process
	StrongProcessPtr m_pChild;  // the child process, if any

public:
	// construction
	Process();
	virtual ~Process();

protected:
	// interface; these functions should be overridden by the subclass as needed
	virtual void OnInit() { m_state = RUNNING; }  // called during the first update; responsible for setting the initial state (typically RUNNING)
	virtual void OnUpdate(float deltaTime) = 0;  // called every frame
	virtual void OnSuccess() { }  // called if the process succeeds (see below)
	virtual void OnFail() { }  // called if the process fails (see below)
	virtual void OnAbort() { }  // called if the process is aborted (see below)

public:
	// Functions for ending the process.
	inline void Succeed();
	inline void Fail();

	// pause
	inline void Pause();
	inline void UnPause();

	// accessors
	State GetState() const { return m_state; }
	bool IsAlive() const { return (m_state == RUNNING || m_state == PAUSED); }
	bool IsDead() const { return (m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED); }
	bool IsRemoved() const { return (m_state == REMOVED); }
	bool IsPaused() const { return m_state == PAUSED; }

	// child functions
	inline void AttachChild(StrongProcessPtr pChild);
	StrongProcessPtr RemoveChild();  // releases ownership of the child
	StrongProcessPtr PeekChild() { return m_pChild; }  // doesn't release ownership of the child

private:
	void SetState(State newState) { m_state = newState; }
};

inline void Process::Succeed()
{
	DEBUG_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = SUCCEEDED;
}

inline void Process::Fail()
{
	DEBUG_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = FAILED;
}

inline void Process::AttachChild(StrongProcessPtr pChild)
{
	if (m_pChild)
		m_pChild->AttachChild(pChild);
	else
		m_pChild = pChild;
}

inline void Process::Pause()
{
	if (m_state == RUNNING)
		m_state = PAUSED;
	else
		DEBUG_WARN("Attempting to pause a process that isn't running");
}

inline void Process::UnPause()
{
	if (m_state == PAUSED)
		m_state = RUNNING;
	else
		DEBUG_WARN("Attempting to unpause a process that isn't paused");
}