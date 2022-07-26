#include <iostream>

int main() {
	int x = 12;

	int& a = x;

	++a;

	std::cout << x << ' ' << a  << '\n';
}