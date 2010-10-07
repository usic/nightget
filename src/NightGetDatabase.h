#ifndef NIGHT_GET_DB_H
#define NIGHT_GET_DB_H

#include <string>
#include <list>
using namespace std;
#include <tr1/memory>
using namespace tr1;


typedef unsigned long database_id;	//TODO: chg the node_id typedef  database_id

enum THREAD_POLICY { simple, threads };

enum ComparisonType { eq, ne, lt, gt, le, ge };

template<typename T>
struct ComparisonStruct {
	ComparisonStruct(const T& v, const string& attrib,const ComparisonType& ct): value(v), attrname(attrib), ctype(ct) {};
	T value;
	string attrname;
	ComparisonType ctype;
};

/*
  Very small interface/

  Template parameters:
  Object - class thar corresponds to your downloads and holds/uses info from database
  ConvType - class that converts db data to Object and vice versa (details in NightGetConverter.h)
  THREAD_POLICY - flag whether the application is compiling with threads (implementation specific)
*/

template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
class NightGetDatabase {
public:
	/* all connecting */
	NightGetDatabase():_converter(new ConvType<Object>) {};
	/* all disconnecting */
	virtual ~NightGetDatabase() throw() { delete _converter; };

	int countObjects () {return do_countObjects(); };

	inline list<shared_ptr<Object> > getObjects () {return do_getObjects();};

	template<typename T>
	inline list<shared_ptr<Object> > getObjects (const ComparisonStruct<T>& condition) {
		_converter->convertComparison(condition);
		return do_getObjects();
	};

	template<typename T, typename U>
	inline list<shared_ptr<Object> > getObjects (const ComparisonStruct<T>& condition1,const ComparisonStruct<U>& condition2) {
 		_converter->convertComparison(condition1);
		_converter->convertComparison(condition2);
		return do_getObjects();
	};

	inline int save(const Object& data) { return do_save(data);};

	int startThread() const 	{ return (tp==threads)?do_startThread():-1; };	// implementation specific
	int endThread() const		{ return (tp==threads)?do_endThread():-1; };	// implementation specific

protected:

	ConvType<Object>* _converter;

private:
	/* there can be only one */
	NightGetDatabase(const NightGetDatabase&);
	NightGetDatabase& operator=(const NightGetDatabase&);

	virtual int do_countObjects()=0;
	virtual list<shared_ptr<Object> > do_getObjects()=0;
	virtual int do_save(const Object& data)=0;
	virtual int do_startThread() const=0;
	virtual int do_endThread() const=0;
};

#endif // NIGHT_GET_DB_H
