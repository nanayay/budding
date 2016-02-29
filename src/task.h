#ifndef AY_TASK
#define AY_TASK

class Task {
public:
	struct Priority{
		enum _PRIORITY_ { 
			Highest = 0,
			High = 3,
			Normal = 5,
			Low = 8,
			Lowest = 10
		};
	};

protected:
	unsigned int m_priority;
	bool m_canKill;

public:
	explicit Task(unsigned int priority = Task::Priority::Normal) :
			m_priority(priority), m_canKill(false) {
	}
	virtual ~Task() {
	}

	// Different with Init()
	// Start() return true will added to kernel for run, return false will not added to kernel
	// Init() will make sure the task be initialized before been called Update()
	virtual bool Start() = 0;
	virtual void OnSuspend() = 0;
	virtual void Update() = 0;
	virtual void OnResume() = 0;
	virtual void Stop() = 0;

	void setCanKill(bool canKill) {
		m_canKill = canKill;
	}
	bool CanKill() const {
		return m_canKill;
	}
	unsigned int Priority() const{
		return m_priority;
	}
	void setPriority(unsigned int priority) {
		m_priority = priority;
	}
};

#endif
