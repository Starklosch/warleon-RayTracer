template <typename T>
class Reference {
	T* ref;
	bool manage;
public:
	Reference(const Reference& other) : ref(other.ref), manage(false) {}
	Reference(Reference&& other) : ref(other.ref), manage(other.manage) {
		other.ref = nullptr;
	}

	Reference(T& value) : ref(&value), manage(false) {}
	Reference(T&& value) : ref(new T(value)), manage(true) {}

	~Reference() {
		if (manage && ref != nullptr)
			delete ref;
	}

	Reference& operator=(const Reference& other) {
		if (this != &other) {
			ref = other.ref;
			manage = false;
		}
		return *this;
	}

	Reference& operator=(Reference&& other) {
		if (this != &other) {
			ref = other.ref;
			manage = other.manage;
			other.ref = nullptr;
		}
		return *this;
	}

	T& operator*() {
		return *ref;
	}

	T* operator->() const {
		return ref;
	}

	T& get() {
		return *ref;
	}

	const T& get() const {
		return *ref;
	}

	operator T&() {
		return *ref;
	}

	operator const T& () const {
		return *ref;
	}
};