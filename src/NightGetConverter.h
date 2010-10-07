#ifndef NIGHT_GET_CONVERTER_H
#define NIGHT_GET_CONVERTER_H

#include <map>
using namespace std;
#include <iostream>
#include <tr1/memory>
using namespace tr1;

#include "NightGetDatabase.h"
#include "NightGetNodes.h"
#include "NightGetDownload.h"
#include "NightGetUtilities.h"
#include "NightGetException.h"
/*
  General specification for class Converter used by NightGetDatabase
  Attention: should you not provide Converter for your specific object and database, compiler will tell you "get lost"
*/
template<typename Object>
class Converter {
public:
	void* convertForDB(const Object& obj);
	shared_ptr<Object> convertFromDB(void* data);
	template<typename T>
	void* convertComparison(const ComparisonStruct<T>& condition);
	void* getComparison();
private:
	void* _comparison; 
};

/*
  Converter for MySQL and NightGetNode (NightGetNode.h) combo.
  Attention: fields names for MySQL tables are hardcoded here
*/
template<>
class Converter<NightGetNode> {
public:
	Converter():_compIsSet(false),_comparison(string()) {};
	~Converter() {};
	map<string,string> convertForDB(const NightGetNode& obj) const;
	shared_ptr<NightGetNode> convertFromDB( map<string,string>& data) const;
	template<typename T>
	void convertComparison(const ComparisonStruct<T>& condition);
	string getComparison();
	void setNullComparison();
private:
	Converter(const Converter&);
	Converter& operator=(const Converter&);

	bool _compIsSet;
	string _comparison;
	string processSQLComparison(const ComparisonType& cr) const;
};


	map<string,string> Converter<NightGetNode>::convertForDB(const NightGetNode& obj) const {
		map<string,string> converted;

		if ( !obj.get_user().empty() )
			if ( !obj.get_password().empty() )
				converted["url"] = obj.get_user()+":"+obj.get_password()+"@"+obj.get_url();
			else
				converted["url"] = obj.get_user()+"@"+obj.get_url();
		else
			converted["url"] = obj.get_url();
		converted["path"] = obj.get_path();
		converted["state"] = a2string(obj.state()+1);	/* MySQL is counting from one, not zero */
		database_id id = obj.get_id();
		if (id <= 0)
			throw BrokenDBObject("NightGetNode with no id");
		converted["id"] = a2string(id);

		return converted;
	};

	shared_ptr<NightGetNode> Converter<NightGetNode>::convertFromDB( map<string,string>& data) const {
		string user,pass,url;
		size_t ind1, ind2;
		if ( (ind1 = data["url"].find_first_of('@') ) != string::npos) {
			user = data["url"].substr(0,ind1);	/* user[:password] */
			url = data["url"].substr(ind1+1,string::npos);

			if ( (ind2 = user.find_first_of(':') ) != string::npos) {
				pass = user.substr(ind2+1,string::npos);
				user = user.substr(0,ind2);
			}
		} else url = data["url"];

		database_id id = string2int(data["id"]); 

		if (data["protocol"]=="http") {
			shared_ptr<NightGetNode> res(new NightGetNode(NightGetHTTP(),url,data["path"],user,pass,id));
			return res;
		} if (data["protocol"]=="ftp") {
			shared_ptr<NightGetNode> res(new NightGetNode(NightGetFTP(),url,data["path"],user,pass,id));
			return res;
		}
		throw BrokenDBObject("record with no protocol");
		shared_ptr<NightGetNode> res(new NightGetNode(NightGetHTTP(),"",""));
		return res;
	};

	template<typename T>
	void Converter<NightGetNode>::convertComparison(const ComparisonStruct<T>& condition) {
		if ( condition.attrname != "id" && condition.attrname != "uid" &&
			condition.attrname != "begin" && condition.attrname != "finish" &&
			condition.attrname != "protocol" && condition.attrname != "path" &&
			condition.attrname != "url" ) {
			throw NightGetException("MisUse => there is no such attribute as "+condition.attrname); 
			return;	//nanja ?!
		}

		string add (condition.attrname + processSQLComparison(condition.ctype) + "'" + a2string(condition.value) + "'");
		if (_compIsSet)
			 _comparison.append(" and ").append(add);
		else { _compIsSet = true; 
			_comparison.append(add); 
		}
	};

	string Converter<NightGetNode>::getComparison() { 
		_compIsSet = false;
		if (!_comparison.empty()) {
			string copy;
			_comparison.swap(copy);
			return " and "+copy; 
		} 
		return _comparison;
	};

	void Converter<NightGetNode>::setNullComparison() {
		_compIsSet = false;
		_comparison.clear();
	};

	string Converter<NightGetNode>::processSQLComparison(const ComparisonType& cr) const {
		string cmp;
		switch (cr) {
			case ne: cmp="<>"; break;
			case lt: cmp="<"; break;
			case gt: cmp=">"; break;
			case le: cmp="<="; break;
			case ge: cmp=">="; break;
			default: cmp="="; break;
		}
		return cmp;
	}

#endif // NIGHT_GET_CONVERTER_H
