#ifndef NIGHT_GET_DB_IMPL_H
#define NIGHT_GET_DB_IMPL_H

#include <list>
#include <map>
using namespace std;
#include <iostream>
#include <tr1/memory>
using namespace tr1;

#include <mysql/mysql.h>
#include "NightGetDatabase.h"
#include "NightGetConverter.h"
#include "NightGetUtilities.h"


/*
  Implementation of NightGetDatabase for MySQL server using MySQL C API
  Attention: queries and names of table(s) are hardcoded here
  ./db.cnf - file where configuration data for connecting resides
*/


template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
class MySQLNightGetDatabase: public NightGetDatabase<Object,ConvType,tp> {
public:
	MySQLNightGetDatabase();
	~MySQLNightGetDatabase() throw();

private:
	MySQLNightGetDatabase(const MySQLNightGetDatabase&);
	MySQLNightGetDatabase& operator=(const MySQLNightGetDatabase&);

	int do_countObjects();
	list<shared_ptr<Object> > do_getObjects();
	int do_save(const Object& data);
	int do_startThread() const;
	int do_endThread() const;

	bool checklib() const { return (tp==threads)? mysql_thread_safe():true; };

	MYSQL* _handler;
};


template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
MySQLNightGetDatabase<Object,ConvType,tp>::MySQLNightGetDatabase():
	NightGetDatabase<Object,ConvType,tp>(),
	_handler(0) { 
	if (this->checklib()) {
	#ifndef NDEBUG
	cerr << "\nNightGet: Connecting to MySQL...\n";
	#endif
		if ( mysql_library_init(0, NULL, NULL)!=0 ) 
			throw DBException("MySQL library is silent");
		_handler = mysql_init(NULL);
		if (!_handler) 
			throw DBException("there is no memory for mysql connection");
		mysql_options(_handler, MYSQL_READ_DEFAULT_FILE,"./db.cnf");
		if ( (_handler = mysql_real_connect( _handler, NULL, NULL, NULL, NULL, 0, NULL, 0 )) == NULL) 
			throw DBException("can't connect to the MySQL");
	#ifndef NDEBUG
	cerr << "\nNightGet: Connecting to MySQL successful.\n";
	#endif
	} else throw DBException("this library is not suited for threads");
};

template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
MySQLNightGetDatabase<Object,ConvType,tp>::~MySQLNightGetDatabase() throw() {
		mysql_close(_handler);
		mysql_library_end();
	#ifndef NDEBUG
	cerr << "\nNightGet: Disconnecting from MySQL successful.\n";
	#endif
};


template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
int MySQLNightGetDatabase<Object,ConvType,tp>::do_countObjects() {
//	string query = "select count(id) from nightload where state='init' or state='started'";
	string query = "select count(id) from nightload where state='init'";

	/*unsigned long length = query.length();
	char* escaped_q = new char [length*2+1];
	mysql_real_escape_string(_handler, escaped_q, query.c_str(), length);*/
	#ifndef NDEBUG
	cerr << "\nNightGet: Executing query to MySQL: " << query << endl;
	#endif
	if( mysql_query(_handler, query.c_str()) != 0 ) {
		throw DBException(mysql_error(_handler));
		return -1;
	}

	MYSQL_RES* result = mysql_store_result(_handler);
	if (!result) {
		throw DBException(mysql_error(_handler));
		return -1;
	}

	return string2int(mysql_fetch_row(result)[0]);
};

template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
list<shared_ptr<Object> > MySQLNightGetDatabase<Object,ConvType,tp>::do_getObjects() {
// 	string query = "select url,path,id,protocol from nightload where (state='init' or state='started') " + NightGetDatabase<Object,ConvType,tp>::_converter->getComparison();

	string query = "select url,path,id,protocol from nightload where state='init' "+NightGetDatabase<Object,ConvType,tp>::_converter->getComparison();

	#ifndef NDEBUG
	cerr << "\nNightGet: Executing query to MySQL: " << query << endl;
	#endif
		if( mysql_query(_handler, query.c_str()) != 0 ) {
			throw DBException(mysql_error(_handler));
			return list<shared_ptr<Object> >();
		}

		MYSQL_RES* result = mysql_store_result(_handler);
		if (!result) {
			throw DBException(mysql_error(_handler));
			return list<shared_ptr<Object> >();
		}

		MYSQL_ROW r;
		unsigned int fields = mysql_field_count(_handler);
		unsigned int rows = mysql_num_rows(result);
		MYSQL_FIELD* names = mysql_fetch_fields(result);

		list<shared_ptr<Object> > objects;

 		while (rows--) {
			try {
			map<string,string> mapped_record;
			r = mysql_fetch_row(result);
			for (unsigned int i = 0; i<fields; ++i)
				mapped_record[names[i].name] = r[i];

			shared_ptr<Object> p(NightGetDatabase<Object,ConvType,tp>::_converter->convertFromDB(mapped_record));
			objects.push_back(p);
			} catch (BrokenDBObject& e) {
				log(e.what());
				break;
			}
 		}
		mysql_free_result(result);

		return objects;
}
template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
int MySQLNightGetDatabase<Object,ConvType,tp>::do_save(const Object& data) {

	string date = get_today();
	try {
	map<string,string> mapped = NightGetDatabase<Object,ConvType,tp>::_converter->convertForDB(data);
	
//TODO make prepared statement	
	string query = "update nightload set state='"+mapped["state"]+"', path='"+mapped["path"]+"', finish='"+date+"', url='"+mapped["url"]+"' where id='"+mapped["id"]+"'";

	#ifndef NDEBUG
	cerr << "\nNightGet: Executing query to MySQL: " << query << endl;
	#endif

	if( mysql_query(_handler, query.c_str()) != 0 ) {
		throw DBException(mysql_error(_handler));
		return -1;
	}
	} catch (BrokenDBObject& e) {
		string s (string ("error during saving to database: ")+e.what());
		log (s.c_str());
		throw NightGetException(s.c_str());
		return -1;
	}
	return 0;
}
template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
int MySQLNightGetDatabase<Object,ConvType,tp>::do_startThread() const {
	if (mysql_thread_init() != 0) {
		throw DBException(mysql_error(_handler));
		return -1;
	}
	return 0;
}
template<typename Object, template <typename T> class ConvType, THREAD_POLICY tp>
int MySQLNightGetDatabase<Object,ConvType,tp>::do_endThread() const {
	mysql_thread_end();
	return 0;
}
#endif // NIGHT_GET_DB_IMPL_H
