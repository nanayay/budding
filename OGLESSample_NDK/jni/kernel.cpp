#include "kernel.h"
#include "log.h"

Kernel::Kernel() :
		m_bRunning(true),
		m_tasks(),
		m_pausedTasks()
{
}

Kernel::Kernel(Kernel& _copy)
{
	this->m_bRunning = _copy.isRunning();
	this->m_tasks = _copy.m_tasks;
	this->m_pausedTasks = _copy.m_pausedTasks;

	// Remove all items in the _copy, since the tasks should have **only** one copy
	_copy.m_tasks.clear();
	_copy.m_pausedTasks.clear();
	_copy.setRunning(false);

}

Kernel& Kernel::operator=(Kernel& _assign)
{
	if (this != &_assign)
	{
		this->m_bRunning = _assign.isRunning();
		this->m_tasks = _assign.m_tasks;
		this->m_pausedTasks = _assign.m_pausedTasks;

		// Remove all items in the _assign, since the tasks should have **only** one copy
		_assign.m_tasks.clear();
		_assign.m_pausedTasks.clear();
		_assign.setRunning(false);
	}
	return *this;
}

Kernel::~Kernel()
{
}

void Kernel::Execute()
{
	LOGD("Kernel::Execute() begin");

	while (m_tasks.size() != 0)
	{
		if (this->m_bRunning == false)
		{
			LOGD("kernel KillAllTask()");
			this->KillAllTask();
		}

		for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end(); i++)
		{
			if ((*i)->CanKill() != true)
			{
				LOGD("Kernel::Execute() call the contained Task's Update() begin");
				(*i)->Update();
				LOGD("Kernel::Execute() call the contained Task's Update() end");
			}
		}

		for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end();)
		{
			TaskListIterator tmp = i;
			i++;

			if ((*tmp)->CanKill())
			{
				LOGD("kernel Stop() and remove CanKill() task");
				(*tmp)->Stop();
				m_tasks.remove(*tmp);
				delete *tmp;
			}
		}
	}
	LOGD("Kernel::Execute() end");
}

bool Kernel::AddTask(Task* pTask) {
	if (pTask->Start() != true)
		return false;

	if (m_tasks.size() == 0)
	{
		m_tasks.push_back(pTask);
		return true;
	}

	TaskListIterator i;
	for (i = m_tasks.begin(); i != m_tasks.end(); i++) {
		if ((*i)->Priority() > pTask->Priority()) {
			break;
		}
	}
	m_tasks.insert(i, pTask);
	return true;
}

void Kernel::SuspendTask(Task* pTask) {
	if (std::find(m_tasks.begin(), m_tasks.end(), pTask) != m_tasks.end()) {
		pTask->OnSuspend();
		m_tasks.remove(pTask);
		m_pausedTasks.push_back(pTask);
	}
}

void Kernel::ResumeTask(Task* pTask) {
	if (std::find(m_pausedTasks.begin(), m_pausedTasks.end(), pTask)
			!= m_pausedTasks.end()) {
		pTask->OnResume();
		m_pausedTasks.remove(pTask);

		if (m_tasks.size() == 0)
		{
			m_tasks.push_back(pTask);
			return;
		}

		TaskListIterator p;
		for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end(); i++) {
			if ((*i)->Priority() > pTask->Priority()) {
				p = i;
				break;
			}
		}
		m_tasks.insert(p, pTask);
	}
}

void Kernel::RemoveTask(Task* pTask) {
	if (std::find(m_tasks.begin(), m_tasks.end(), pTask) != m_tasks.end()) {
		pTask->setCanKill(true);
	}
}

void Kernel::KillAllTask() {
	if (m_tasks.size() == 0)
	{
		return;
	}

	for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end(); i++) {
		(*i)->setCanKill(true);
	}
}

Kernel& Kernel::getSingleton() {
	static Kernel instance;
	return instance;
}
