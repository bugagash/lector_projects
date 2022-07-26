//#include <stdlib.h>
#include <cstring>
#include <vector>
#include <string>

/*enum class sign : int8_t {
	neg = -1,
	zer = 0,
	pos = 1,

};*/

class BigInteger{
private:

	std::vector<uint32_t> body;
	uint32_t sz;
	int8_t sg;

	static const uint32_t RADIX = 1e9;


	void make_zero_size(uint32_t size = static_cast<uint32_t>(0)) {
		body.clear();
		sz = 0;
		sg = 1;
		for (uint32_t i = 0; i < size; ++i) {
			body.push_back(static_cast<uint32_t>(0));
			++sz;
		}
	}

public:

	class divide_by_zero : public std::exception {};	//That's because we have already started!
	class null_body : public std::exception {};			//That's too!
	class Overflowing_of_int_conversion : public std::exception {};

	friend std::ostream& operator<<(std::ostream& output, const BigInteger& bi);
	friend std::istream& operator>>(std::istream& input, BigInteger& bi);

	void plus(const BigInteger& bi1) {
		uint32_t ptr = 0;
		uint32_t CF = 0;
		uint32_t cur_val = 0;
		for (; ptr < sz && ptr < bi1.sz; ++ptr) {
			cur_val = body[ptr] + bi1.body[ptr] + CF;
			CF = cur_val / RADIX;
			body[ptr] = cur_val % RADIX;
		}
		for (; ptr < bi1.sz; ++ptr) {
			cur_val = bi1.body[ptr] + CF;
			CF = cur_val / RADIX;
			body.push_back(cur_val % RADIX);
			++sz;
		}
		for (; ptr < sz; ++ptr) {
			cur_val = body[ptr] + CF;
			CF = cur_val / RADIX;
			body[ptr] = cur_val % RADIX;
		}

		if (CF != 0) {
			body.push_back(CF);
			++sz;
		}
	}

	bool modul_bigger(const BigInteger& bi) const {
		if (sz != bi.sz) return (sz > bi.sz);
		for (uint32_t i = sz; i != 0; --i) {
			if (body[i - 1] < bi.body[i - 1]) return false;
			if (body[i - 1] > bi.body[i - 1]) return true;
		}
		return false;
	}

	bool modul_equal(const BigInteger& bi) const {
		if (sz != bi.sz) return false;
		for (uint32_t i = 0; i < sz; ++i) {
			if (body[i] != bi.body[i]) return false;
		}
		return true;
	}

	void minus(const BigInteger& bi) {
		if (modul_equal(bi)) {
			sg = 1;
			*this = 0;
			return;
		}
		uint32_t CF = 0, cur_val = 0;
		if (modul_bigger(bi)) {
			uint32_t i = 0;
			for (; i < sz && i < bi.sz; ++i) {
				cur_val = RADIX + body[i] - bi.body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
			for (; i < sz; ++ i) {
				cur_val = RADIX + body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
		}
		else {
			sg = -sg;
			uint32_t i = 0;
			for (; i < sz && i < bi.sz; ++i) {
				cur_val = RADIX + bi.body[i] - body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
			for (; i < bi.sz; ++i) {
				cur_val = RADIX + bi.body[i] - CF;
				body.push_back(cur_val % RADIX);
				CF = (cur_val >= RADIX ? 0 : 1);
				++sz;
			}
		}
	}

/*	void make_zero_size(uint32_t size = static_cast<uint32_t>(0)) {
		body.clear();
		sz = 0;
		sg = 1;
		for (uint32_t i = 0; i < size; ++i) {
			body.push_back(static_cast<uint32_t>(0));
			++sz;
		}
	}
*/
	std::string toString() const {
		//if (body.size() == 0) throw BigInteger::null_body();
		std::string str;
		uint32_t i = 0, deg = RADIX / 10;
		if (*this == static_cast<BigInteger>(0)) {
			str.push_back('0');
			return str;
		}
		if (sg == -1) str.push_back('-');

		uint32_t pos = sz - 1;
		for (; pos > 0; --pos) {
			if (body[pos] != 0) break;
		}

		for (i = 0, deg = RADIX / 10; i < 9 && (((body[pos] / deg) % 10) == 0); ++i, deg /= 10);
		for (; i < 9; ++i, deg /= 10) {
			str.push_back('0' + ((body[pos] / deg) % 10));
		}
		pos += 1;
		for (--pos; pos != 0; --pos) {
			for (i = 0, deg = RADIX / 10; i < 9; i++, deg /= 10) {
				str.push_back('0' + ((body[pos - 1] / deg) % 10));
			}
		}

		return str;
	}

	uint32_t size() const {
		return this->sz;
	}

	int8_t sign() const {
		return this->sg;
	}

	BigInteger() : sz(1), sg(1) {
		body.push_back(0);
	}

	BigInteger(const int& n) {
		if (n == 0) {
			sg = 1;
			sz = 1;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}
		int copy = n;
		sg = 1;
		sz = 0;
		if (n < 0) {
			copy = -copy;
			sg = -sg;
		}

		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const uint32_t& n) {
		if (n == 0) {
			sz = 1;
			sg = 1;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}
		uint32_t copy = n;
		sg = 1;
		sz = 0;
		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const size_t& n) {
		if (n == 0) {
			sz = 1;
			sg = 1;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}
		size_t copy = n;
		sg = 1;
		sz = 0;
		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const ssize_t& n) {
		if (n == 0) {
			sg = 1;
			sz = 1;
			body.push_back(0);
			return;
		}
		size_t copy = n;
		sg = 1;
		sz = 0;
		if (n < 0) {
			copy = -copy;
			sg = -sg;
		}

		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const BigInteger& bi1) {
		if (this == &bi1) return;

		sg = bi1.sg;
		sz = bi1.sz;

		body.clear();

		for (uint32_t i = 0; i < sz; ++i) body.push_back(bi1.body[i]);
	}

	BigInteger& neg() {
		sg = -sg;
		return *this;
	}

	bool operator==(const BigInteger& bi) const {
		if (sz != bi.sz || sg != bi.sg) {
			return false;
		}
		for (uint32_t i = 0; i < sz; ++i) {
			if (body[i] != bi.body[i]) return false;
		}
		return true;
	}

	bool operator!=(const BigInteger& bi) const {
		return !(*this == bi);
	}

	bool operator<(const BigInteger& bi) const {			//Everithing (sometimes except "==") define only "<"
		if (*this == bi) return false;
		if (sg != bi.sg) return (sg < bi.sg);
		if (sz != bi.sz) return (sz < bi.sz);
		if (sg == -1) {
			return modul_bigger(bi);
		}
		else {
			return !modul_bigger(bi);
		}
	}

	bool operator>(const BigInteger& bi1) const {
		return !(*this < bi1);
	}

	bool operator>=(const BigInteger& bi1) const {
		return (*this > bi1) || (*this == bi1);
	}

	bool operator<=(const BigInteger& bi1) const {
		return (*this < bi1) || (*this == bi1);
	}

	explicit operator bool() const {
		return (*this != 0);
	}

	explicit operator int() const {
		if (sz >= 1) throw BigInteger::Overflowing_of_int_conversion();
		int ans = body[0];
		if (sg == -1) ans = -ans;

		return ans;
	}

	/*operator bool() const {
		return (*this != 0);
	}*/

	BigInteger& operator+=(const BigInteger& bi) {
		if (sg == bi.sg) {
			plus(bi);
		}
		else minus(bi);
		return *this;
	}

	BigInteger& operator-=(const BigInteger& bi) {
		if (sg != bi.sg) {
			plus(bi);
		}
		else minus(bi);
		return *this;
	}

	BigInteger& operator++() {
		*this += 1;
		return *this;
	}

	BigInteger operator++(int) {
		BigInteger copy;
		copy = *this;
		*this += 1;
		return copy;
	}

	BigInteger& operator--() {
		*this -= 1;
		return *this;
	}

	BigInteger operator--(int) {
		BigInteger copy = *this;
		*this -= 1;
		return copy;
	}

	void remove_leading_zeros() {
		while (sz > 1 && body.back() == 0) {
			body.pop_back();
			--sz;
		}
	}

	BigInteger& operator*=(const BigInteger& bi) {
		BigInteger copy = *this;
		uint64_t cur_val = 0;
		uint32_t CF = 0;
		 
		copy.make_zero_size(sz + bi.sz);
		copy.sg = (sg * bi.sg);

		for (uint32_t i = 0; i < sz; ++i) {
			for (uint32_t j = 0; (j < bi.sz) || CF; ++j) {
				cur_val = static_cast<uint64_t>(copy.body[i + j] + static_cast<uint64_t>(body[i]) * (j < bi.sz ? bi.body[j] : 0) + CF);
				copy.body[i + j] = static_cast<uint32_t>(cur_val % RADIX);
				CF = static_cast<uint32_t>(cur_val / RADIX);
			}
		}

		copy.remove_leading_zeros();
		*this = copy;
		return *this;
	}

	BigInteger& operator*=(const ssize_t& x) {
		if (*this == 0) return *this;
		uint64_t cur_val = 0;
		uint32_t CF = 0;
		if (x < 0) sg = -sg;

		for (uint32_t i = 0; i < sz; ++i) {
			cur_val = (static_cast<uint64_t>(body[i]) + CF) * x;
			body[i] = cur_val % static_cast<uint64_t>(RADIX);
			CF = cur_val / static_cast<uint64_t>(RADIX);
		}

		while (CF) {
			body.push_back(CF % RADIX);
			++sz;
			CF /= RADIX;
		}
		this->remove_leading_zeros();
		return *this;
	}

	/*BigInteger& two_dividing() {					//Useless for my purpuses, but I'll live it here!
		if (*this == 0) {
			return *this;
		}
		uint32_t CF = 0, cur_val = 0;

		for (uint32_t i = sz - 1; i !=  -1; --i) {
			cur_val = CF + body[i] / 2;
			CF = (body[i] % 2 == 0 ? 0 : RADIX / 2);
			body[i] = cur_val;
		}

		this->remove_leading_zeros();
		return *this;
	}*/

	friend BigInteger operator*(const BigInteger& bi1, const BigInteger& bi2);	// How to avoid friend???
	friend BigInteger operator-(const BigInteger& bi1, const BigInteger& bi2);	// Same question...

	void shift_right() {
		if (sz == 0) {
			++sz;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}

		body.push_back(body[sz - 1]);
		++sz;
		for (uint32_t i = sz - 2; i > 0; --i) {
			body[i] = body[i - 1];
		}
		body[0] = 0;
	}

	BigInteger& operator/=(const BigInteger& bi) {
		if (bi == 0) throw BigInteger::divide_by_zero();
		if (*this == 0) return *this;
		if (modul_equal(bi)) {
			sg = sg * bi.sg;
			*this = 1;
			return *this;
		}
		if (!modul_bigger(bi)) {
			*this = 0;
			return *this;
		}
		sg = sg * bi.sg;
		//BigInteger right_copy = bi;
		BigInteger left = *this;
		BigInteger cur_val;
		this->make_zero_size(sz);

		for (uint32_t i = left.sz; i !=  0; --i) {
			cur_val.shift_right();
			cur_val.body[0] = left.body[i - 1];
			cur_val.remove_leading_zeros();
			//std::cout << "\nCur_val : " << cur_val;
			
			uint32_t x = 0, l = 0, r = RADIX;
			while(r >= l) {
				uint32_t m = (l + r) >> 1;
				BigInteger tmp = bi * m;
				//std::cout << "\nNow m is : " << m << " and tmp is : " << tmp;
				if (tmp <= cur_val) {
					x = m;
					l = m + 1;

				}
				else r = m - 1;
			}
			//std::cout << "\nThis Cur_Val x is : " << x << "\n";
			body[i - 1] = x;
			cur_val -= bi * x;
		}								

		this->remove_leading_zeros();
		return *this;
	}

	BigInteger& operator%=(const BigInteger& bi) {
		BigInteger copy = *this;
		copy /= bi;
		*this -= copy * bi;
		this->remove_leading_zeros();
		return *this;
	}

	BigInteger operator-() const {
		BigInteger copy = *this;
		copy.neg();
		return copy;
	}

	//BigInteger operator+(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator-(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator*(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator/(const BigInteger& bi) const {}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	~BigInteger() {}

};

void _reverse(char *buf, const int size) {
	char tmp;
	for (ssize_t i = 0, j = size - 1; i < (size >> 1); ++i, --j) {
		tmp = buf[i];
		buf[i] = buf[j];
		buf[j] = tmp;
	}
}

std::ostream& operator<<(std::ostream& output, const BigInteger& bi) {
	output << bi.toString();
	return output;
}

std::istream& operator>>(std::istream& input, BigInteger& bi) {
	bi.sz = 0;
	bi.sg = 1;
	bi.body.clear();
	std::string s;
	getline(input, s);
	char chunk[10];
	memset(chunk, '0', sizeof(char) * 10);
	size_t length = s.length();
	size_t index = 0;

	for (size_t i = length - 1; i != 0; --i) {
		chunk[index++] = s[i];
		if (index == 9) {
			_reverse(chunk, 9);
			chunk[index] = '\0';
			bi.body.push_back(atoi(chunk));
			++bi.sz;
			index = 0;
		}
	}
	if (s[0] == '-') {
		bi.sg = -1;
	}
	else {
		chunk[index++] = s[0];
	}
	if (index != 0) {
		chunk[index] = '\0';
		_reverse(chunk, index);
		bi.body.push_back(atoi(chunk));
		++bi.sz;
	}
	return input;
}

BigInteger operator+(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy += bi2;
	return copy;
}

BigInteger operator-(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy -= bi2;
	return copy;
}

BigInteger operator*(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy *= bi2;
	return copy;
}

BigInteger operator/(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy /= bi2;
	return copy;
}

BigInteger operator%(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy %= bi2;
	return copy;
}

void swap_bi(BigInteger& bi1, BigInteger& bi2) {
	BigInteger tmp = bi1;
	bi1 = bi2;
	bi2 = tmp;
}

BigInteger gcd_bi(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy_1 = bi1;
	BigInteger copy_2 = bi2;
	if (copy_1 == 0) {
		return copy_2;
	}
	while(copy_2 != 0){
		if(copy_2 == 0) return copy_1;
		//std::cout << "\na = " << copy_1 << "\nb = " << copy_2 << "\n";
		copy_1 %= copy_2;
		swap_bi(copy_1, copy_2);
	}
	return copy_1;
}




class Rational {
private:
	BigInteger numerator;
	BigInteger denomenator;
	int8_t sg;

public:
	Rational() : numerator(0), denomenator(1), sg(1) {}					//Creates zero!

	Rational(const int& n) {
		sg = (n >= 0 ? 1 : -1);
		numerator = n;
		denomenator = 1;
	}

	Rational(const ssize_t& n) {
		sg = (n >= 0 ? 1 : -1);
		numerator = n;
		denomenator = 1;
	}

	Rational(const double& _val) {
		sg = (_val >= 0 ? 1 : -1);
		double n = (_val >= 0 ? _val : -_val);
		uint32_t num = static_cast<uint32_t>(n);
		uint32_t den = 10;

		while ( !((n * den >= num * den) && (n * den <= num * den))  && (den <  100000000)) {
			num *= 10;
			num += (static_cast<uint32_t>(n * den) % 10);
			den *= 10;
		}

		num *= 10;

		this->numerator = num;
		this->denomenator = den;
		this->simplify();
	}

	Rational(const Rational& r) : numerator(r.numerator), denomenator(r.denomenator),  sg(r.sg) {}

	Rational(const BigInteger& bi) : numerator(bi), denomenator(1), sg(bi.sign()) {}

	~Rational() {}

	BigInteger f_denomenator() const {
		return denomenator;
	}

	BigInteger f_numerator() const {
		return numerator;
	}

	void simplify() {
		BigInteger gcd = gcd_bi(this->numerator, this->denomenator);
		numerator /= gcd;
		denomenator /= gcd;
	}

	void set_numerator(const BigInteger& _numerator) {
		this->numerator = _numerator;
		if (numerator < 0) {
			sg = -1;
			numerator.neg();
		}
		this->simplify();
	}

	void set_denomenator(const BigInteger& _denomenator) {
		this->denomenator = _denomenator;
		if (denomenator < 0) {
			sg = -1;
			denomenator.neg();
		}
		this->simplify();
	}

	void modul_plus(const Rational& ra) {
		this->numerator = (this->numerator * ra.denomenator + ra.numerator * this->denomenator);
		this->denomenator *= ra.denomenator;
		this->simplify();
	}

	void modul_minus(const Rational& ra) {
		this->numerator = (this->numerator * ra.denomenator - this->denomenator * ra.numerator);
		this->denomenator *= ra.denomenator;
		this->simplify();
	}

	Rational& operator+=(const Rational& ra) {
		if (sg == ra.sg) {
			modul_plus(ra);
		}

		else {
			modul_minus(ra);
			if (this->numerator.sign() == -1) {
				sg = -sg;
				numerator.neg();
			}
		}
		return *this;
	}

	Rational& operator-=(const Rational& ra) {
		if (sg == ra.sg) {
			modul_minus(ra);
			if (this->numerator.sign() == -1) {
				sg = -sg;
				numerator.neg();
			}
		}

		else {
			modul_plus(ra);
		}
		return *this;
	}

	Rational& operator*=(const Rational& ra) {
		sg = sg * ra.sg;
		this->numerator *= ra.numerator;
		this->denomenator *= ra.denomenator;
		this->simplify();
		return *this;
	}

	Rational& operator/=(const Rational& ra) {
		sg = sg * ra.sg;
		this->numerator *= ra.denomenator;
		this->denomenator *= ra.numerator;
		this->simplify();
		return *this;
	}

	Rational operator-() const {
		Rational copy = *this;
		copy.sg = -copy.sg;
		return copy;
	}

	bool operator<(const Rational& ra) const {
		if (sg != ra.sg) {
			return (sg < ra.sg);
		}
		return ((this->numerator * ra.denomenator < this->denomenator * ra.numerator) && (sg > 0));
	}

	bool operator>(const Rational& ra2) const {
		return !(*this < ra2);
	}

	bool operator==(const Rational& ra) const {
		if (this->sg != ra.sg) return false;
		return ((this->numerator == ra.numerator) && (this->denomenator == ra.denomenator));
	}

	bool operator!=(const Rational& ra) const {
		return !(*this == ra);
	}

	bool operator<=(const Rational& ra) const {
		return ((*this < ra) || (*this == ra));
	}

	bool operator>=(const Rational& ra) const {
		return ((*this > ra) || (*this == ra));
	}

	std::string toString() const {
		std::string _numerator = numerator.toString();
		std::string _denomenator = denomenator.toString();
		std::string ans;
		if (sg < 0) ans += '-';
		ans += _numerator;

		if (_denomenator != "1") {
			ans += "/";
			ans += _denomenator;
		}
		return ans;
	}

	std::string asDecimal(const size_t precision) const {
		std::string whole = (this->numerator / this->denomenator).toString();
		std::string tmp, fractional;
		BigInteger frac = (this->numerator % this->denomenator);

		for (size_t i = 0; i < (precision / 9 + 1); ++i) {
			frac.shift_right();
			tmp += (frac / this->denomenator).toString();
			frac %= this->denomenator;
			if (frac == 0) break;
		}

		for (size_t i = 0; tmp[i] && i < precision; ++i) {
			fractional += tmp[i];
		}

		for (size_t i = 0; fractional[i] && i < precision; ++i) {
			if (i == precision - 1) fractional[i] = '\0';
		}

		if (fractional.length() == 0) fractional += '0';
		if (sg < 0) {
			return ('-' + whole + '.' + fractional);
		}
		return (whole + '.' + fractional);
	}

	explicit operator double() const {
		uint32_t up = static_cast<int>(this->numerator);
		uint32_t down = static_cast<int>(this->denomenator);
		double d = static_cast<double>(up) / down;

		return d;
	}
};

Rational operator+(const Rational& ra1, const Rational& ra2) {
	Rational copy = ra1;
	copy += ra2;
	return copy;
}

Rational operator-(const Rational& ra1, const Rational& ra2) {
	Rational copy = ra1;
	copy -= ra2;
	return copy;
}

Rational operator*(const Rational& ra1, const Rational& ra2) {
	Rational copy = ra1;
	copy *= ra2;
	return copy;
}

Rational operator/(const Rational& ra1, const Rational& ra2) {
	Rational copy = ra1;
	copy /= ra2;
	return copy;
}
