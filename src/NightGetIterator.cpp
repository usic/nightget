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

#include <list>

#include "NightGet.h"
#include "NightGetIterator.h"
#include "NightGetQueues.h"

/* This file is not needed: implementation in NightGetQueues.cpp */

struct NightGetSimpleQueueIterator::IteratorImpl {
	const NightGetSimpleQueue *_listPtr;
	list<NightGetNode>::iterator _current;
};

struct NightGetPriorityQueueIterator::IteratorImpl {
	const NightGetPriorityQueue* _listPtr;
	list<NightGetNode>::iterator _current;
};

// Class NightGetSimpleQueueIterator
// TODO comparison
NightGetSimpleQueueIterator::NightGetSimpleQueueIterator(const NightGetSimpleQueue& queue):
	_iterPtr( new NightGetSimpleQueueIterator::IteratorImpl ){
	
	_iterPtr->_listPtr = &queue;
	_iterPtr->_current = queue._queuePtr->_queue.begin();
}

NightGetSimpleQueueIterator::~NightGetSimpleQueueIterator(){
	if (_iterPtr) delete _iterPtr;
}

NightGetSimpleQueueIterator::NightGetSimpleQueueIterator(const NightGetSimpleQueueIterator& iterator):
	_iterPtr( new NightGetSimpleQueueIterator::IteratorImpl ){
	
	_iterPtr->_listPtr = iterator._iterPtr->_listPtr;
	_iterPtr->_current = iterator._iterPtr->_current;
}

const NightGetSimpleQueueIterator& NightGetSimpleQueueIterator::operator=(const NightGetSimpleQueueIterator& iterator){
	if (this != &iterator){
		_iterPtr->_listPtr = iterator._iterPtr->_listPtr;
		_iterPtr->_current = iterator._iterPtr->_current;
	}
	return *this;
}

void NightGetSimpleQueueIterator::do_prev(){
	if ( _iterPtr->_current != _iterPtr->_listPtr->_queuePtr->_queue.begin() ) 
		--( _iterPtr->_current);
}

void NightGetSimpleQueueIterator::do_next(){
	if ( _iterPtr->_current != _iterPtr->_listPtr->_queuePtr->_queue.end()) 
		++( _iterPtr->_current);
}

NightGetNode& NightGetSimpleQueueIterator::do_current() const {
	return *( _iterPtr->_current );
}

bool NightGetSimpleQueueIterator::do_is_first() const {
	return _iterPtr->_current == _iterPtr->_listPtr->_queuePtr->_queue.begin();
}

bool NightGetSimpleQueueIterator::do_is_last() const {
	return _iterPtr->_current == _iterPtr->_listPtr->_queuePtr->_queue.end();
}

// Class NightGetPriorityQueueIterator
NightGetPriorityQueueIterator::NightGetPriorityQueueIterator(const NightGetPriorityQueue& queue):
	_iterPtr( new IteratorImpl ) {

	_iterPtr->_listPtr = &queue;
	_iterPtr->_current = queue._queuePtr->_queue.begin();
}

NightGetPriorityQueueIterator::~NightGetPriorityQueueIterator(){
	if (_iterPtr) delete _iterPtr;
}

NightGetPriorityQueueIterator::NightGetPriorityQueueIterator(const NightGetPriorityQueueIterator& iterator):
	_iterPtr(new IteratorImpl ){

	_iterPtr->_listPtr = iterator._iterPtr->_listPtr;
	_iterPtr->_current = iterator._iterPtr->_current;
}

const NightGetPriorityQueueIterator& NightGetPriorityQueueIterator::operator=(const NightGetPriorityQueueIterator& iterator){
	if (this != &iterator){
		_iterPtr->_listPtr = iterator._iterPtr->_listPtr;
		_iterPtr->_current = iterator._iterPtr->_current;
	}
	return *this;
}

void NightGetPriorityQueueIterator::do_next(){
	if ( _iterPtr->_current != _iterPtr->_listPtr->_queuePtr->_queue.end() ) ++_iterPtr->_current;
}

void NightGetPriorityQueueIterator::do_prev(){
	if ( _iterPtr->_current != _iterPtr->_listPtr->_queuePtr->_queue.begin() ) --_iterPtr->_current;
}

NightGetNode& NightGetPriorityQueueIterator::do_current() const {
	return *(_iterPtr->_current);
}

bool NightGetPriorityQueueIterator::do_is_first() const {
	return _iterPtr->_current == _iterPtr->_listPtr->_queuePtr->_queue.begin();
}

bool NightGetPriorityQueueIterator::do_is_last() const {
	return _iterPtr->_current == _iterPtr->_listPtr->_queuePtr->_queue.end();
}
