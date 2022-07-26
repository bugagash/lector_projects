#include <time.h>
#include <iostream>
#include "BigInt.h"

#define TT std::cout << "CHECKED!\n";

int main() {
	Rational ra1, ra2;

	BigInteger numerator1, numerator2;
	BigInteger denomenator1, denomenator2;

	std::cout << "Enter numerator1 : ";
	std::cin >> numerator1;
	std::cout << "\nAnd denomenator1 : ";
	std::cin >> denomenator1;
	std::cout << "\nEnter numerator2 : ";
	std::cin >> numerator2;
	std::cout << "\nAnd denomenator2 : ";
	std::cin >> denomenator2;

	ra1.set_numerator(numerator1);				// NOTE FOR TOMORROW!!!! CHECK THIS  ---- !OK!
	TT
	ra1.set_denomenator(denomenator1);			// SAME								 ----- !OK!
	TT	
	ra2.set_numerator(numerator2);				// SAME								----- !OK!
	TT
	ra2.set_denomenator(denomenator2);			// SAME								-------- !OK!

	std::cout << "\nra1 = " << ra1.asDecimal(5) << "\nra2 = " << ra2.asDecimal(5) << "\n";
	ra1 *= ra2;
	std::cout << "I'm HERE!!!!\n\n\n";
	std::cout << ra1.toString() << " = " << ra1.asDecimal(5) << "\n";
}