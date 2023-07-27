//miles.h By Miles C 2020. See README.md, VERSION.md, and miles.cpp

//the class milesString is somewhat based on the code for CStr.h and CStr.cpp by Brian Overland in 1996. This code was excerpted from the book "C++ in Plain English [by Brian Overland, 1996]". The rest of the code is written by myself, and although I fully understand the CStr code and made modifications to it, I cannot take full credit for it.

#ifndef MILES_H
#define MILES_H
class milesString;
class milesArray;
class milesInt;
class milesDecimal;
void milesintarrcpy(int* dest, const int* source, size_t cpylength);
void milesintarrcat(int* dest, const int* source, size_t currentlength, size_t catlength);
void mileschararrcpy(char* dest, const char* source, size_t cpylength);
void mileschararrcat(char* dest, const char* source, size_t currentlength, size_t catlength);
size_t milesstrlen(const char* charstr);

class milesString {
private:
	char *stringdata;
	size_t stringlength;
public:
	milesString();
	milesString(const char *charstr);
	milesString(const milesString &str);
	#ifdef INCLUDE_STRING_CAPABILITIES
	milesString(const std::string& str);
	#endif
	milesString(size_t newlength); // Constructor for an empty string with determined length
	~milesString();

	const char* read(void) const {return stringdata;}
	const char* const* dynamicread(void) const { return &stringdata; }
	char* exportdata(void) const;
	size_t length(void) const {return stringlength;}
	void write(const char *charstr);
	void write(const milesString &str);
	void allocate(size_t newlength);
	void extend(size_t additionallength);
	void concat(const char *charstr);
	void concat(const milesString &str);
	void edit(size_t index, char character) { if (character == '\0') throw "Changing Characters in a milesString to Null terminators is prohibited."; if (index < stringlength) { stringdata[index] = character; return; } else { throw("Index out of range"); } }
	void fill(char character) { if (character == '\0') throw "Changing Characters in a milesString to Null terminators is prohibited."; for (int i = 0; i < stringlength; i++) { stringdata[i] = character; } }
	void remove(size_t index);
	void remove(size_t index, size_t length);
	void insert(size_t index, char character);
	void insert(size_t index, const milesString &str);
	void insert(size_t index, const char* charstr);
	char character(size_t index) { if (index < stringlength) { return stringdata[index]; } else { throw("Index out of range"); } }
	size_t find(const char* charstr, size_t start, size_t end) const;
	size_t find(const milesString& str, size_t start, size_t end) const;
	size_t find(char character, size_t start, size_t end) const;
	size_t find(const char* charstr) const;
	size_t find(const milesString& str) const;
	size_t find(char character) const;
	size_t count(char character) const;
	size_t count(const char* charstr) const;
	size_t count(const milesString& str) const;
	size_t count(char character, size_t start, size_t end) const;
	size_t count(const char* charstr, size_t start, size_t end) const;
	size_t count(const milesString& str, size_t start, size_t end) const;
	bool isnumber(void);
	milesString upper(void);
	milesString lower(void);
	milesString reversed(void);
	milesString substring(size_t start, size_t length);
	long double tonumber(void) const;

	friend milesString operator+(milesString str1, const milesString &str2);
	friend milesString operator+(milesString str, const char *charstr);
	friend milesString operator+(const char *charstr, const milesString &str);
	
	friend bool operator==(milesString str1, milesString str2);
	friend bool operator==(milesString str, const char* charstr);
	friend bool operator==(const char* charstr, milesString str);

	milesString& operator=(const milesString &source){write(source); return *this;}
	milesString& operator=(const char *charstr){write(charstr); return *this;}
	milesString& operator<<(const milesString &source){write(source); return *this;}
	milesString& operator<<(const char *charstr){write(charstr); return *this;}
	char operator[](size_t index) const { if (index < stringlength) { return stringdata[index]; } else { throw("Index out of range"); } }
	char& operator[](size_t index) { if (index < stringlength) { return stringdata[index]; } else { throw("Index out of range"); } }

	milesString operator*(size_t count);

	char* operator>>(char *charstr){mileschararrcpy(charstr, stringdata, stringlength); return charstr;}
};

/*--------------------------------MILESARRAY--------------------------------*/

class milesArray { //ONLY SUPPORTS INTEGERS
private:
	int* arraydata;
	size_t arraylength;
public:
	milesArray();
	milesArray(const int* vect, size_t vectlength);
	milesArray(const milesArray& arr);
	milesArray(const std::initializer_list<int>& list);
	#ifdef INCLUDE_VECTOR_CAPABILITIES
	milesArray(const std::vector<int> &list);
	#endif
	/*milesArray(const std::array<int,N> &list);
	milesArray(const std::list<int> &list); //not working yett!!*/
	milesArray(size_t newlength); // Constructor for an empty array with determined length
	~milesArray();

	const int* read(void) const { return arraydata; }
	const int* const* dynamicread(void) const { return &arraydata; }
	int* exportdata(void) const;
	size_t length(void) const { return arraylength; }
	int product(void) const;
	int product(size_t start, size_t end) const;
	int sum(void) const;
	int sum(size_t start, size_t end) const;
	double mean(void) const { return sum() / (double)length(); }
	double mean(size_t start, size_t end) const { return sum(start, end) / (double)(end-start); }
	double median(void) const;
	double median(size_t start, size_t end) const;
	int min(void) const;
	int min(size_t start, size_t end) const;
	int max(void) const;
	int max(size_t start, size_t end) const;
	int mode(void) const;
	int mode(size_t start, size_t end) const;
	int range(void) const {	return max() - min();}
	int range(size_t start, size_t end) const {return max(start, end) - min(start, end);}
	void add(const milesArray& arr);
	void add(const int* vect, size_t vectlength);
	void add(const std::initializer_list<int> &list);
	void subtract(const milesArray& arr);
	void subtract(const int* vect, size_t vectlength);
	void subtract(const std::initializer_list<int> &list);
	void multiply(const milesArray& arr);
	void multiply(const int* vect, size_t vectlength);
	void multiply(const std::initializer_list<int> &list);
	void divide(const milesArray& arr);
	void divide(const int* vect, size_t vectlength);
	void divide(const std::initializer_list<int> &list);
	void write(const int* vect, size_t newlength);
	void write(const milesArray& arr);
	void write(const std::initializer_list<int> &list);
	void allocate(size_t newlength);
	void extend(size_t additionallength);
	void fill(int element) { for (int i = 0; i < arraylength; i++) { arraydata[i] = element; } }
	void concat(const int* vect, size_t additionallength);
	void concat(const milesArray& arr);
	void edit(size_t index, int element) { if (index < arraylength) {arraydata[index] = element; return; } else { throw("Index out of range"); } }
	void append(int element);
	void swap(size_t index1, size_t index2);
	void remove(size_t index);
	void remove(size_t index, size_t length);
	void insert(size_t index, int element);
	void insert(size_t index, const milesArray &arr);
	void insert(size_t index, const int* vect, size_t vectlength);
	int element(size_t index) { if (index < arraylength) { return arraydata[index]; } else { throw("Index out of range"); } }
	int front(void) const { return arraydata[0]; }
	int& front(void) { return arraydata[0]; }
	int back(void) const { return arraydata[arraylength - 1]; }
	int& back(void) { return arraydata[arraylength - 1]; }
	size_t find(const int* vect, size_t ptrlength) const;
	size_t find(const milesArray& arr) const;
	size_t find(int element) const;
	size_t find(const int* vect, size_t ptrlength, size_t start, size_t end) const;
	size_t find(const milesArray& arr, size_t start, size_t end) const;
	size_t find(int element, size_t start, size_t end) const;
	size_t count(const int* vect, size_t vectlength) const;
	size_t count(const milesArray& arr) const;
	size_t count(int element) const;
	size_t count(const int* vect, size_t vectlength, size_t start, size_t end) const;
	size_t count(const milesArray& arr, size_t start, size_t end) const;
	size_t count(int element, size_t start, size_t end) const;
	milesArray ascending(void) const;
	milesArray descending(void) const;
	milesArray reversed(void) const;
	milesArray shuffled(void) const;
	milesArray shuffled(unsigned long seed) const;
	milesArray subarray(size_t start, size_t length) const;
	#ifdef INCLUDE_MATH_CAPABILITIES
	milesString outputstring(void) const;
	#endif

	friend milesArray operator+(milesArray arr1, const milesArray &arr2);

	friend bool operator==(const milesArray& arr1, const milesArray& arr2);

	milesArray& operator=(const milesArray& arr) { write(arr); return *this; }
	milesArray& operator<<(const milesArray& arr) { write(arr); return *this; }
	int* operator>>(int* vect) const { milesintarrcpy(vect, arraydata, arraylength); return vect; }
	milesArray operator*(size_t count);
	int operator[](size_t index) const { if (index < arraylength) { return arraydata[index]; } else { throw("Index out of range"); } }
	int& operator[](size_t index) { if (index < arraylength) { return arraydata[index]; } else { throw("Index out of range"); } }
};


class milesInt {
private:
	int value;
public:
	//I am using a LOT of passing by const reference. This is just to optimize and circumvent the copying process (even if it is pretty fast for a milesInt)
	//because I am using pass-by-reference in my other objects, some of which are more expensive to copy.
	milesInt() { value = 0; }
	milesInt(int initvalue) { value = initvalue; }
	milesInt(const milesInt& initvalue) { value = initvalue.read(); }
	milesInt(const milesDecimal& initvalue); //not inlined because of circular refrence
	~milesInt() {}
	int read(void) const { return value; }
	#ifdef INCLUDE_MATH_CAPABILITIES
	size_t length(void) const;
	milesString tostring(void) const;
	void writefromstring(const milesString& newvaluestr);
	void writefromstring(const char* charstr);
	#endif
	void write(int newvalue) { value = newvalue; }
	void write(const milesInt& newvalue) { value = newvalue.read(); }

	milesInt& operator=(const milesInt& newvalue) { write(newvalue.read()); return *this; }
	milesInt& operator=(int newvalue) { write(newvalue); return *this; }

	friend milesInt operator+(milesInt val1, milesInt val2);
	friend milesInt operator+(milesInt val1, int val2);
	friend milesInt operator+(int val1, milesInt val2);

	friend milesInt operator-(milesInt val1, milesInt val2);
	friend milesInt operator-(milesInt val1, int val2);
	friend milesInt operator-(int val1, milesInt val2);

	friend milesInt operator*(milesInt val1, milesInt val2);
	friend milesInt operator*(milesInt val1, int val2);
	friend milesInt operator*(int val1, milesInt val2);

	friend milesInt operator/(milesInt val1, milesInt val2);
	friend milesInt operator/(milesInt val1, int val2);
	friend milesInt operator/(int val1, milesInt val2);

	friend milesInt operator%(milesInt val1, milesInt val2);
	friend milesInt operator%(milesInt val1, int val2);
	friend milesInt operator%(int val1, milesInt val2);

	#ifdef INCLUDE_MATH_CAPABILITIES
	friend milesInt operator^(milesInt val1, milesInt val2);
	friend milesInt operator^(milesInt val1, int val2);
	friend milesInt operator^(int val1, milesInt val2);
	#endif

	friend bool operator<(milesInt val1, milesInt val2);
	friend bool operator<(milesInt val1, int val2);
	friend bool operator<(int val1, milesInt val2);

	friend bool operator>(milesInt val1, milesInt val2);
	friend bool operator>(milesInt val1, int val2);
	friend bool operator>(int val1, milesInt val2);

	friend bool operator<=(milesInt val1, milesInt val2);
	friend bool operator<=(milesInt val1, int val2);
	friend bool operator<=(int val1, milesInt val2);

	friend bool operator>=(milesInt val1, milesInt val2);
	friend bool operator>=(milesInt val1, int val2);
	friend bool operator>=(int val1, milesInt val2);

	friend bool operator==(milesInt val1, milesInt val2);
	friend bool operator==(milesInt val1, int val2);
	friend bool operator==(int val1, milesInt val2);

	friend bool operator!=(milesInt val1, milesInt val2);
	friend bool operator!=(milesInt val1, int val2);
	friend bool operator!=(int val1, milesInt val2);

	milesInt operator++() { return ++value; }       // Prefix increment operator.
	milesInt operator++(int) { return value++; }    // Postfix increment operator.
	milesInt operator--() { return --value; }       // Prefix decrement operator.
	milesInt operator--(int) { return value--; }     // Postfix decrement operator.

	milesInt operator+=(milesInt val) { value += val.read(); return *this;}
	milesInt operator+=(int val) { value += val; return * this;}

	milesInt operator-=(milesInt val) { value -= val.read(); return *this;}
	milesInt operator-=(int val) { value -= val; return *this;}

	milesInt operator*=(milesInt val) { value *= val.read(); return *this;}
	milesInt operator*=(int val) { value *= val; return *this;}

	milesInt operator/=(milesInt val) { value /= val.read(); return *this;}
	milesInt operator/=(int val) { value /= val; return *this;}

	milesInt operator%=(milesInt val) { value %= val.read(); return *this;}
	milesInt operator%=(int val) { value %= val; return *this;}

	#ifdef INCLUDE_MATH_CAPABILITIES
	milesInt operator^=(milesInt val) { value = pow(value, val.read()); return * this;}
	milesInt operator^=(int val) { value = pow(value, val); return *this;}
	#endif
};

class milesDecimal { //milesDecimal is a long double
private:
	long double value;
public:
	milesDecimal() { value = 0; }
	milesDecimal(long double initvalue) { value = initvalue; }
	milesDecimal(const milesDecimal& initvalue) { value = initvalue.read(); }
	milesDecimal(const milesInt& initvalue); //not inlined because of circular refrence
	~milesDecimal() {}
	long double read(void) const { return value; }
	#ifdef INCLUDE_MATH_CAPABILITIES
	size_t length(void) const;
	size_t length(size_t decimals) const;
	milesString tostring(void) const;
	milesString tostring(size_t decimals) const;
	void writefromstring(const milesString& newvaluestr);
	void writefromstring(const char* charstr);
	#endif
	void write(long double newvalue) { value = newvalue; }
	void write(const milesDecimal& newvalue) { value = newvalue.read(); }

	milesDecimal& operator=(const milesDecimal& newvalue) { write(newvalue.read()); return *this; }
	milesDecimal& operator=(long double newvalue) { write(newvalue); return *this; }

	friend milesDecimal operator+(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator+(milesDecimal val1, long double val2);
	friend milesDecimal operator+(long double val1, milesDecimal val2);

	friend milesDecimal operator-(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator-(milesDecimal val1, long double val2);
	friend milesDecimal operator-(long double val1, milesDecimal val2);

	friend milesDecimal operator*(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator*(milesDecimal val1, long double val2);
	friend milesDecimal operator*(long double val1, milesDecimal val2);

	friend milesDecimal operator/(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator/(milesDecimal val1, long double val2);
	friend milesDecimal operator/(long double val1, milesDecimal val2);

	#ifdef INCLUDE_MATH_CAPABILITIES
	friend milesDecimal operator%(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator%(milesDecimal val1, long double val2);
	friend milesDecimal operator%(long double val1, milesDecimal val2);

	friend milesDecimal operator^(milesDecimal val1, milesDecimal val2);
	friend milesDecimal operator^(milesDecimal val1, long double val2);
	friend milesDecimal operator^(long double val1, milesDecimal val2);
	#endif

	friend bool operator<(milesDecimal val1, milesDecimal val2);
	friend bool operator<(milesDecimal val1, long double val2);
	friend bool operator<(long double val1, milesDecimal val2);

	friend bool operator>(milesDecimal val1, milesDecimal val2);
	friend bool operator>(milesDecimal val1, long double val2);
	friend bool operator>(long double val1, milesDecimal val2);

	friend bool operator<=(milesDecimal val1, milesDecimal val2);
	friend bool operator<=(milesDecimal val1, long double val2);
	friend bool operator<=(long double val1, milesDecimal val2);

	friend bool operator>=(milesDecimal val1, milesDecimal val2);
	friend bool operator>=(milesDecimal val1, long double val2);
	friend bool operator>=(long double val1, milesDecimal val2);

	friend bool operator==(milesDecimal val1, milesDecimal val2);
	friend bool operator==(milesDecimal val1, long double val2);
	friend bool operator==(long double val1, milesDecimal val2);

	friend bool operator!=(milesDecimal val1, milesDecimal val2);
	friend bool operator!=(milesDecimal val1, long double val2);
	friend bool operator!=(long double val1, milesDecimal val2);

	milesDecimal operator++() { return ++value; }       // Prefix increment operator.
	milesDecimal operator++(int) { return value++; }    // Postfix increment operator.
	milesDecimal operator--() { return --value; }       // Prefix decrement operator.
	milesDecimal operator--(int) { return value--; }     // Postfix decrement operator.

	milesDecimal operator+=(milesDecimal val) { value += val.read(); return *this; }
	milesDecimal operator+=(long double val) { value += val; return *this; }

	milesDecimal operator-=(milesDecimal val) { value -= val.read(); return *this;}
	milesDecimal operator-=(long double val) { value -= val; return *this;}

	milesDecimal operator*=(milesDecimal val) { value *= val.read(); return *this;}
	milesDecimal operator*=(long double val) { value *= val; return *this;}

	milesDecimal operator/=(milesDecimal val) { value /= val.read(); return *this;}
	milesDecimal operator/=(long double val) { value /= val; return *this;}

	#ifdef INCLUDE_MATH_CAPABILITIES
	milesDecimal operator%=(milesDecimal val) { value = fmod(value,val.read()); return *this;}
	milesDecimal operator%=(long double val) { value = fmod(value,val); return *this;}

	milesDecimal operator^=(milesDecimal val) { value = pow(value, val.read()); return *this;}
	milesDecimal operator^=(long double val) { value = pow(value, val); return *this;}
	#endif
};
#endif