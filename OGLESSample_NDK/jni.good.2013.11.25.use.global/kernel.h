#include <list>

#ifndef AY_KERNEL
#define AY_KERNEL

#include "task.h"

class Kernel {
private:
	typedef std::list<Task*> TaskList;
	typedef std::list<Task*>::iterator TaskListIterator;

	TaskList m_tasks; // sorted by task's priority
	TaskList m_pausedTasks; // unsorted list

private:
	Kernel(const Kernel& _copy) {};
	Kernel operator=(const Kernel& _assign) {};

public:
	Kernel();
	virtual ~Kernel();

	void Execute();

	bool AddTask(Task* pTask);
	void SuspendTask(Task* pTask);
	void ResumeTask(Task* pTask);
	void RemoveTask(Task* pTask);
	void KillAllTask();

};

extern bool IsRunning;

#endif
