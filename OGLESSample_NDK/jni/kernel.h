#ifndef AY_KERNEL
#define AY_KERNEL

#include <list>
#include "pattern.h"
#include "task.h"

class Kernel
	: public Singleton<Kernel>
{
private:
	typedef std::list<Task*> TaskList;
	typedef std::list<Task*>::iterator TaskListIterator;

private:
	TaskList m_tasks; // sorted by task's priority, 0 > 1 > ... > 10
	TaskList m_pausedTasks; // unsorted list
	bool m_bRunning;
	
public:
	Kernel();
	virtual ~Kernel();

	void Execute();

	bool AddTask(Task* pTask);
	void SuspendTask(Task* pTask);
	void ResumeTask(Task* pTask);
	void RemoveTask(Task* pTask);
	void KillAllTask();

	bool isRunning () const { return this->m_bRunning; }
	void setRunning(bool running) { this->m_bRunning = running; }
};

#endif
