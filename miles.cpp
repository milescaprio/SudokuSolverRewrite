//miles.cpp By Miles C 2021. See README.md and miles.h

//the class milesString is mainly based on the code for CStr.h and CStr.cpp by Brian Overland in 1996. This code was excerpted from the book "C++ in Plain English [by Brian Overland, 1996]". The rest of the code is written by myself, and although I fully understand the CStr code and made modifications to it, I cannot take full credit for it.

#define INCLUDE_VECTOR_CAPABILITIES //comment out this line if you want to save storage to disable including std::vector and its abilities
#define INCLUDE_STRING_CAPABILITIES //comment out this line if you want to save storage to disable including std::string and its abilities
#define INCLUDE_MATH_CAPABILITIES //comment out this line if you want to save storage to disable including math.h and all functions which use it (tostring, tonumber, ^, milesDecimal %, outputstring, milesInt and milesDecimal length)

#include <initializer_list>
#ifdef INCLUDE_STRING_CAPABILITES
#include <string.h>
#endif
#ifdef INCLUDE_VECTOR_CAPABILITIES
#include <vector>
#endif
#ifdef INCLUDE_MATH_CAPABILITIES
#include <math.h>
#endif
#include <malloc.h>
#include "miles.h"
/*--------------------------------MILESSTRING--------------------------------*/
milesString::milesString() {
	stringdata = (char*)malloc(1);
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	*stringdata = '\0';
	stringlength = 0; //the rule for the miles library is that local length MUST be changed AFTER malloc exception handling; so if malloc fails and the error is caught by the user (not recommended) the old length will be retained, however the data pointer will save a null pointer.
}

milesString::milesString(const char* charstr) {
	size_t newlen = milesstrlen(charstr);
	stringdata = (char*)malloc(newlen + 1); //i added one
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdata, charstr, newlen);
	stringlength = newlen;
}

milesString::milesString(const milesString& str) {
	stringdata = (char*)malloc(str.stringlength + 1);//i added one
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdata, str.stringdata, str.stringlength);
	stringlength = str.stringlength;
}

#ifdef INCLUDE_STRING_CAPABILITIES
milesString::milesString(const std::string& str) {
	size_t newlen = str.length();
	stringdata = (char*)malloc(newlen + 1);
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdata, str.c_str(), newlen);
	stringlength = newlen;
}
#endif

milesString::milesString(size_t initlength) { //CONSTRUCTOR TO INITIALIZE A BLANK EDITABLE STRING WITH CERTAIN LENGTH
	stringdata = (char*)malloc(initlength + 1);
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	stringdata[initlength] = '\0';
	stringlength = initlength;
}

milesString::~milesString() {
	free(stringdata);
}

void milesString::write(const char* charstr) {
	size_t newlength;
	newlength = milesstrlen(charstr);
	if (stringlength != newlength) {
		if (stringdata)
			free(stringdata);
		stringdata = (char*)malloc(newlength + 1);
		if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
		stringlength = newlength;
	}
	mileschararrcpy(stringdata, charstr, newlength);
}

void milesString::write(const milesString& str) {
	if (stringlength != str.stringlength) {
		if (stringdata)
			free(stringdata);
		stringdata = (char*)malloc(str.stringlength + 1);
		if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
		stringlength = str.stringlength;
	}
	mileschararrcpy(stringdata, str.stringdata, str.stringlength);
}

void milesString::allocate(size_t newlength) {
	free(stringdata);
	stringdata = (char*)malloc(newlength + 1);
	if (!stringdata) throw "Error: Insufficient Memory for Allocation Request";
	stringdata[newlength] = '\0';
	stringlength = newlength;
}

void milesString::extend(size_t additionallength) {
	char* stringdatatemp;
	if (additionallength == 0)
		return;
	stringdatatemp = (char*)malloc((stringlength + additionallength + 1));
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (stringdata) {
		mileschararrcpy(stringdatatemp, stringdata, stringlength);
		free(stringdata);
	}
	stringdatatemp[stringlength] = ' '; //erase null terminator
	stringdatatemp[stringlength + additionallength] = '\0';
	stringdata = stringdatatemp;
	stringlength += additionallength;
}

char* milesString::exportdata(void) const {
	char* savelocation = (char*)malloc(stringlength + 1);
	if (!savelocation) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(savelocation, stringdata, stringlength);
	return savelocation;
}

/*void milesString::destroy(void) {
	free(stringdata);
} we shouldnt use this code, as the object will still remain. People can use new and delete to destroy the miles objects.*/

void milesString::concat(const char* charstr) {
	size_t additionallength = milesstrlen(charstr);
	char* stringdatatemp;
	if (additionallength == 0)
		return;
	stringdatatemp = (char*)malloc(stringlength + additionallength + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (stringdata) {
		mileschararrcpy(stringdatatemp, stringdata, stringlength);
		free(stringdata);
	}
	mileschararrcat(stringdatatemp, charstr, stringlength, additionallength);
	stringdata = stringdatatemp;
	stringlength += additionallength;
}

void milesString::concat(const milesString& str) {
	char* stringdatatemp;
	if (str.stringlength == 0)
		return;
	stringdatatemp = (char*)malloc(stringlength + str.stringlength + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (stringdata) {
		mileschararrcpy(stringdatatemp, stringdata, stringlength);
		free(stringdata);
	}
	mileschararrcat(stringdatatemp, str.stringdata, stringlength, str.stringlength);
	stringdata = stringdatatemp;
	stringlength += str.stringlength;
}

bool milesString::isnumber() {
	size_t decimalpointcount = 0;
	bool goodchars = true;
	if (!((stringdata[0] >= '0' && stringdata[0] <= '9') || stringdata[0] == '-' || stringdata[0] == '.')) {
		goodchars = false;
	}
	for (size_t i = 1; i < stringlength; i++) {
		if (!((stringdata[i] >= '0' && stringdata[i] <= '9') || stringdata[i] == '.')) {
			goodchars = false;
			break;
		}
		if (stringdata[i] == '.') {
			decimalpointcount++;
		}
	}
	if (decimalpointcount > 1 || goodchars == false) {
		return false;
	}
	else {
		return true;
	}
}

long double milesString::tonumber() const {
	size_t decimallocation = find('.');
	if (decimallocation == stringlength) {
		decimallocation = stringlength;
	}
	long double number = 0;
	for (size_t i = 0; i < decimallocation; i++) {
		if (stringdata[i] >= '0' && stringdata[i] <= '9') {
			number += (stringdata[i] - '0') * pow(10, (decimallocation - i - 1));
		}
		else {
			if (!(i == 0 && stringdata[i] == '-')) {
				throw "The string being requested to convert to a number is not eligible to do so, as the format is incorrect.";
				return -1;
			}
		}
	}
	for (size_t i = decimallocation + 1; i < stringlength; i++) {
		if (stringdata[i] >= '0' && stringdata[i] <= '9') {
			number += (stringdata[i] - '0') * pow(10, (decimallocation - i));
		}
		else {
			if (!(i == 0 && stringdata[i] == '-')) {
				throw "The string being requested to convert to a number is not eligible to do so, as the format is incorrect.";
				return -1;
			}
		}
	}
	if (stringdata[0] == '-') {
		number *= -1;
	}
	return number;
}

size_t milesString::find(const char* charstr, size_t start, size_t end) const {
	size_t ptrlength = milesstrlen(charstr);
	size_t found = end;
	bool flag;
	for (size_t i = start; i < end - ptrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < ptrlength; k++) {
			if (charstr[k] != stringdata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::find(const milesString& str, size_t start, size_t end) const {
	size_t found = end;
	bool flag;
	for (size_t i = start; i < end - str.stringlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < str.stringlength; k++) {
			if (str.stringdata[k] != stringdata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::find(char character, size_t start, size_t end) const {
	size_t found = end;
	for (size_t i = start; i < end; i++) {
		if (character == stringdata[i]) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::find(const char* charstr) const {
	size_t ptrlength = milesstrlen(charstr);
	size_t found = stringlength;
	bool flag;
	for (size_t i = 0; i < stringlength - ptrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < ptrlength; k++) {
			if (charstr[k] != stringdata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::find(const milesString& str) const {
	size_t found = stringlength;
	bool flag;
	for (size_t i = 0; i < stringlength - str.stringlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < str.stringlength; k++) {
			if (str.stringdata[k] != stringdata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::find(char character) const {
	size_t found = stringlength;
	for (size_t i = 0; i < stringlength; i++) {
		if (character == stringdata[i]) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesString::count(char character) const {
	size_t found = 0;
	for (int i = 0; i < stringlength; i++) {
		if (stringdata[i] == character) {
			found++;
		}
	}
	return found;
}

size_t milesString::count(const milesString& str) const {
	size_t found = 0;
	bool flag;
	for (size_t i = 0; i < stringlength - str.stringlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < str.stringlength; k++) {
			if (stringdata[i + k] != str.stringdata[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesString::count(const char* charstr) const {
	size_t charstrlength = milesstrlen(charstr);
	size_t found = 0;
	bool flag;
	for (size_t i = 0; i < stringlength - charstrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < charstrlength; k++) {
			if (stringdata[i + k] != charstr[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesString::count(char character, size_t start, size_t end) const {
	if (end > stringlength) throw "Index out of range";
	size_t found = 0;
	for (size_t i = start; i < end; i++) {
		if (stringdata[i] == character) {
			found++;
		}
	}
	return found;
}

size_t milesString::count(const milesString& str, size_t start, size_t end) const {
	if (end > stringlength) throw "Index out of range";
	size_t found = 0;
	bool flag;
	for (size_t i = start; i < end - str.stringlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < str.stringlength; k++) {
			if (stringdata[i + k] != str.stringdata[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesString::count(const char* charstr, size_t start, size_t end) const {
	size_t charstrlength = milesstrlen(charstr);
	if (end > stringlength) throw "Index out of range";
	size_t found = 0;
	bool flag;
	for (size_t i = start; i < end - charstrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < charstrlength; k++) {
			if (stringdata[i + k] != charstr[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

milesString milesString::upper() {
	milesString str(*this);
	size_t strlength = str.stringlength;
	for (size_t strindex = 0; strindex < strlength; strindex++) {
		char character = str.stringdata[strindex];
		if (character <= 'z' && character >= 'a') {
			str.stringdata[strindex] -= 32;
		}
	}
	return str;
}

milesString milesString::lower() {
	milesString str(*this);
	size_t strlength = str.stringlength;
	for (size_t strindex = 0; strindex < strlength; strindex++) {
		char character = str.stringdata[strindex];
		if (character <= 'Z' && character >= 'A') {
			str.stringdata[strindex] += 32;
		}
	}
	return str;
}

milesString milesString::reversed() {
	milesString reversedstr(stringlength);
	for (size_t i = 0; i < stringlength; i++) {
		reversedstr.stringdata[i] = stringdata[stringlength - i - 1];
	}
	return reversedstr;
}

milesString milesString::substring(size_t start, size_t length) {
	if (start + length > stringlength) {
		throw "The requested substring is longer than the string itself.";
	}
	milesString substr(length);
	for (size_t i = 0; i < length; i++) {
		substr.stringdata[i] = stringdata[i + start];
	}
	return substr;
}

void milesString::insert(size_t index, char character) {
	if (index >= stringlength) { throw "Index out of range"; }
	if (character == '\0') throw "Changing Characters in a milesString to Null terminators is prohibited.";
	char* stringdatatemp = (char*)malloc((stringlength + 1) + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdatatemp, stringdata, index);
	stringdatatemp[index] = character;
	mileschararrcat(stringdatatemp, stringdata + index, index + 1, stringlength - index);
	free(stringdata);
	stringdata = stringdatatemp;
	stringlength++;
}

void milesString::insert(size_t index, const milesString &str) {
	if (index >= stringlength) { throw "Index out of range"; }
	char* stringdatatemp = (char*)malloc((stringlength + str.stringlength) + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdatatemp, stringdata, index);
	mileschararrcat(stringdatatemp, str.stringdata, index, str.stringlength);
	mileschararrcat(stringdatatemp, stringdata + index, index + str.stringlength, stringlength - index);
	free(stringdata);
	stringdata = stringdatatemp;
	stringlength += str.stringlength;
}

void milesString::insert(size_t index, const char* charstr) {
	size_t charstrlen = milesstrlen(charstr);
	if (index >= stringlength) { throw "Index out of range"; }
	char* stringdatatemp = (char*)malloc((stringlength + charstrlen) + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdatatemp, stringdata, index);
	mileschararrcat(stringdatatemp, charstr, index, charstrlen);
	mileschararrcat(stringdatatemp, stringdata + index, index + charstrlen, stringlength - index);
	free(stringdata);
	stringdata = stringdatatemp;
	stringlength += charstrlen;
}

void milesString::remove(size_t index) {
	if (stringlength == 0) { throw "Element removal count exceeds length of array"; }
	if (index + 1 > stringlength) { throw "Index out of range"; }
	char* stringdatatemp = (char*)malloc((stringlength - 1) + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdatatemp, stringdata, index);
	mileschararrcat(stringdatatemp, stringdata + index + 1, index, stringlength - index - 1);
	free(stringdata);
	stringdata = stringdatatemp;
	stringlength--;
}

void milesString::remove(size_t index, size_t length) {
	if (index + length > stringlength) { throw "Given index and length exceeds array boundary"; }
	if (index >= stringlength) { throw "Index out of range"; }
	char* stringdatatemp = (char*)malloc((stringlength - length) + 1);
	if (!stringdatatemp) throw "Error: Insufficient Memory for Allocation Request";
	mileschararrcpy(stringdatatemp, stringdata, index);
	mileschararrcat(stringdatatemp, stringdata + index + length, index, stringlength - index - length);
	free(stringdata);
	stringdata = stringdatatemp;
	stringlength -= length;
}

milesString operator+(milesString str1, const milesString &str2) {
	str1.concat(str2);
	return str1;
}

milesString operator+(milesString str, const char* charstr) {
	str.concat(charstr);
	return str;
}

milesString operator+(const char* charstr, const milesString &str) {
	milesString strtemp = milesstrlen(charstr) + str.stringlength;
	for (size_t i = 0; i < strtemp.stringlength - str.stringlength; i++) {
		strtemp.stringdata[i] = charstr[i];
	}
	for (size_t i = 0; i < str.stringlength; i++) {
		strtemp.stringdata[i + strtemp.stringlength - str.stringlength] = str.stringdata[i];
	}
	return strtemp;
}

bool operator==(milesString str1, milesString str2) {
	bool flag = 1;
	if (str1.stringlength == str2.stringlength) {
		for (size_t i = 0; i < str1.stringlength; i++) {
			if (str1.stringdata[i] != str2.stringdata[i]) {
				flag = 0;
			}
		} 
		return flag;
	}
	else {
		return 0;
	}
}

bool operator==(milesString str, const char* charstr) {
	bool flag = 1;
	if (str.stringlength == milesstrlen(charstr)) {
		for (size_t i = 0; i < str.stringlength; i++) {
			if (str.stringdata[i] != charstr[i]) {
				flag = 0;
			}
		}
		return flag;
	}
	else {
		return 0;
	}
}

bool operator==(const char* charstr, milesString str) {
	bool flag = 1;
	if (str.stringlength == milesstrlen(charstr)) {
		for (size_t i = 0; i < str.stringlength; i++) {
			if (str.stringdata[i] != charstr[i]) {
				flag = 0;
			}
		}
		return flag;
	}
	else {
		return 0;
	}
}

milesString milesString::operator*(size_t count) {
	milesString multiplied = stringlength * count; //compressed coded allocate to initialization constructor
	for (size_t i = 0; i < stringlength * count; i++) {
		multiplied.stringdata[i] = stringdata[i % stringlength];
	}
	return multiplied;
}

/*--------------------------------MILESINTARRAY--------------------------------*/
//CONSTRUCTORS ETC:

milesArray::milesArray() { //initializes to {0}
	arraydata = (int*)malloc(sizeof(int));
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = 1;
	arraydata[0] = 0;
}

milesArray::milesArray(const int* vect, size_t vectlength) {
	arraydata = (int*)malloc(sizeof(int)*vectlength);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = vectlength;
	milesintarrcpy(arraydata, vect, arraylength);
}

milesArray::milesArray(const milesArray& arr) {
	arraydata = (int*)malloc(arr.arraylength*sizeof(int));
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = arr.arraylength;
	milesintarrcpy(arraydata, arr.arraydata, arraylength);
}

milesArray::milesArray(const std::initializer_list<int> &list) {
	size_t newlen = list.size();
	arraydata = (int*)malloc(sizeof(int) * newlen);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = newlen;
	size_t count = 0;
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[count] = list.begin()[i];
		count++;
	}
}

#ifdef INCLUDE_VECTOR_CAPABILITIES
milesArray::milesArray(const std::vector<int>& list) {
	size_t newlen = list.size();
	arraydata = (int*)malloc(sizeof(int) * newlen);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = newlen;
	size_t count = 0;
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[count] = list[i];
		count++;
	}
}
#endif

/*milesArray::milesArray(const std::array<int,N>& list) {
	arraylength = N;
	arraydata = (int*)malloc(sizeof(int) * arraylength);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	int count = 0;
	for (int i = 0; i < arraylength; i++) {
		arraydata[count] = list[i];
		count++;
	}
}

milesArray::milesArray(const std::list<int>& list) {
	arraylength = list.size();
	arraydata = (int*)malloc(sizeof(int) * arraylength);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	int count = 0;
	for (int i = 0; i < arraylength; i++) {
		arraydata[count] = list.begin()[i];
		count++;
	}
}//not working yet!!!!! if it does will add ifdef's*/

milesArray::milesArray(size_t newlength) { //Constructor for an empty array with predetermined length. Contains free memory, not 0's, so use fill(0) afterward if you aren't going to immediately overwrite.
	arraydata = (int*)malloc(sizeof(int)*newlength);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = newlength;
}

milesArray::~milesArray() {
	free(arraydata);
}

//MEMBER FUNCTIONS:

int* milesArray::exportdata(void) const {
	int* savelocation = (int*)malloc(sizeof(int) * arraylength);
	if (!savelocation) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(savelocation, arraydata, arraylength);
	return savelocation;
}

void milesArray::write(const int* vect, size_t newlength) {
	if (arraylength != newlength) {
		if (arraydata)
			free(arraydata);
		arraydata = (int*)malloc(newlength*sizeof(int));
		if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
		arraylength = newlength;
	}
	milesintarrcpy(arraydata, vect, newlength);
}

void milesArray::write(const milesArray& arr) {
	if (arraylength != arr.arraylength) {
		if (arraydata)
			free(arraydata);
		arraydata = (int*)malloc(arr.arraylength *sizeof(int));
		if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
		arraylength = arr.arraylength;
	}
	milesintarrcpy(arraydata, arr.arraydata, arr.arraylength);
}

void milesArray::write(const std::initializer_list<int> &list) {
	size_t cpylen = list.size();
	if (arraylength != cpylen) {
		if (arraydata)
			free(arraydata);
		arraydata = (int*)malloc(cpylen * sizeof(int));
		if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
		arraylength = cpylen;
	}
	milesintarrcpy(arraydata, list.begin(), cpylen);
}

void milesArray::allocate(size_t newlength) {
	free(arraydata);
	arraydata = (int*)malloc(sizeof(int) * newlength);
	if (!arraydata) throw "Error: Insufficient Memory for Allocation Request";
	arraylength = newlength;
}

void milesArray::extend(size_t additionallength) {
	int* arraydatatemp;
	if (additionallength == 0)
		return;
	arraydatatemp = (int*)malloc((arraylength + additionallength) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (arraydata) {
		milesintarrcpy(arraydatatemp, arraydata, arraylength);
		free(arraydata);
	}
	arraydata = arraydatatemp;
	arraylength += additionallength;
}

/*void milesArray::destroy(void) {
	free(arraydata);
} we shouldnt use this code, as the object will still remain. People can use new and delete to destroy the miles objects.*/

void milesArray::concat(const int* vect, size_t additionallength) {
	int* arraydatatemp;
	if (additionallength == 0)
		return;
	arraydatatemp = (int*)malloc((arraylength + additionallength) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (arraydata) {
		milesintarrcpy(arraydatatemp, arraydata, arraylength);
		free(arraydata);
	}
	milesintarrcat(arraydatatemp, vect, arraylength, additionallength);
	arraydata = arraydatatemp;
	arraylength += additionallength;
}

void milesArray::concat(const milesArray &arr) {
	size_t additionallength;
	int* arraydatatemp;
	additionallength = arr.arraylength;
	if (additionallength == 0)
		return;
	arraydatatemp = (int*)malloc((arraylength + additionallength) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (arraydata) {
		milesintarrcpy(arraydatatemp, arraydata, arraylength);
		free(arraydata);
	}
	milesintarrcat(arraydatatemp, arr.arraydata, arraylength, additionallength);
	arraydata = arraydatatemp;
	arraylength += additionallength;
}

void milesArray::append(int element) {
	int* arraydatatemp = (int*)malloc((arraylength + 1) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	if (arraydata) {
		milesintarrcpy(arraydatatemp, arraydata, arraylength);
		free(arraydata);
	}
	arraydatatemp[arraylength/* - 1 + 1*/] = element;
	arraydata = arraydatatemp;
	arraylength++;
}

void milesArray::swap(size_t index1, size_t index2) {
	if (index1 <= arraylength && index2 <= arraylength) {
		int temp = arraydata[index1];
		arraydata[index1] = arraydata[index2];
		arraydata[index2] = temp;
	}
}

void milesArray::insert(size_t index, int element) {
	if (index >= arraylength) { throw "Index out of range"; }
	int* arraydatatemp = (int*)malloc((arraylength + 1) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(arraydatatemp, arraydata, index);
	arraydatatemp[index] = element;
	milesintarrcat(arraydatatemp, arraydata + index, index + 1, arraylength - index);
	free(arraydata);
	arraydata = arraydatatemp;
	arraylength++;
}

void milesArray::insert(size_t index, const milesArray &arr) {
	if (index >= arraylength) { throw "Index out of range"; }
	int* arraydatatemp = (int*)malloc((arraylength+arr.arraylength) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(arraydatatemp, arraydata, index);
	milesintarrcat(arraydatatemp, arr.arraydata, index, arr.arraylength);
	milesintarrcat(arraydatatemp, arraydata + index, index + arr.arraylength, arraylength - index);
	free(arraydata);
	arraydata = arraydatatemp;
	arraylength+=arr.arraylength;
}

void milesArray::insert(size_t index, const int* vect, size_t vectlength) {
	if (index >= arraylength) { throw "Index out of range"; }
	int* arraydatatemp = (int*)malloc((arraylength + vectlength) * sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(arraydatatemp, arraydata, index);
	milesintarrcat(arraydatatemp, vect, index, vectlength);
	milesintarrcat(arraydatatemp, arraydata + index, index + vectlength, arraylength - index);
	free(arraydata);
	arraydata = arraydatatemp;
	arraylength += vectlength;
}

void milesArray::remove(size_t index) {
	if (arraylength == 0) { throw "Element removal count exceeds length of array"; }
	if (index + 1 > arraylength) { throw "Index out of range"; }
	int* arraydatatemp = (int*)malloc((arraylength - 1)*sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(arraydatatemp, arraydata, index);
	milesintarrcat(arraydatatemp, arraydata + index + 1, index, arraylength - index - 1);
	free(arraydata);
	arraydata = arraydatatemp;
	arraylength--;
}

void milesArray::remove(size_t index, size_t length) {
	//if (arraylength < length) { throw "Element removal count exceeds length of array"; return; }
	if (index + length > arraylength) { throw "Given index and length exceeds array boundary"; }
	if (index >= arraylength) { throw "Index out of range";}
	int* arraydatatemp = (int*)malloc((arraylength - length)*sizeof(int));
	if (!arraydatatemp) throw "Error: Insufficient Memory for Allocation Request";
	milesintarrcpy(arraydatatemp, arraydata, index);
	milesintarrcat(arraydatatemp, arraydata + index + length, index, arraylength - index - length);
	free(arraydata);
	arraydata = arraydatatemp;
	arraylength-=length;
}

milesArray milesArray::ascending(void) const {
	milesArray ascended = *this;
	for (size_t i = 1; i < arraylength; i++) {
		size_t j = i;
		while (j > 0 && ascended.arraydata[j - 1] > ascended.arraydata[j]) {
			ascended.swap(j, j - 1);
			j--;
		}
	}
	return ascended;
}

milesArray milesArray::descending(void) const {
	milesArray descended = *this;
	for (size_t i = 1; i < arraylength; i++) {
		size_t j = i;
		while (j > 0 && descended.arraydata[j - 1] < descended.arraydata[j]) {
			descended.swap(j, j - 1);
			j--;
		}
	}
	return descended;
}

size_t milesArray::find(const int* vect, size_t ptrlength) const {
	size_t found = arraylength;
	bool flag;
	for (size_t i = 0; i < arraylength - ptrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < ptrlength; k++) {
			if (vect[k] != arraydata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::find(const milesArray& arr) const {
	size_t found = arraylength;
	bool flag;
	for (size_t i = 0; i < arraylength - arr.arraylength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < arr.length(); k++) {
			if (arr.arraydata[k] != arraydata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::find(int element) const {
	size_t found = arraylength;
	for (size_t i = 0; i < arraylength; i++) {
		if (element == arraydata[i]) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::find(const int* vect, size_t ptrlength, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = arraylength;
	bool flag;
	for (size_t i = start; i < end - ptrlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < ptrlength; k++) {
			if (vect[k] != arraydata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::find(const milesArray& arr, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = arraylength;
	bool flag;
	for (size_t i = start; i < end - arr.arraylength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < arr.arraylength; k++) {
			if (arr.arraydata[k] != arraydata[i + k]) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::find(int element, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = arraylength;
	for (size_t i = start; i < end; i++) {
		if (element == arraydata[i]) {
			found = i;
			break;
		}
	}
	return found;
}

size_t milesArray::count(int element) const {
	size_t found = 0;
	for (size_t i = 0; i < arraylength; i++) {
		if (arraydata[i] == element) {
			found++;
		}
	}
	return found;
}

size_t milesArray::count(const milesArray& arr) const {
	size_t found = 0;
	bool flag;
	for (size_t i = 0; i < arraylength - arr.arraylength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < arr.arraylength; k++) {
			if (arraydata[i + k] != arr.arraydata[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesArray::count(const int* vect, size_t vectlength) const {
	size_t found = 0;
	bool flag;
	for (size_t i = 0; i < arraylength - vectlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < vectlength; k++) {
			if (arraydata[i + k] != vect[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesArray::count(int element, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = 0;
	for (size_t i = start; i < end; i++) {
		if (arraydata[i] == element) {
			found++;
		}
	}
	return found;
}

size_t milesArray::count(const milesArray& arr, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = 0;
	bool flag;
	for (size_t i = start; i < end - arr.arraylength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < arr.arraylength; k++) {
			if (arraydata[i + k] != arr.arraydata[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

size_t milesArray::count(const int* vect, size_t vectlength, size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	size_t found = 0;
	bool flag;
	for (size_t i = start; i < end - vectlength + 1; i++) {
		flag = 1;
		for (size_t k = 0; k < vectlength; k++) {
			if (arraydata[i + k] != vect[k]) {
				flag = 0;
				break;
			}
		}
		if (flag) found++;
	}
	return found;
}

milesArray milesArray::reversed(void) const {
	milesArray reversedarr = arraylength;
	for (size_t i = 0; i < arraylength; i++) {
		reversedarr.arraydata[i] = arraydata[arraylength - i - 1];
	}
	return reversedarr;
}

milesArray milesArray::shuffled(void) const {
	size_t n = arraylength;
	milesArray arr1 = n;
	int* index_arr = (int*)malloc(sizeof(int) * n);
	if (!index_arr) throw "Error: Insufficient Memory for Allocation Request";
	size_t index;
	for (size_t i = 0; i < n; i++)
		index_arr[i] = 0;
	for (size_t i = 0; i < n; i++) {
		do {
			index = rand() % n;
		} while (index_arr[index] != 0);
		index_arr[index] = 1;
		arr1.arraydata[i] = arraydata[index];
	}
	return arr1;
}

milesArray milesArray::shuffled(unsigned long seed) const {
	srand(seed);
	size_t n = arraylength;
	milesArray arr1 = n;
	int* index_arr = (int*)malloc(sizeof(int) * n);
	if (!index_arr) throw "Error: Insufficient Memory for Allocation Request";
	size_t index;
	for (size_t i = 0; i < n; i++)
		index_arr[i] = 0;
	for (size_t i = 0; i < n; i++) {
		do {
			index = rand() % n;
		} while (index_arr[index] != 0);
		index_arr[index] = 1;
		arr1.arraydata[i] = arraydata[index];
	}
	return arr1;
}

milesArray milesArray::subarray(size_t start, size_t length) const {
	if (start + length > arraylength) {
		throw "The requested subarray is longer than the array itself.";
	}
	milesArray subarr = length;
	for (size_t i = 0; i < length; i++) {
		subarr.arraydata[i] = arraydata[i + start];
	}
	return subarr;
}

#ifdef INCLUDE_MATH_CAPABILITIES
milesString milesArray::outputstring(void) const {
	milesArray elemlens = arraylength;
	milesInt tempelem;
	for (size_t i = 0; i < arraylength; i++) {
		tempelem = arraydata[i];
		elemlens[i] = tempelem.length();
	}
	milesString ret = elemlens.sum() + arraylength + 1; //allocate space in return value for each of the elements' lengths, plus a comma for each element, minus one (no comma at end), plus two (brackets);
	ret[0] = '{';
	ret[ret.length() - 1] = '}';
	size_t sofar = 1; //may not be appropriate use of size_t; it is possible the string will be bigger than the array, but in this case, there would already be an error
	milesString tempelemstr;
	for (size_t i = 0; i < arraylength; i++) {
		tempelem = arraydata[i];
		tempelemstr = tempelem.tostring();
		for (size_t k = 0; k < tempelemstr.length(); k++) {
			ret[sofar + k] = tempelemstr[k];
		}
		if (i) ret[sofar - 1] = ',';
		sofar += tempelemstr.length() + 1;
	}
	return ret;
}
#endif

int milesArray::product(void) const {
	int output = 1;
	for (size_t i = 0; i < arraylength; i++) {
		output *= arraydata[i];
	}
	return output;
}

int milesArray::product(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	int output = 1;
	for (size_t i = start; i < end; i++) {
		output *= arraydata[i];
	}
	return output;
}

int milesArray::sum(void) const {
	int output = 0;
	for (size_t i = 0; i < arraylength; i++) {
		output += arraydata[i];
	}
	return output;
}

int milesArray::sum(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	int output = 0;
	for (size_t i = start; i < end; i++) {
		output += arraydata[i];
	}
	return output;
}

double milesArray::median(void) const {
	milesArray arrascending = ascending();
	if (arraylength % 2) {
		return arrascending.arraydata[(arraylength - 1) / 2];
	}
	else {
		return (arrascending.arraydata[arraylength / 2] + arrascending.arraydata[arraylength / 2 - 1])/ 2.0;
	}
}

double milesArray::median(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	milesArray arrascending = subarray(start, end - start).ascending();
	if (arrascending.length() % 2) {
		return arrascending.arraydata[(arrascending.length() - 1) / 2];
	}
	else {
		return (arrascending.arraydata[arrascending.length() / 2] + arrascending.arraydata[arrascending.length() / 2 - 1]) / 2.0;
	}
}

int milesArray::min(void) const {
	int minvalue = arraydata[0];
	for (size_t i = 1; i < arraylength; i++) {
		if (arraydata[i] < minvalue) {
			minvalue = arraydata[i];
		}
	}
	return minvalue;
}

int milesArray::max(void) const {
	int maxvalue = arraydata[0];
	for (size_t i = 1; i < arraylength; i++) {
		if (arraydata[i] > maxvalue) {
			maxvalue = arraydata[i];
		}
	}
	return maxvalue;
}

int milesArray::min(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	int minvalue = arraydata[start];
	for (size_t i = start+1; i < end; i++) {
		if (arraydata[i] < minvalue) {
			minvalue = arraydata[i];
		}
	}
	return minvalue;
}

int milesArray::max(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	int maxvalue = arraydata[start];
	for (size_t i = start+1; i < end; i++) {
		if (arraydata[i] > maxvalue) {
			maxvalue = arraydata[i];
		}
	}
	return maxvalue;
}

int milesArray::mode(void) const {
	milesArray points = arraylength;
	milesArray counts = arraylength;
	points.fill(0);
	counts.fill(0);
	size_t trackerindex;
	size_t founditems = 0;
	for (size_t i = 0; i < arraylength; i++) {
		trackerindex = points.find(arraydata[i]);
		if (trackerindex == points.length()) {
			trackerindex = founditems;
			points.edit(founditems, arraydata[i]);
			founditems++;
		}
		counts.edit(trackerindex, counts.element(trackerindex) + 1);
	}
	return points.element(counts.find(counts.max()));
}

int milesArray::mode(size_t start, size_t end) const {
	if (end > arraylength) throw "Index out of range";
	milesArray points = end-start;
	milesArray counts = end-start;
	points.fill(0);
	counts.fill(0);
	size_t trackerindex;
	size_t founditems = 0;
	for (size_t i = start; i < end; i++) {
		trackerindex = points.find(arraydata[i]);
		if (trackerindex == points.length()) {
			trackerindex = founditems;
			points.edit(founditems, arraydata[i]);
			founditems++;
		}
		counts.edit(trackerindex, counts.element(trackerindex) + 1);
	}
	return points.element(counts.find(counts.max()));
}

void milesArray::multiply(const milesArray& arr) {
	if (arr.arraylength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] *= arr.arraydata[i];
	}
}

void milesArray::multiply(const int* vect, size_t vectlength) {
	if (vectlength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] *= vect[i];
	}
}

void milesArray::multiply(const std::initializer_list<int> &list) {
	if (list.size() != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] *= list.begin()[i];
	}
}

void milesArray::add(const milesArray& arr) {
	if (arr.arraylength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] += arr.arraydata[i];
	}
}

void milesArray::add(const int* vect, size_t vectlength) {
	if (vectlength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] += vect[i];
	}
}

void milesArray::add(const std::initializer_list<int> &list) {
	if (list.size() != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] += list.begin()[i];
	}
}

void milesArray::divide(const milesArray& arr) {
	if (arr.arraylength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] /= arr.arraydata[i];
	}
}

void milesArray::divide(const int* vect, size_t vectlength) {
	if (vectlength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] /= vect[i];
	}
}

void milesArray::divide(const std::initializer_list<int> &list) {
	if (list.size() != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] /= list.begin()[i];
	}
}

void milesArray::subtract(const milesArray& arr) {
	if (arr.arraylength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] -= arr.arraydata[i];
	}
}

void milesArray::subtract(const int* vect, size_t vectlength) {
	if (vectlength != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] -= vect[i];
	}
}

void milesArray::subtract(const std::initializer_list<int> &list) {
	if (list.size() != arraylength) {
		throw "The two arrays cannot be operated because they are of different length.";
		return;
	}
	for (size_t i = 0; i < arraylength; i++) {
		arraydata[i] -= list.begin()[i];
	}
}

//OPERATOR OVERLOADS:

milesArray operator+(milesArray arr1, const milesArray &arr2) {
	arr1.concat(arr2);
	return arr1;
}
milesArray milesArray::operator*(size_t count) {
	milesArray multiplied = arraylength * count; //compressed coded allocate to initialization constructor
	for (size_t i = 0; i < arraylength * count; i++) {
		multiplied.arraydata[i] = arraydata[i % arraylength];
	}
	return multiplied;
}

bool operator==(const milesArray& arr1, const milesArray& arr2) {
	bool flag = 1;
	if (arr1.arraylength == arr2.arraylength) {
		for (size_t i = 0; i < arr1.arraylength; i++) {
			if (arr1.arraydata[i] != arr2.arraydata[i]) {
				flag = 0;
			}
		}
		return flag;
	}
	else {
		return 0;
	}
}

/*--------------------------------MILESINT--------------------------------*/

milesInt::milesInt(const milesDecimal& initvalue) { value = initvalue.read(); }

milesInt operator+(milesInt val1, milesInt val2) { milesInt output(val1.read() + val2.read()); return output; }
milesInt operator+(milesInt val1, int val2) { milesInt output(val1.read() + val2); return output; }
milesInt operator+(int val1, milesInt val2) { milesInt output(val1 + val2.read()); return output; }

milesInt operator-(milesInt val1, milesInt val2) { milesInt output(val1.read() - val2.read()); return output; }
milesInt operator-(milesInt val1, int val2) { milesInt output(val1.read() - val2); return output; }
milesInt operator-(int val1, milesInt val2) { milesInt output(val1 - val2.read()); return output; }

milesInt operator*(milesInt val1, milesInt val2) { milesInt output(val1.read() * val2.read()); return output; }
milesInt operator*(milesInt val1, int val2) { milesInt output(val1.read() * val2); return output; }
milesInt operator*(int val1, milesInt val2) { milesInt output(val1 * val2.read()); return output; }

milesInt operator/(milesInt val1, milesInt val2) { milesInt output(val1.read() / val2.read()); return output; }
milesInt operator/(milesInt val1, int val2) { milesInt output(val1.read() / val2); return output; }
milesInt operator/(int val1, milesInt val2) { milesInt output(val1 / val2.read()); return output; }

milesInt operator%(milesInt val1, milesInt val2) { milesInt output(val1.read() % val2.read()); return output; }
milesInt operator%(milesInt val1, int val2) { milesInt output(val1.read() % val2); return output; }
milesInt operator%(int val1, milesInt val2) { milesInt output(val1 % val2.read()); return output; }

#ifdef INCLUDE_MATH_CAPABILITIES
milesInt operator^(milesInt val1, milesInt val2) { milesInt output(pow(val1.read(), val2.read())); return output; }
milesInt operator^(milesInt val1, int val2) { milesInt output(pow(val1.read(), val2)); return output; }
milesInt operator^(int val1, milesInt val2) { milesInt output(pow(val1, val2.read())); return output; }
#endif

bool operator<(milesInt val1, milesInt val2) { return val1.read() < val2.read(); }
bool operator<(milesInt val1, int val2) { return val1.read() < val2; }
bool operator<(int val1, milesInt val2) { return val1 < val2.read(); }

bool operator>(milesInt val1, milesInt val2) { return val1.read() > val2.read(); }
bool operator>(milesInt val1, int val2) { return val1.read() > val2; }
bool operator>(int val1, milesInt val2) { return val1 > val2.read(); }

bool operator<=(milesInt val1, milesInt val2) { return val1.read() <= val2.read(); }
bool operator<=(milesInt val1, int val2) { return val1.read() <= val2; }
bool operator<=(int val1, milesInt val2) { return val1 <= val2.read(); }

bool operator>=(milesInt val1, milesInt val2) { return val1.read() >= val2.read(); }
bool operator>=(milesInt val1, int val2) { return val1.read() >= val2; }
bool operator>=(int val1, milesInt val2) { return val1 >= val2.read(); }

bool operator==(milesInt val1, milesInt val2) { return val1.read() == val2.read(); }
bool operator==(milesInt val1, int val2) { return val1.read() == val2; }
bool operator==(int val1, milesInt val2) { return val1 == val2.read(); }

bool operator!=(milesInt val1, milesInt val2) { return val1.read() != val2.read(); }
bool operator!=(milesInt val1, int val2) { return val1.read() != val2; }
bool operator!=(int val1, milesInt val2) { return val1 != val2.read(); }

#ifdef INCLUDE_MATH_CAPABILITIES
size_t milesInt::length(void) const {
	if (value == 0)
		return 1;
	if (value == 1)
		return 1;
	if (value == -1)
		return 2;
	double logoutput;
	if (value > 0) {
		logoutput = log10(value);
	}
	else {
		logoutput = log10(-value)+1;
	}
	return ceil(logoutput);
}

void milesInt::writefromstring(const milesString& newvaluestr) {
	value = (int)newvaluestr.tonumber();
}
void milesInt::writefromstring(const char* charstr) {
	milesString newvaluestr(charstr);
	value = (int)newvaluestr.tonumber();
}

milesString milesInt::tostring(void) const {
	bool isNegative = (value < 0);
	int absvalue = isNegative ? -value : value;
	size_t newlength = length();
	size_t modulo = pow(10, newlength - 1);
	int filter = 0;
	int currentdigit;
	milesString newstring = newlength;
	if (isNegative) newstring[0] = '-';
	for (size_t i = 0; i < newlength; i++) {
		currentdigit = (absvalue - filter - absvalue % modulo) / modulo;
		filter += currentdigit * modulo;
		if(i || !isNegative) newstring.edit(i, currentdigit + '0');
		modulo /= 10;
	}
	return newstring;
}
#endif

/*--------------------------------MILESDECIMAL--------------------------------*/

milesDecimal::milesDecimal(const milesInt& initvalue) { value = initvalue.read(); }

milesDecimal operator+(milesDecimal val1, milesDecimal val2) { milesDecimal output(val1.read() + val2.read()); return output; }
milesDecimal operator+(milesDecimal val1, long double val2) { milesDecimal output(val1.read() + val2); return output; }
milesDecimal operator+(long double val1, milesDecimal val2) { milesDecimal output(val1 + val2.read()); return output; }

milesDecimal operator-(milesDecimal val1, milesDecimal val2) { milesDecimal output(val1.read() - val2.read()); return output; }
milesDecimal operator-(milesDecimal val1, long double val2) { milesDecimal output(val1.read() - val2); return output; }
milesDecimal operator-(long double val1, milesDecimal val2) { milesDecimal output(val1 - val2.read()); return output; }

milesDecimal operator*(milesDecimal val1, milesDecimal val2) { milesDecimal output(val1.read() * val2.read()); return output; }
milesDecimal operator*(milesDecimal val1, long double val2) { milesDecimal output(val1.read() * val2); return output; }
milesDecimal operator*(long double val1, milesDecimal val2) { milesDecimal output(val1 * val2.read()); return output; }

milesDecimal operator/(milesDecimal val1, milesDecimal val2) { milesDecimal output(val1.read() / val2.read()); return output; }
milesDecimal operator/(milesDecimal val1, long double val2) { milesDecimal output(val1.read() / val2); return output; }
milesDecimal operator/(long double val1, milesDecimal val2) { milesDecimal output(val1 / val2.read()); return output; }

#ifdef INCLUDE_MATH_CAPABILITIES
milesDecimal operator%(milesDecimal val1, milesDecimal val2) { milesDecimal output(fmod(val1.read(), val2.read())); return output; }
milesDecimal operator%(milesDecimal val1, long double val2) { milesDecimal output(fmod(val1.read(), val2)); return output; }
milesDecimal operator%(long double val1, milesDecimal val2) { milesDecimal output(fmod(val1, val2.read())); return output; }

milesDecimal operator^(milesDecimal val1, milesDecimal val2) { milesDecimal output(pow(val1.read(), val2.read())); return output; }
milesDecimal operator^(milesDecimal val1, long double val2) { milesDecimal output(pow(val1.read(), val2)); return output; }
milesDecimal operator^(long double val1, milesDecimal val2) { milesDecimal output(pow(val1, val2.read())); return output; }
#endif

bool operator<(milesDecimal val1, milesDecimal val2) { return val1.read() < val2.read(); }
bool operator<(milesDecimal val1, long double val2) { return val1.read() < val2; }
bool operator<(long double val1, milesDecimal val2) { return val1 < val2.read(); }

bool operator>(milesDecimal val1, milesDecimal val2) { return val1.read() > val2.read(); }
bool operator>(milesDecimal val1, long double val2) { return val1.read() > val2; }
bool operator>(long double val1, milesDecimal val2) { return val1 > val2.read(); }

bool operator<=(milesDecimal val1, milesDecimal val2) { return val1.read() <= val2.read(); }
bool operator<=(milesDecimal val1, long double val2) { return val1.read() <= val2; }
bool operator<=(long double val1, milesDecimal val2) { return val1 <= val2.read(); }

bool operator>=(milesDecimal val1, milesDecimal val2) { return val1.read() >= val2.read(); }
bool operator>=(milesDecimal val1, long double val2) { return val1.read() >= val2; }
bool operator>=(long double val1, milesDecimal val2) { return val1 >= val2.read(); }

bool operator==(milesDecimal val1, milesDecimal val2) { return val1.read() == val2.read(); }
bool operator==(milesDecimal val1, long double val2) { return val1.read() == val2; }
bool operator==(long double val1, milesDecimal val2) { return val1 == val2.read(); }

bool operator!=(milesDecimal val1, milesDecimal val2) { return val1.read() != val2.read(); }
bool operator!=(milesDecimal val1, long double val2) { return val1.read() != val2; }
bool operator!=(long double val1, milesDecimal val2) { return val1 != val2.read(); }

#ifdef INCLUDE_MATH_CAPABILITIES
size_t milesDecimal::length(void) const {
	if (value <= 1 && value >= 0)
		return 1;
	if (value < 0 && value >= -1)
		return 2;
	double logoutput;
	if (value > 0) {
		logoutput = log10(value);
	}
	else {
		logoutput = log10(-value) + 1;
	}
	return ceil(logoutput);
}

size_t milesDecimal::length(size_t decimals) const {
	if (value <= 1 && value >= 0)
		return 1;
	if (value < 0 && value >= -1)
		return 2;
	double logoutput;
	if (value > 0) {
		logoutput = log10(value);
	}
	else {
		logoutput = log10(-value) + 1;
	}
	return ceil(logoutput)+(decimals > 0)+decimals;
}

void milesDecimal::writefromstring(const milesString& newvaluestr) {
	value = newvaluestr.tonumber();
}
void milesDecimal::writefromstring(const char* charstr) {
	milesString newvaluestr(charstr);
	value = (long double)newvaluestr.tonumber();
}

milesString milesDecimal::tostring(void) const {
	bool isNegative = (value < 0);
	int absvalue = isNegative ? -floor(value) : floor(value);
	size_t newlength = length();
	size_t modulo = pow(10, newlength - 1);
	int filter = 0;
	int currentdigit;
	milesString newstring = newlength;
	if (isNegative) newstring[0] = '-';
	for (size_t i = 0; i < newlength; i++) {
		currentdigit = (absvalue - filter - absvalue % modulo) / modulo;
		filter += currentdigit * modulo;
		if (i || !isNegative) newstring.edit(i, currentdigit + '0');
		modulo /= 10;
	}
	return newstring;
}

milesString milesDecimal::tostring(size_t decimals) const {
	bool isNegative = (value < 0);
	long double absvalue = isNegative ? -value : value;
	size_t intlength = length();
	bool isfloat = decimals > 0;
	long double modulo = pow(10, intlength - 1);
	long double filter = 0;
	int currentdigit;
	milesString newstring = intlength + decimals + isfloat;
	if (isNegative) newstring[0] = '-';
	for (size_t i = 0; i < intlength; i++) {
		currentdigit = (absvalue - filter - fmod(absvalue, modulo)) / modulo;
		filter += currentdigit * modulo;
		if(i || !isNegative) newstring.edit(i, currentdigit + '0');
		modulo /= 10;
	}
	newstring.edit(intlength, '.');
	if (isfloat) {
		for (size_t i = intlength + 1; i < decimals + intlength + 1; i++) {
			currentdigit = (absvalue - filter - fmod(absvalue, modulo)) / modulo;
			filter += currentdigit * modulo;
			newstring.edit(i, currentdigit + '0');
			modulo /= 10;
		}
	}
	return newstring;
}
#endif

//--------------------------------OTHERFUNCTIONS--------------------------------//
void milesintarrcpy(int* dest, const int* source, size_t cpylength) {
	for (size_t i = 0; i < cpylength; i++) { //copy function
		dest[i] = source[i];
	}
}

void milesintarrcat(int* dest, const int* source, size_t currentlength, size_t catlength) {
	for (size_t i = currentlength; i < currentlength + catlength; i++) {
		dest[i] = source[i - currentlength];
	}
}

void mileschararrcpy(char* dest, const char* source, size_t cpylength) {
	for (size_t i = 0; i < cpylength; i++) { //copy function
		dest[i] = source[i];
	}
	dest[cpylength] = '\0';
}

void mileschararrcat(char* dest, const char* source, size_t currentlength, size_t catlength) {
	for (size_t i = currentlength; i < currentlength + catlength; i++) {
		dest[i] = source[i - currentlength];
	}
	dest[currentlength + catlength] = '\0';
}

size_t milesstrlen(const char* charstr) {
	size_t sofar = 0;
	while (charstr[sofar]) { ++sofar; }
	return sofar;
}
