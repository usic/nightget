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
#ifndef NIGHT_GET_CONTROLLER
#define NIGHT_GET_CONTROLLER

#include <pthread.h>
#include "NightGet.h"
#include "NightGetQueues.h"
#include "NightGetNodes.h"
#include "NightGetPriority.h"
#include "NightGetDatabase.h"
#include "NightGetException.h"
#include "NightGetUtilities.h"

template<typename Object>
class Converter;

#define ALLOW_PROCESS 0x0
#define DENY_PROCESS 0x1

struct megafunction_arg;

// return amount of node processed
void* megafunction( void* );

class NightGetController {
public:
	NightGetController() {}
	virtual ~NightGetController() {}

	inline void addToQueue(const NightGetNode& node)		{ do_addToQueue(node);}
	inline void setPriority(const Priority<NightGetNode>& pri) 	{do_setPriority(pri); }
	inline int startNightGet(const int& start_time=0, const int& end_time=0)
		{return do_startNightGet(start_time,end_time);}
	inline int saveToDB() const 					{ return do_saveToDB();}
	inline int saveToDB(const NightGetNode& node) const 		{ return do_saveToDB(node);}
	inline int endNightGet() 					{ return do_endNightGet();}
	inline u_int32_t state() const					{ return do_state(); }
	inline void buildQueue()					{ do_buildQueue(); }
protected:

private:
	virtual void do_addToQueue(const NightGetNode& node)=0;
	virtual void do_setPriority(const Priority<NightGetNode>&)=0;
	
	virtual int do_startNightGet(const int& start_time=0, const int& end_time=0)=0;
	virtual int do_saveToDB() const=0;
	virtual int do_saveToDB(const NightGetNode&) const=0;
	virtual int do_endNightGet()=0;
	
	virtual u_int32_t do_state() const = 0;	
	virtual void do_buildQueue()=0;

	NightGetController(const NightGetController&);
	const NightGetController& operator=(const NightGetController&);
};


class ExampleNightGet: public NightGetController {
public:
	ExampleNightGet(const Priority<NightGetNode>& pri, NightGetDatabase<NightGetNode,Converter,threads>* db, unsigned int threads=1);
	virtual ~ExampleNightGet();

	static pthread_mutex_t _mutex;

private:
	virtual void do_addToQueue(const NightGetNode& node);
	virtual void do_setPriority(const Priority<NightGetNode>&);
	
	virtual int do_startNightGet(const int& start_time=0, const int& end_time=0);
	virtual int do_saveToDB() const;
	virtual int do_saveToDB(const NightGetNode&) const;
	virtual int do_endNightGet();
	
	virtual void allow();
	virtual void deny();
	virtual u_int32_t do_state() const;
	virtual void do_buildQueue();

	struct ControllerImpl;
	ControllerImpl* _controller;
	
	ExampleNightGet();
	ExampleNightGet(const ExampleNightGet&);
	const ExampleNightGet& operator=(const ExampleNightGet&);
};

#endif // NIGHT_GET_CONTROLLER
