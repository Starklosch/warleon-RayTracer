#pragma once

namespace war{
class ID{
public:
	typedef size_t type;
private:
	mutable id_t count;
public:
	ID():count(0){}
	id_t generate() const{
		return ++count;
	}
};
};
