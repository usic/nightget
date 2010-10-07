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
#include <cassert>
#include <db_cxx.h>
#include <string>
#include <list>
#include <map>
#include "NightGetDB.h"

struct NightGetDB::DBImpl {
	DBImpl(): _db(NULL,0){}
	Db _db;
};

NightGetDB::NightGetDB():
	_db( new DBImpl ){}

NightGetDB::~NightGetDB(){
	do_disconnect();
	if ( _db ) delete _db;
}

int NightGetDB::do_connect(const string& database, const string& host, const string& user, const string& password){
	u_int32_t open_flags = DB_CREATE;
	//TODO fix damned  quickstart params !!!
	_db->_db.open(
		NULL,
		database.c_str(),
		NULL,
		DB_BTREE,
		open_flags,
		0
		);
	return EXIT_SUCCESS;
}

void NightGetDB::do_disconnect(){
	_db->_db.close(0);
}

map<string,string> NightGetDB::do_getRecord(const string& id, const string& table) const {
	Dbc *_cursorPtr;
	_db->_db.cursor(NULL, &_cursorPtr, 0);
	Dbt _key,_data;
	_data.set_data(NULL);
	_data.set_ulen(0);
	map<string,string> _map;
	int ret = 0;
	string _buf;
	if (table.empty()){
		_key.set_data( id.c_str() );
		_key.set_ulen( id.size() + 1);
		const Dbt _ckey(_key);
 		while ( ( ret = _cursorPtr->get(&_key, &_data, DB_NEXT) ) == 0 ){
			_buf.assign( _key.get_data() + id.size() + 1, _key.get_size() - id.size() - 1);
			_map[_buf] = string( static_casr<char*>( _data.get_data() ), _data.get_size() );
			_key = _ckey;
			_buf.clear();
		}
		_cursorPtr->close();
	} else {
		_buf.assign( id + '\0' + table );
		_key.set_data( _buf.c_str() );
		_key.set_ulen( id.size() + table.size() + 1);
		ret = _db->_db.get(NULL,&_key,&_data,0);
		_map[table] = string( static_casr<char*>( _data.get_data() ), _data.get_size() );
	}
	return _map;
}

void NightGetDB::do_updateRecord(const string& id, const string& table, const string& value) {
	Dbt _key, _data;
	string _buf(id + '\0' + table);
	_key.set_data( _buf.c_str() );
	_key.set_ulen( id.size() + table.size() + 1);
	_data.set_ulen( value.size() + 1 );
	_data,set_data( value.c_str() );
	int ret = _db->_db.put(NULL, &_key, &_data,0);
}

void NightGetDB::do_createRecord(const strinf& id, const map<string,string>& record){
	Dbt _key, _data;
	map<string,string>::iterator iter = record.begin();
	string _buf;
	while (iter++ != record.end() ){
		_buf.assign( id + '\0' + iter->first);
		_key.set_data( _buf.c_str() );
		_key.set_ulen( _buf.size() + 1);
		_data.set_data( iter->second.c_str() );
		_data.set_ulen( iter->second.size() + 1);
		_db->_db.put(NULL, &_key, &_data, 0);
		_buf.clear();
	}
}

list<string> NightGetDB::do_getRecords(const string& table ) const {
	string::size_type loc = table.find('=');
	const string _field(table, 0, loc);
	const string _value(table, loc+1, table.size() - _field.size() - 1);
	Dbc *_cursorPtr;
	_db->_db.cursor(NULL, &_cursorPtr, 0);
	Dbt _key(NUll, 0), _data(NULL,0);
	int ret = 0, _field_lenght = 0;
	string _id, _buf;
	list<string> _result_list();
	while ( ( ret = _cursorPtr->get(&_key, &_data, DB_NEXT) ) == 0 ){
		if ( _value.compare( static_cast<char*>_data.get_data() ) != 0 ) continue;
		_id.assign( _key.get_data() );
		_buf.assign( _key.get_data() + _id.size() + 1);
		if ( _buf.compare(_field) == 0 ){
			_result_list.push_back(_id);
		}
	_data.set_data(NULL);
	_data.set_ulen(0);
	_key.set_data(NULL);
	_key.set_ulen(0);
	_buf.clear(); _id.clear();
	}
	_cursorPtr->close();
	return _result_list;
}

map<string,string> NightGetDB::do_getColumnsInfo(const string& table) const{
	// to be written :(
	return map<string,string>();
}

unsigned int NightGetDB::do_getNumberColumns() const{
	// to be written :(
	return 0;
}
unsigned int NightGetDB::do_getNumberRows(const string& table="") const{
	// to be written :(
	return 0;
}