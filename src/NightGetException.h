#ifndef NIGHT_GET_EXCEPT_H
#define NIGHT_GET_EXCEPT_H

#include <string>

class NightGetException: public std::exception {
public:
	NightGetException(const string& msg): _error("NightGetException: "+msg) {};
	const char* what() const throw() { return _error.c_str(); };
	virtual ~NightGetException() throw() {};
private:
 	string _error;
};

class ThreadException: public NightGetException {
public:
	ThreadException(const string& msg): NightGetException("thread=>"+msg) {};
	virtual ~ThreadException() throw() {};
};

class DownloadException: public NightGetException {
public:
	DownloadException(const string& msg): NightGetException("download=>"+msg) {};
	virtual ~DownloadException() throw() {};
};
class DBException: public NightGetException {
public:
	DBException(const string& msg): NightGetException("database=>"+msg) {};
	virtual ~DBException() throw() {};
};
class BrokenDBObject: public NightGetException {
public:
	BrokenDBObject(const string& msg): NightGetException("broken db object=>"+msg) {};
	virtual ~BrokenDBObject() throw() {};
};

#endif // NIGHT_GET_EXCEPT_H
