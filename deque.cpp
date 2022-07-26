#include "deque.h"
#include <iostream>
#include <string>

struct S {
	int* body;
	int sz;

	S(int* const& b, int _sz) {
		sz = _sz;
		body = new int[_sz];
		for (int i = 0; i < _sz; ++i) {
			*(body + i) = *(b + i);
		}
	}

	S(const S& s) {
		sz = s.sz;
		body = new int[sz];
		for (int i = 0; i < sz; ++i) {
			*(body + i) = *(s.body + i);
		}
	}

	~S() {
		delete [] body;
	}

	S& operator++() {
		for (auto i = 0; i < sz; ++i) {
			++body[i];
		}

		return *this;
	}

	bool operator==(const S& s) const {
		if (sz != s.sz) return false;
		else {
			for (auto i = 0; i < sz; ++i) {
				if (body[i] != s.body[i]) return false;
			}
		}
		return true;
	}

	bool operator!=(const S& s) const {
		return !(*this == s);
	}

	void print() {
		for (auto i = 0; i < sz; ++i) {
			std::cout << body[i] << ' ';
		}
		std::cout << '\n';
	}
};

int main() {
	/*int * x = new int[50];

	for (auto i = 0; i < 50; ++i) {
		x[i] = 1;
	}

	S a(x, 10);

	Deque<S> d(1, a);
	Deque<S> p;
	++a;
	for (auto i = 0; i < 1000; ++i) {
		d.push_back(++a);
	}
	for (auto i = 0; i < 999; ++i) {
		d.erase(d.end()-1);
	}
	std::cout << d.size() << "\n__________\n";
	std::cout << d[0].body[0] << '\n';
	p = d;
	std::cout << (p.size() == d.size()) << '\n';

	delete [] x;*/
	Deque<std::string> d, d2;
	for (int i = 0; i < 10000 - 1; ++i) {
		d.push_front("3443");
	}

	for (int i = 0; i < 9000; ++i) {
		d.pop_front();
	}

	for (int i = 0; i < 9998; ++i) {
		d2.push_back("3443");
	}
	d2.push_front("3444");
	d2.push_front("3443");
	d.insert(d.begin() + 1, "3444");
	//d.erase(d.begin() + 1);
	for (auto it = d.begin(), it2 = d2.begin(); it < d.end() && it2 < d2.end(); ++it, ++it2) {
		std::cout << (*it == *it2) << ' ';
	}
	std::cout << '\n';
}

