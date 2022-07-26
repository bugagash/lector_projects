#include <iostream>

template <typename T>
class SmartPointer {
private:
	T* ptr;

public:
	SmartPointer(T* pt): ptr(pt) {}

	SmartPointer(SmartPointer& other) {

	}

	~SmartPointer() {
		delete ptr;
	}

	T& operator*() {
		return *ptr;
	}
};

int main() {


}