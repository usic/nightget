/***************************************************************************
 *   Copyright (C) USIC 2008 by Sergiy Kibrik, Olha Yevtushenko   *
 *   sakib@meta.ua,caelum@meta.ua   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef NIGHT_GET_DB
#define NIGHT_GET_DB
#include <string>
#include <list>
#include <map>

using namespace std;

class DatabaseExceptions;

class AbstractDatabase {
public:
//i think it would be better to distinct connection to database/opening of the file from creation of the object 
//(taking into consideration that many database apis have function of initialization !=connect)
//something like init and bind
//in plain text constructor would do nothing	
	inline int connect( const string& database, const string& host="", const string& user="", const string& password="") 
		{ return do_connect(database, host, user, password); }
	inline void disconnect()
		{ do_disconnect(); }
	//full record by id number and name of the table 
	inline map<string,string> getRecord(const string id&, string table="")
		{ return do_getRecord(id, table); }
	//Assotiative Array contains 2 strings
	//option - write it as a template
	inline void updateRecord(const string& id, const string& table, const string& value) 
		{ do_updateRecord(id,table,value); }
	inline void createRecord(const string& id, const map<string,string>& record)
		{ do_createRecord(id, record); }
	inline void deleteRecord(const string& id)
		{ do_deleteRecord(id); }
	inline const list<string> getRecords(const string& table="")
		{ return do_getRecords(table); }
	//the name and type of columns
	inline map<string,string> getColumnsInfo(const string& table="")
		{ return do_getColumnsInfo(table); }
	inline unsigned int getNumberColumns(const string& table="")
		{ return do_getNumberColumns(table); }
	inline unsigned int getNumberRows(const string& table="")
		{ return do_getNumberRows(table); }
protected:
	AbstractDatabase();
	~AbstractDatabase();	
private:
	virtual int do_connect(const string& database, const string& host, const string& user, const string& password) = 0;
	virtual void do_disconnect() = 0;
	virtual map<string,string> do_getRecord(const string& id, const string& table) const = 0;
	virtual void do_updateRecord(const string& id, const string& table, const string& value) = 0;
	virtual void do_createRecord(const strinf& id, const map<string,string>& record) = 0;
	virtual void do_deleteRecord(const string& id) = 0;
	virtual list<string> do_getRecords(const string& table ) const = 0;
	virtual map<string,string> do_getColumnsInfo(const string& table) const = 0;
	virtual unsigned int do_getNumberColumns() const = 0;
	virtual unsigned int do_getNumberRows(const string& table="") const = 0;
};

class NightGetDB: public AbstractDatabase {
public:
	NightGetDB();
	~NightGetDB();
private:
	virtual int do_connect(const string& database, const string& host, const string& user, const string& password);
	virtual void do_disconnect();
	virtual map<string,string> do_getRecord(const string& id, const string& table) const;
	virtual void do_updateRecord(const string& id, const string& table, const string& value);
	virtual void do_createRecord(const strinf& id, const map<string,string>& record);
	virtual list<string> do_getRecords(const string& table ) const;
	virtual map<string,string> do_getColumnsInfo(const string& table) const;
	virtual unsigned int do_getNumberColumns() const;
	virtual unsigned int do_getNumberRows(const string& table="") const;
	
	struct DBImpl;
	
	DBImpl* _db; 
};

//part of Mysql version
// class MySQLDatabase: public AbstractDatabase
// {
// typedef MYSQL* handler;
// public:
// 	MySQLDatabase() { mysql_init(_link); };
// 	virtual ~MySQLDatabase(){ mysql_close(_link); };
// 	
// private:
// 	virtual void do_connect(const string& host, const string& user, const string& password, const string& database) {
// 		try {
// 			mysql_real_connect(_link, host, user, passwd, database, 0, NULL, 0);
// 		} //throw exceptions class DatabaseExceptions
// 	};
// 	virtual AssotiativeArray do_getRecord(string id, string table) {
// 		try {
// 			mysql_query(_link, "SELECT * FROM "+table+" WHERE id="+id.c_str());
// 			MYSQL_RES* res = mysql_store_result(_link);
// 			if (res) {
// 			    int fields_number = mysql_num_fields(res);
// 				MYSQL_FIELD* fields_names = mysql_fetch_fields(res);
// 				MYSQL_ROW fields_values = mysql_fetch_row();	//row must be only one because id is unique
// 				AssotiativeArray record(fields_number);
// 				for(i = 0; i<fields_number; i++) {
// 					record[fields_names[i].name] = fields_values[i];
// 				}
// 				mysql_free_result(res);
// 			} 
// 			//else throw 
// 		}
// 		return record;
// 	}
// 	
// 	hanlder _link;
// };

# endif // NIGHT_GET_DB
