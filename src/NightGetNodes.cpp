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

#include "NightGet.h"
#include "NightGetNodes.h"
#include "NightGetDownload.h"

using namespace std;

struct NightGetNode::Attrs {
	Attrs(): _downloader(NULL), _id (0), _state ( NG_INIT ) {}
	NightGetDownloadMethod* _downloader;
	node_id _id;
	string _url;
	string _path;
	string _login;
	string _password;
	state_t _state;
};

// Class NightGetNode
NightGetNode::NightGetNode(const NightGetDownloadMethod& dmethod):
	_attrs(new Attrs){
	
	_attrs->_downloader = dmethod.clone();
}

NightGetNode::NightGetNode(const NightGetDownloadMethod& dmethod, const string& url, const string& path, const string& username, const string& password, const node_id& id):_attrs(new Attrs){
	_attrs->_downloader = dmethod.clone();
	_attrs->_url = url;
	_attrs->_path = path;
	_attrs->_login = username;
	_attrs->_password = password;
	_attrs->_id = id;
}


NightGetNode::~NightGetNode(){
	if (_attrs->_downloader) delete _attrs->_downloader;
	if (_attrs) delete _attrs;
}

NightGetNode::NightGetNode(const NightGetNode& node):
	_attrs(new Attrs ){
		
	_attrs->_downloader = node._attrs->_downloader->clone();
	_attrs->_url = node._attrs->_url;
	_attrs->_state = node._attrs->_state;
	_attrs->_path = node._attrs->_path;
	_attrs->_password = node._attrs->_password;
	_attrs->_login = node._attrs->_login;
	_attrs->_id = node._attrs->_id;
}

const NightGetNode& NightGetNode::operator=(const NightGetNode& node){
	if (this != &node){
		_attrs->_downloader = node._attrs->_downloader;
		_attrs->_url = node._attrs->_url;
		_attrs->_state = node._attrs->_state;
		_attrs->_path = node._attrs->_path;
		_attrs->_password = node._attrs->_password;
		_attrs->_login = node._attrs->_login;
		_attrs->_id = node._attrs->_id;
	}
	return *this;
}

void NightGetNode::do_set_url(const string& url){
	_attrs->_url = url;
}

void NightGetNode::do_set_path(const string& path){
	_attrs->_path = path;
}

void NightGetNode::do_set_id(const node_id& id){
	_attrs->_id = id;
}

void NightGetNode::do_set_user(const string& username){
	_attrs->_login = username;
}

void NightGetNode::do_set_password(const string& pass){
	_attrs->_password = pass;
}

const string& NightGetNode::do_get_url() const {
	return _attrs->_url;
}

const string& NightGetNode::do_get_path() const {
	return _attrs->_path;
}

const node_id& NightGetNode::do_get_id() const {
	return _attrs->_id;
}

const string& NightGetNode::do_get_user() const {
	return _attrs->_login;
}

const string& NightGetNode::do_get_password() const {
	return _attrs->_password;
}

const state_t& NightGetNode::do_state() const {
	return _attrs->_state;
}

const state_t& NightGetNode::do_download() {
	return _attrs->_state = _attrs->_downloader->download(*this);
}

void NightGetNode::do_cleanup(){
	_attrs->_state = NG_EMPTY;
	if (! unlink( _attrs->_path.c_str() )){
	#ifndef NDEBUG
		perror("NightGetNode::cleanup(): ");
	#endif
	}
	_attrs->_url = _attrs->_path = _attrs->_password = _attrs->_login = "";
	return;
}

