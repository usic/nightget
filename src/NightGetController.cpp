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
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "NightGetController.h"
#include <iostream>

struct megafunction_arg {
	megafunction_arg(const NightGetController& controller, NightGetQueue<NightGetNode>& queue,NightGetDatabase<NightGetNode,Converter,threads>& db):
		_controller(controller),
		_queue(queue),
		_database(db) {}
	const NightGetController& _controller;
	NightGetQueue<NightGetNode>& _queue;
	NightGetDatabase<NightGetNode,Converter,threads>& _database;
};

struct ExampleNightGet::ControllerImpl {
	NightGetDatabase<NightGetNode,Converter,threads>* _pdb;		//does not create/delete
	NightGetPriorityQueue* _pq;	//does create/delete

	unsigned int _threads;
	pthread_t* _childs;
	megafunction_arg* _arg;
	u_int32_t _state;
};

pthread_mutex_t ExampleNightGet::_mutex = PTHREAD_MUTEX_INITIALIZER;

ExampleNightGet::ExampleNightGet(const Priority<NightGetNode>& pri, NightGetDatabase<NightGetNode,Converter,threads>* db, unsigned int threads):
	_controller( new ControllerImpl ) {
	if ( db==NULL ) {
		throw NightGetException("creating controller without database?");
		return;
	}
	_controller->_pdb = db;
	_controller->_pq = new NightGetPriorityQueue(pri);
	_controller->_threads = threads;
	_controller->_childs = new pthread_t[ _controller->_threads];
	_controller->_arg = new megafunction_arg(*this, *(this->_controller->_pq),*(this->_controller->_pdb));
	_controller->_state = DENY_PROCESS;
}

ExampleNightGet::~ExampleNightGet(){
	//FIXME do all necessary checks
	delete _controller->_pq;
	delete [] _controller->_childs;
	delete _controller->_arg;
	delete _controller;
}

void ExampleNightGet::do_addToQueue(const NightGetNode& node) {
	_controller->_pq->push(node);
}

void ExampleNightGet::do_setPriority(const Priority<NightGetNode>& pri) {
	_controller->_pq->change_priority(pri);
}

int ExampleNightGet::do_startNightGet(const int& start_time, const int& end_time) {
	log("NightGet starts");
	allow();
	if ( _controller->_pq->empty() ) return EXIT_SUCCESS;
	unsigned int count_threads = _controller->_threads;
	while ( count_threads-- ){
		if ( pthread_create( _controller->_childs + count_threads, NULL, &megafunction, _controller->_arg ) != 0) {
			perror("ExampleNightGet::do_StartNightGet(): ");
			throw NightGetException("NightGetController couldn't create threads");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int ExampleNightGet::do_saveToDB() const {
	try { 
		if (_controller->_pq->empty()) return 0;
		while (!_controller->_pq->empty()) {
			this->do_saveToDB(_controller->_pq->back());
			_controller->_pq->pop();
		}
		return 0;
	} catch(DBException& e) {
		throw; 	//FIXME serialize etc.
		return -1;
	} catch(NightGetException& e) { //FIXME
		return -1;
	}
}

int ExampleNightGet::do_saveToDB(const NightGetNode& data) const {
	return _controller->_pdb->save(data);
}

int ExampleNightGet::do_endNightGet() {
	if ( state() == DENY_PROCESS ) return -1;
	deny();
	int error;
	unsigned int success_counter = 0;
 	unsigned int *tmp_counter = 0;
	for(unsigned int in=0; in < _controller->_threads; ++in){
 		if ( (error = pthread_join( _controller->_childs[in],(void**)&tmp_counter )) != 0 )
			throw ThreadException( strerror(error) );
 		success_counter += *tmp_counter;
	}
	return success_counter;	//implicit conversion to int
}

void ExampleNightGet::do_buildQueue() {
	try {
		list<shared_ptr<NightGetNode> > plist = _controller->_pdb->getObjects();
		list<shared_ptr<NightGetNode> >::iterator plist_it1 = plist.begin();
		list<shared_ptr<NightGetNode> >::iterator plist_it2 = plist.end();

		for (size_t iter = 0; plist_it1 != plist_it2; ++plist_it1, ++iter)
			do_addToQueue( *(*plist_it1) );
	} catch (DBException& e) {
		log(e.what());
		return;
	} catch (std::exception& e) {
		_controller->_pq->clear();
		throw;
	}
}

u_int32_t ExampleNightGet::do_state() const {
	return _controller->_state;
}

void ExampleNightGet::allow() {
	_controller->_state = ALLOW_PROCESS;
}

void ExampleNightGet::deny(){
	_controller->_state = DENY_PROCESS;
}

void* megafunction(void* _arg) {
	// _arg holds megafunction_arg
	megafunction_arg* arg = static_cast<megafunction_arg*>(_arg);
	pthread_mutex_lock(&ExampleNightGet::_mutex);
	arg->_database.startThread();
	pthread_mutex_unlock(&ExampleNightGet::_mutex);
	unsigned int _counter = 0;

// TODO: use of pthread_cond* + checking locking of mutex 
// 	state_t _node_state = NG_EMPTY;
// 	while ( arg->_controller.state() == ALLOW_PROCESS && ! _cursor->is_last() ){
	while (true) {
		pthread_mutex_lock(&ExampleNightGet::_mutex);
		if ( arg->_controller.state() == ALLOW_PROCESS && !arg->_queue.empty() ) {
			NightGetNode node = arg->_queue.front();
			arg->_queue.shift();
			pthread_mutex_unlock(&ExampleNightGet::_mutex);	
			if (  node.state() == NG_INIT ) {
				if ( node.download() == NG_FINISHED ) ++_counter;
				pthread_mutex_lock(&ExampleNightGet::_mutex);
				arg->_controller.saveToDB(node);		// TODO: chg to serializing or smth
				pthread_mutex_unlock(&ExampleNightGet::_mutex);
			}
		} else {
			pthread_mutex_unlock(&ExampleNightGet::_mutex);
			break;
		}
	}
	arg->_database.endThread();
	std::cerr << "\tcounter in thread = " << _counter<<std::endl;	// XXX ?
	pthread_exit(&_counter) ;
	// never survive here
	return (void*)_counter;
}
