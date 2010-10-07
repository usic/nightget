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
#include <stdio.h>
#include <string>
#include <curl/curl.h>
using namespace std;

#include "NightGet.h"
#include "NightGetDownload.h"
#include "NightGetNodes.h"
#include "NightGetException.h"

#define DEFAULT_FTP_USER "anonymous"
#define DEFAULT_FTP_PASS "show@must.go.on"
#define CURL_OK 0x0

// Class NightGetCurlInt

unsigned long NightGetCurlInt::_global_inits = 0;

struct NightGetCurlInt::HandlerImpl {
	CURL* _handler;
};

NightGetCurlInt::NightGetCurlInt(const string& url, FILE* stream, const string& userpwd):
	_hPtr ( new HandlerImpl() ) {
	
	if (! _global_inits ){
		if ( CURLcode err = curl_global_init(CURL_GLOBAL_ALL) )
			throw DownloadException(curl_easy_strerror(err));
	}
	++_global_inits;
	
	_hPtr->_handler = curl_easy_init(); // TODO exeption if NULL handler
	if (_hPtr->_handler == NULL)
		throw DownloadException("curl_easy_init(): couldn't start session");
	#ifndef NDEBUG
	curl_easy_setopt(_hPtr->_handler,CURLOPT_VERBOSE, 1);
	#endif
	setopt(url, stream, userpwd);
}

NightGetCurlInt::~NightGetCurlInt(){
	if (_hPtr) {
		curl_easy_reset(_hPtr->_handler);
		delete _hPtr;
	}
	if (! --_global_inits )
		curl_global_cleanup();
}

void NightGetCurlInt::setopt(const string& url, FILE* stream, const string& userpwd){
	curl_easy_setopt( _hPtr->_handler, CURLOPT_URL, url.c_str() );
	curl_easy_setopt( _hPtr->_handler, CURLOPT_WRITEDATA, stream);
	if (! userpwd.empty() ) 
		curl_easy_setopt( _hPtr->_handler, CURLOPT_USERPWD, userpwd.c_str() );
}

int  NightGetCurlInt::perform(){
	if ( curl_easy_perform(_hPtr->_handler) != CURL_OK )
		return EXIT_FAILURE;
	else return EXIT_SUCCESS;
}

// Class NightGetFTP
const state_t  NightGetFTP::do_download(const NightGetNode& node){
	state_t _state = node.state();
	if ( _state == NG_FINISHED || _state == NG_EMPTY )
		return _state;
	if ( _state == NG_STARTED )
		return NG_ERROR;
	string _url = node.get_url();
	string _path = node.get_path();
	FILE* _file = fopen(_path.c_str(), "w");
	string _user = node.get_user();
	string _password = node.get_password();
	if ( _user.empty() || _password.empty() ){
		_user = DEFAULT_FTP_USER;
		_password = DEFAULT_FTP_PASS;
	}
	NightGetCurlInt _performer( _url, _file, _user+":"+_password);
	_state = NG_STARTED;
	if ( _performer.perform() )
		_state = NG_ERROR; 
	else
		_state = NG_FINISHED;
	fclose(_file);
	return _state;
}

NightGetFTP* NightGetFTP::do_clone() const {
	return new NightGetFTP;
}

// Class NightGetHTTP
const state_t  NightGetHTTP::do_download(const NightGetNode& node){
	state_t _state = node.state();
	if ( _state == NG_FINISHED || _state == NG_EMPTY )
		return _state;
	if ( _state == NG_STARTED )
		return NG_ERROR;
	string _url = node.get_url();
	string _path = node.get_path();
	FILE* _file = fopen(_path.c_str(), "w");
	string _user = node.get_user();
	string _password = node.get_password();

	NightGetCurlInt _performer( _url, _file, _user+":"+_password);
	_state = NG_STARTED;
	if ( _performer.perform() )
		_state = NG_ERROR; 
	else
		_state = NG_FINISHED;
	fclose(_file);
	return _state;
}

NightGetHTTP* NightGetHTTP::do_clone() const {
	return new NightGetHTTP;
}
