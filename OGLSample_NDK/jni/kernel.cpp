#include "kernel.h"

Kernel::Kernel() :
		bRunning(true) {
}

Kernel::~Kernel() {
}

void Kernel::Execute() {
	LOGD("kernel Execute() begin");

	while (m_tasks.size() != 0) {
		LOGD("kernel m_tasks is not 0 size");
		if (this->bRunning == false) {
			LOGD("kernel will KillAllTask");
			this->KillAllTask();
		}

		for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end(); i++) {
			if ((*i)->CanKill() != true) {
				LOGD("kernel task begin to update");
				(*i)->Update();
				LOGD("kernel task end to update");
			}
		}

		for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end();) {
			TaskListIterator tmp = i;
			i++;

			if ((*tmp)->CanKill()) {
				LOGD("kernel begin to remove task");
				(*tmp)->Stop();
				m_tasks.remove(*tmp);
				delete *tmp;
				LOGD("kernel end to remove task");
			}
		}
	}
	LOGD("kernel Execute() end");
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
		pTask->SetCanKill(true);
	}
}

void Kernel::KillAllTask() {
	if (m_tasks.size() == 0)
	{
		return;
	}

	for (TaskListIterator i = m_tasks.begin(); i != m_tasks.end(); i++) {
		(*i)->SetCanKill(true);
	}
}

Kernel& Kernel::GetSingleton() {
	static Kernel instance;
	return instance;
}
