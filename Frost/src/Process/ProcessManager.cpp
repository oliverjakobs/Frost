#include "ProcessManager.h"

ProcessManager::~ProcessManager()
{
	ClearAllProcesses();
}

// The process update tick.  Called every logic tick.  This function returns the number of process chains that 
// succeeded in the upper 32 bits and the number of process chains that failed or were aborted in the lower 32 bits.
unsigned int ProcessManager::UpdateProcesses(float deltaTime)
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		// grab the next process
		StrongProcessPtr pCurrProcess = (*it);

		// save the iterator and increment the old one in case we need to remove this process from the list
		ProcessList::iterator thisIt = it;
		++it;

		// process is uninitialized, so initialize it
		if (pCurrProcess->GetState() == Process::UNINITIALIZED)
			pCurrProcess->OnInit();

		// give the process an update tick if it's running
		if (pCurrProcess->GetState() == Process::RUNNING)
			pCurrProcess->OnUpdate(deltaTime);

		// check to see if the process is dead
		if (pCurrProcess->IsDead())
		{
			// run the appropriate exit function
			switch (pCurrProcess->GetState())
			{
			case Process::SUCCEEDED:
			{
				pCurrProcess->OnSuccess();
				StrongProcessPtr pChild = pCurrProcess->RemoveChild();
				if (pChild)
					AttachProcess(pChild);
				else
					++successCount;  // only counts if the whole chain completed
				break;
			}

			case Process::FAILED:
			{
				pCurrProcess->OnFail();
				++failCount;
				break;
			}

			case Process::ABORTED:
			{
				pCurrProcess->OnAbort();
				++failCount;
				break;
			}
			}

			// remove the process and destroy it
			m_processList.erase(thisIt);
		}
	}

	return ((successCount << 16) | failCount);
}

// Attaches the process to the process list so it can be run on the next update.
WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr pProcess)
{
	m_processList.push_front(pProcess);
	return WeakProcessPtr(pProcess);
}

// Aborts all processes.  If immediate == true, it immediately calls each ones OnAbort() function and destroys all 
// the processes.
void ProcessManager::AbortAllProcesses(bool immediate)
{
	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		ProcessList::iterator tempIt = it;
		++it;

		StrongProcessPtr pProcess = *tempIt;
		if (pProcess->IsAlive())
		{
			pProcess->SetState(Process::ABORTED);
			if (immediate)
			{
				pProcess->OnAbort();
				m_processList.erase(tempIt);
			}
		}
	}
}

// Clears all processes (and DOESN'T run any exit code)
void ProcessManager::ClearAllProcesses()
{
	m_processList.clear();
}
