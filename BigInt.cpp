#include <iostream>
#include <time.h>
#include "BigInt.h"

int main() {
	double time;

	BigInteger a;
	BigInteger b;

	std::cin >> a >> b;
	
	/*
	time = (clock() - time) / CLOCKS_PER_SEC;
	std::cout << "\ntime of calculating\na = " << a << " and\nb = " << b << "\nIs : " << time << "\n";	
	*/
	time = clock();
	a *= -a;
	time = (clock() - time) / CLOCKS_PER_SEC;
	std::cout << "\nTime of dividing : " << time << "\nResult : " << a << "\n";

}