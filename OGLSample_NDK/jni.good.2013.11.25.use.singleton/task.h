#ifndef AY_TASK
#define AY_TASK

class Task {
private:
	unsigned int m_priority;
	bool m_canKill;

public:
	explicit Task(unsigned int priority) :
			m_priority(priority), m_canKill(false) {
	}
	virtual ~Task() {
	}

	virtual bool Start() = 0;
	virtual void OnSuspend() = 0;
	virtual void Update() = 0;
	virtual void OnResume() = 0;
	virtual void Stop() = 0;

	void SetCanKill(bool canKill) {
		m_canKill = canKill;
	}
	bool CanKill() {
		return m_canKill;
	}
	unsigned int Priority() {
		return m_priority;
	}
	void SetPriority(unsigned int priority) {
		m_priority = priority;
	}
};

#endif
