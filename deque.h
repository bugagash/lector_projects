#include <iostream>


/*template<typename T>
struct add_reff {
	using type = T&;
};

template<typename T>
struct add_reff<T&> {
	using type = T&;
};

template<typename T>
using add_reff_t = typename add_reff<T>::type;*/



template <typename T>
class Deque {
private:

	struct deque_index {
		size_t chunk;
		size_t position;
	};

	T** body;
	deque_index begin_index;
	deque_index end_index;
	size_t SZ = 0;
	size_t CP = 8;
	static const size_t ch_size = 32;

	void resize(size_t new_CP) {
		if (new_CP <= CP) {
			return;
		}

		//T** new_body = reinterpret_cast<T**>(new uint8_t*[sizeof(T*) * new_CP]);
		T** new_body = new T*[new_CP];
		for (size_t i = 0; i < new_CP; ++i) {
			if ( (i >= (new_CP - CP) / 2) && (i < (new_CP + CP) / 2) ) {
				continue;
			}
			new_body[i] = reinterpret_cast<T*>(new uint8_t[ch_size * sizeof(T)]);
		}

		for (size_t ind = 0, i = (new_CP - CP) / 2; ind < CP; ++ind, ++i) {
			new_body[i] = body[ind];
		}

		//for (size_t i = begin_index.chunk; i <= end_index.chunk; ++i) 

		delete [] body;
		body = new_body;
		begin_index.chunk += (new_CP - CP) / 2;
		end_index.chunk += (new_CP - CP) / 2;
		CP = new_CP;
	}


	template <bool isConst>
	class base_iterator : public std::iterator< std::bidirectional_iterator_tag, typename std::conditional<isConst, const T, T>::type > {
	friend class Deque<T>;

	private:

		using iterator_type = base_iterator<isConst>;
		//using body_ref_type = typename std::conditional<isConst, T** const&, T**&>::type;
		using reference_type = typename std::conditional<isConst, const T&, T&>::type;
		using pointer_type = typename std::conditional<isConst, const T*, T*>::type;
		using value_type = typename std::conditional<isConst, T** const, T**>::type;

		T** it_body;
		size_t ch;
		size_t pos;

	public:

		base_iterator(T** _body, size_t _ch, size_t _pos) : it_body(_body), ch(_ch), pos(_pos) {}
		base_iterator(const base_iterator& it) : it_body(it.it_body), ch(it.ch), pos(it.pos) {}

		iterator_type& operator=(const iterator_type& it) {
			it_body = it.it_body;
			ch = it.ch;
			pos = it.pos;
			return *this;
		}

		iterator_type& operator++() {
			if (++pos < ch_size) {
				return *this;
			}
			ch += 1;
			pos = 0;
			return *this;
		}

		iterator_type operator++(int) {
			iterator_type copy = *this;
			++(*this);
			return copy;
		}

		iterator_type& operator--() {
			if (pos >= 1) {
				--pos;
				return *this;
			}

			pos = ch_size - 1;
			--ch;
			return *this;
		}

		iterator_type operator--(int) {
			iterator_type copy = *this;
			--(*this);
			return copy;
		}

		iterator_type& operator+=(size_t n) {
			ch += (pos + n) / ch_size;
			pos = (pos + n) % ch_size;
			return *this;
		}

		iterator_type operator+(size_t n) {
			iterator_type copy = *this;
			copy += n;
			return copy;
		}

		iterator_type& operator-=(size_t n) {
			if (n > ch_size * ch + pos) {
				throw std::out_of_range("Too much");
			}
			if (pos < n % ch_size) {
				pos = ch_size + pos - (n % ch_size);
				ch -= (1 + (n / ch_size));
				return *this;
			}
			pos -= n % ch_size;
			ch -= n / ch_size;
			return *this;
		}

		iterator_type operator-(size_t n) {
			iterator_type copy = *this;
			copy -= n;
			return copy;
		}

		ssize_t operator-(const base_iterator& it) const {
			if (*this < it) {
				throw std::out_of_range("To biiiiiig");
			}
			return (ch - it.ch) * ch_size + (pos - it.pos);
		}

		reference_type operator*() {
			return it_body[ch][pos];
		}

		pointer_type operator->() {
			return &it_body[ch][pos];
		}

		bool operator<(const base_iterator& it) const {
			bool tmp = (ch <= it.ch);
			if (ch == it.ch) tmp &= (pos < it.pos);
			return tmp;
		}

		bool operator>(const base_iterator& it) const {
			return it < *this;
		}

		bool operator==(const base_iterator& it) const {
			return !(*this < it || *this > it);
		}

		bool operator!=(const base_iterator& it) const {
			return !(*this == it);
		}

		bool operator<=(const base_iterator& it) const {
			return !(*this > it);
		}
		
		bool operator>=(const base_iterator& it) const {
			return !(*this < it);
		}

	};

	

public:

	using iterator = base_iterator<false>;
	using const_iterator = base_iterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	iterator begin() {
		iterator it = iterator(body, begin_index.chunk, begin_index.position);
		return it;
	}

	const_iterator begin() const {
		const_iterator it = const_iterator(body, begin_index.chunk, begin_index.position);
		return it;
	}

	const_iterator cbegin() const {
		const_iterator it = const_iterator(body, begin_index.chunk, begin_index.position);
		return it;
	}

	iterator end() {
		iterator it = iterator(body, end_index.chunk, end_index.position);
		return it;
	}

	const_iterator end() const {
		const_iterator it = const_iterator(body, end_index.chunk, end_index.position);
		return it;
	}

	const_iterator cend() const {
		const_iterator it = const_iterator(body, end_index.chunk, end_index.position);
		return it;
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const {
		return const_reverse_iterator(begin());
	}

	Deque() {
		body = new T*[CP];
		//body = reinterpret_cast<T**>(new uint8_t*[CP * sizeof(T*)]);
		for (size_t i = 0; i < CP; ++i) {
			body[i] = reinterpret_cast<T*>(new uint8_t[ch_size * sizeof(T)]);
		}
		begin_index.chunk = 4;
		end_index.chunk = 4;
		begin_index.position = 0;
		end_index.position = 0;
	}

	Deque(size_t n) {
		while (2*n >= CP * ch_size) {
			CP *= 2;
		}

		body = new T*[CP];
		//body = reinterpret_cast<T**>(new uint8_t*[sizeof(T*) * CP]);
		for (size_t i = 0; i < CP; i++) {
			body[i] = reinterpret_cast<T*>(new uint8_t[ch_size * sizeof(T)]);
		}
		begin_index.chunk = (CP - n) / 2;
		begin_index.position = 0;
		end_index.chunk = ((CP - n) / 2) + n / ch_size;
		end_index.position = n % ch_size;
		SZ = n;
	}

	Deque(size_t n, const T& elem) {
		while (2*n >= CP * ch_size) {
			CP *= 2;
		}
		body = new T*[CP];
		//body = reinterpret_cast<T**>(new uint8_t*[sizeof(T*) * CP]);
		for (size_t i = 0; i < CP; ++i) {
			body[i] = reinterpret_cast<T*>(new uint8_t[ch_size * sizeof(T)]);
		}
		begin_index.chunk = CP / 2;
		begin_index.position = 0;
		end_index.chunk = (CP / 2) + (n / ch_size);
		end_index.position = n % ch_size;
		SZ = n;

		for (size_t _chunk = begin_index.chunk; _chunk <= end_index.chunk; ++_chunk) {
			for (size_t _position = (_chunk == begin_index.chunk ? begin_index.position : 0);
									_position < (_chunk == end_index.chunk ? end_index.position : ch_size);
									++_position) {
				new(*(body + _chunk) + _position) T(elem);
			}
		}
	}
	
	Deque(const Deque<T>& de) : SZ(de.SZ), CP(de.CP) {
		begin_index.chunk = de.begin_index.chunk;
		begin_index.position = de.begin_index.position;
		end_index.chunk = de.end_index.chunk;
		end_index.position = de.end_index.position;

		//body = reinterpret_cast<T**>(new uint8_t*[sizeof(T*) * CP]);
		body = new T*[CP];
		for (size_t _chunk = 0; _chunk < CP; ++_chunk) {
			body[_chunk] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * ch_size]);
		}

		/*for (size_t _chunk = begin_index.chunk; _chunk <= end_index.chunk; ++_chunk) {
			for (size_t _position = (_chunk == begin_index.chunk ? begin_index.position : 0);
									_position < (_chunk == end_index.chunk ? end_index.position : ch_size);
									++_position) {
				new(*(body + _chunk) + _position) T(de.body[_chunk][_position]);
			}
		}*/
		for (size_t i = 0; i < SZ; ++i) {
			new( &((*this)[i]) ) T(de[i]);
		}
	}

	~Deque() {
		for (size_t _chunk = begin_index.chunk; _chunk <= end_index.chunk; _chunk++) {
			for (size_t _position = (_chunk == begin_index.chunk ? begin_index.position : 0);
				_position < (_chunk == end_index.chunk ? end_index.position : ch_size); 
				++_position) {
				(*(body + _chunk) + _position)->~T();
			}
		}

		for (size_t i = 0; i < CP; i++) {
			delete [] reinterpret_cast<uint8_t*>(body[i]);
		}

		//delete [] reinterpret_cast<uint8_t**>(body);
		delete [] body;
	}

	Deque<T>& operator=(const Deque<T>& de) {
		for (size_t i = 0; i < SZ; ++i) {
			(&((*this)[i]))->~T();
		}
		for (size_t i = 0; i < CP; ++i) {
			delete [] reinterpret_cast<uint8_t*>(body[i]);
		}
		delete [] body;

		begin_index.chunk = de.begin_index.chunk;
		begin_index.position = de.begin_index.position;
		end_index.chunk = de.end_index.chunk;
		end_index.position = de.end_index.position;
		CP = de.CP;
		SZ = de.SZ;

		//body = reinterpret_cast<T**>(new uint8_t*[sizeof(T*) * CP]);
		body = new T*[CP];
		for (size_t _chunk = 0; _chunk < CP; ++_chunk) {
			body[_chunk] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * ch_size]);
		}

		/*for (size_t _chunk = begin_index.chunk; _chunk <= end_index.chunk; ++_chunk) {
			for (size_t _position = (_chunk == begin_index.chunk ? begin_index.position : 0);
									_position < (_chunk == end_index.chunk ? end_index.position : ch_size);
									++_position) {
				new(*(body + _chunk) + _position) T(de.body[_chunk][_position]);
			}
		}*/
		for (size_t i = 0; i < SZ; ++i) {
			new( &((*this)[i]) ) T(de[i]);
		}
		return *this;
	}

	T& operator[](const size_t& index) {
		size_t _ch = begin_index.chunk + index / ch_size;
		size_t _pos = begin_index.position + index % ch_size;
		if (_pos >= ch_size) {
			_ch += _pos / ch_size;
			_pos %= ch_size;
		}
		return body[_ch][_pos];
	}

	const T& operator[](const size_t& index) const {
		size_t _ch = begin_index.chunk + index / ch_size;
		size_t _pos = begin_index.position + index % ch_size;
		if (_pos >= ch_size) {
			_ch += _pos / ch_size;
			_pos %= ch_size;
		}
		return body[_ch][_pos];
	}

	T& at(const size_t& index) {
		if (index >= SZ || index < 0) {
			throw std::out_of_range("rrrange");
		}
		return (*this)[index];
	}

	const T& at(const size_t& index) const {
		if (index >= SZ || index < 0) {
			throw std::out_of_range("rrrange");
		}
		return (*this)[index];
	}



	size_t size() const {
		return SZ;
	}

	void push_back(const T& elem) {
		if (end_index.chunk == CP - 1 && end_index.position + 1 == ch_size) {
			this->resize(2*CP);
		}
		new(*(body + end_index.chunk) + end_index.position) T(elem);
		end_index.position += 1;
		if (end_index.position >= ch_size) {
			end_index.chunk += 1;
			end_index.position  = 0;
		}
		++SZ;
	}

	void push_front(const T& elem) {
		if (begin_index.chunk == 0 && begin_index.position == 0) {
			this->resize(2 * CP);
		}
		if (begin_index.position == 0) {
			begin_index.chunk -= 1;
			begin_index.position = ch_size - 1;
			new(*(body + begin_index.chunk) + begin_index.position) T(elem);
			++SZ;
			return;
		}begin_index.position -= 1;
		//body[begin_index.chunk][begin_index.position] = elem;
		new(*(body + begin_index.chunk) + begin_index.position) T(elem);
		++SZ;
	}

	void pop_front() {
		if (SZ == 0) {
			throw std::out_of_range("Deque is empty");
		}
		//T tmp = body[begin_index.chunk][begin_index.position];
		(*(body + begin_index.chunk) + begin_index.position)->~T();
		begin_index.position += 1;
		if (begin_index.position == ch_size) {
			begin_index.chunk += 1;
			begin_index.position = 0;
		}
		--SZ;
		//return tmp;
	}

	void pop_back() {
		if (SZ == 0) {
			throw std::out_of_range("Deque is empty");
		}
		if (end_index.position == 0) {
			end_index.chunk -= 1;
			end_index.position = ch_size - 1;
		}
		else {
			end_index.position -= 1;
		}
		//T tmp = body[end_index.chunk][end_index.position];
		(*(body + end_index.chunk) + end_index.position)->~T();
		--SZ;
		//return tmp;
	}

	void insert(iterator iter, const T& val) {
		if (iter > end() || iter < begin()) {
			throw std::out_of_range("Not in deque\n");
		}

		if (iter == end()) {
			push_back(val);
			return;
		}
		
		Deque<T>::iterator copy_end = end() - 1;
		T copy = *copy_end;

		for (; copy_end > iter; --copy_end) {
			copy_end->~T();
			new(&(*copy_end)) T(*(copy_end - 1));
		}
		copy_end->~T();
		new(&(*copy_end)) T(val);
		push_back(copy);
		//push_back(copy_end);
	}

	void erase(iterator iter) {
		if (iter > end() || iter < begin()) {
			throw std::out_of_range("Not in deque\n");
		}
		for (iterator it = iter; it < this->end() - 1; ++it) {
			it->~T();
			new(&(*it)) T(*(it + 1));
		}
		pop_back();
	}

};
