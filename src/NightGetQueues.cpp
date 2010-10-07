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

#include <unistd.h>
#include <stdio.h>

#include <list>
using namespace std;

#include "NightGet.h"
#include "NightGetQueues.h"
#include "NightGetIterator.h"

struct NightGetSimpleQueue::QueueImpl {
	list<NightGetNode> _queue;
};

struct NightGetPriorityQueue::QueueImpl {
	list<NightGetNode> _queue;
	const Priority<NightGetNode>* _compare;
};

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

// Class NightGetSimpleQueue
NightGetSimpleQueue::NightGetSimpleQueue():
	_queuePtr( new QueueImpl ){}

NightGetSimpleQueue::~NightGetSimpleQueue(){
	if (_queuePtr) delete _queuePtr;
}

Iterator<NightGetNode>* NightGetSimpleQueue::do_CreateIterator(){
	return new NightGetSimpleQueueIterator(*this);
}

void NightGetSimpleQueue::do_shift(){
	if ( ! _queuePtr->_queue.empty() ) _queuePtr->_queue.pop_front();
}

void NightGetSimpleQueue::do_unshift(const NightGetNode& node){
	_queuePtr->_queue.push_front(node);
}

void NightGetSimpleQueue::do_pop(){
	if (! _queuePtr->_queue.empty() ) _queuePtr->_queue.pop_back();
}

void NightGetSimpleQueue::do_push(const NightGetNode& node){
	_queuePtr->_queue.push_back(node);
}

NightGetNode& NightGetSimpleQueue::do_front() const {
	// TODO exeption
	return _queuePtr->_queue.front();
}

NightGetNode& NightGetSimpleQueue::do_back() const {
	// TODO exeption
	return _queuePtr->_queue.back();
}

size_t NightGetSimpleQueue::do_size() const {
	return static_cast<size_t>( _queuePtr->_queue.size() );
}

void NightGetSimpleQueue::do_clear(){
	_queuePtr->_queue.clear();
}

bool NightGetSimpleQueue::do_empty() const {
	return _queuePtr->_queue.empty();
}

// Class NightGetPriorityQueue
// TODO copy & assign

NightGetPriorityQueue::NightGetPriorityQueue(const Priority<NightGetNode>& compare):
	_queuePtr( new QueueImpl ){

	_queuePtr->_compare = compare.clone();
}

NightGetPriorityQueue::~NightGetPriorityQueue(){
	if ( _queuePtr->_compare ) delete _queuePtr->_compare;
	if ( _queuePtr) delete _queuePtr;
}

Iterator<NightGetNode>* NightGetPriorityQueue::do_CreateIterator(){
	return new NightGetPriorityQueueIterator(*this);
}

void NightGetPriorityQueue::do_shift(){
	if ( ! _queuePtr->_queue.empty() ){
		_queuePtr->_queue.pop_front();
	}
}

void NightGetPriorityQueue::do_unshift(const NightGetNode& node){
	do_push(node);
}

void NightGetPriorityQueue::do_pop(){
	if (! _queuePtr->_queue.empty() ){
		_queuePtr->_queue.pop_back();
	}
}

void NightGetPriorityQueue::do_push(const NightGetNode& node){
	if ( _queuePtr->_queue.empty() ){
		_queuePtr->_queue.push_front( node );
	} else {
		list<NightGetNode>::iterator iter( _queuePtr->_queue.begin() );
		list<NightGetNode>::iterator end_iter( _queuePtr->_queue.end() );
		for (; iter != end_iter && _queuePtr->_compare->operator()( *iter,node ); ++iter);
		_queuePtr->_queue.insert(iter,node);
	}
}

NightGetNode& NightGetPriorityQueue::do_front() const {
	// TODO exeption
	return _queuePtr->_queue.front();
}

NightGetNode& NightGetPriorityQueue::do_back() const {
	// TODO exeption
	return _queuePtr->_queue.back();
}

size_t NightGetPriorityQueue::do_size() const {
	return static_cast<size_t>( _queuePtr->_queue.size() );
}

void NightGetPriorityQueue::do_clear(){
	_queuePtr->_queue.clear();
}

bool NightGetPriorityQueue::do_empty() const {
	return _queuePtr->_queue.empty();
}

void NightGetPriorityQueue::set_priority(const Priority<NightGetNode>& compare){
	_queuePtr->_compare = compare.clone();
}

void NightGetPriorityQueue::rebuild(){
	size_t size = do_size();
	while (size--){
		NightGetNode node = do_front();
		do_shift();
		do_push(node);
	}
}

