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

#ifndef NIGHT_GET_DOWNL
#define NIGHT_GET_DOWNL
#include <stdio.h>
#include <string>
using namespace std;
#include "NightGet.h"

class NightGetDownloadMethod {
public:
	inline const state_t download(const NightGetNode& node) 	{return do_download(node); }
	inline NightGetDownloadMethod* clone() const		{return do_clone(); }
	virtual	~NightGetDownloadMethod(){}
protected:
	NightGetDownloadMethod(){}
private:
	virtual const state_t do_download(const NightGetNode&) = 0;
	virtual NightGetDownloadMethod* do_clone() const = 0;
};

class NightGetCurlInt {
public:
	NightGetCurlInt(const string& url, FILE* stream, const string& userpwd = "");
	~NightGetCurlInt();
	void setopt(const string& url, FILE* stream, const string& userpwd = "");
	int perform();
private:
	static unsigned long _global_inits;
	struct HandlerImpl;
	HandlerImpl* _hPtr;
};

class NightGetFTP: public NightGetDownloadMethod {
public:
	NightGetFTP(){}
	~NightGetFTP(){}
private:
	virtual const state_t do_download(const NightGetNode& node);
	virtual NightGetFTP* do_clone() const;

};

class NightGetHTTP: public NightGetDownloadMethod {
public:
	NightGetHTTP(){}
	~NightGetHTTP(){}
private:
	virtual const state_t do_download(const NightGetNode& node);
	virtual NightGetHTTP* do_clone() const;

};

#endif // NIGHT_GET_DOWNL
