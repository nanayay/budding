#ifndef AY_TASK
#define AY_TASK

class Task {
protected:
	unsigned int m_priority;
	bool m_canKill;

public:
	explicit Task(unsigned int priority) :
			m_priority(priority), m_canKill(false) {
	}
	virtual ~Task() {
	}

	virtual bool Start() = 0; // Different with Init()
							  // Start() return true will added to kernel for run, return false will not added to kernel
							  // Init() will make sure the task be initialized before been called Update()
	virtual void OnSuspend() = 0;
	virtual void Update() = 0;
	virtual void OnResume() = 0;
	virtual void Stop() = 0;

	void SetCanKill(bool canKill) {
		m_canKill = canKill;
	}
	bool CanKill() const {
		return m_canKill;
	}
	unsigned int Priority() const{
		return m_priority;
	}
	void SetPriority(unsigned int priority) {
		m_priority = priority;
	}
};

#endif
