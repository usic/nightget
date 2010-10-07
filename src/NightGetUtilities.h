#ifndef NIGHT_GET_UTIL_H
#define NIGHT_GET_UTIL_H

#include <sstream>
#include <string>
using namespace std;

extern int string2int(const string& );

template<typename T>
string a2string(const T& val) {
	ostringstream ss;
	ss << val;
	return ss.str();
}

extern string get_today();

extern void log(const char*);

#endif // NIGHT_GET_UTIL_H
