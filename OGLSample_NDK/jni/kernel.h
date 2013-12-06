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
	bool bRunning;

private:
	Kernel();
	Kernel(const Kernel& _copy);
	Kernel operator=(const Kernel& _assign);

public:
	virtual ~Kernel();

	void Execute();

	bool AddTask(Task* pTask);
	void SuspendTask(Task* pTask);
	void ResumeTask(Task* pTask);
	void RemoveTask(Task* pTask);
	void KillAllTask();

	static Kernel& GetSingleton();
	bool isRunning () const { return this->bRunning; }
	void SetRunning(bool running) { this->bRunning = running; }
};

#endif
