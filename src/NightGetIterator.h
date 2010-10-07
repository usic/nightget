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

#ifndef NIGHT_GET_ITERATOR
#define NIGHT_GET_ITERATOR

#include "NightGet.h"
// TODO add copy & assign & compare

/* note: implementation in NightGetQueues.cpp */
template <class TItem>
class Iterator {
	public:
		inline void next()		{ do_next(); }
		inline void prev()		{ do_prev(); }
		inline bool is_first() const	{ return do_is_first(); }
		inline bool is_last() const 	{ return do_is_last();}
		inline TItem& current() const 	{ return do_current(); }
		virtual ~Iterator(){}
	protected:
		Iterator(){}
	private:
		virtual void do_next() = 0;
		virtual void do_prev() = 0;
		virtual bool do_is_first() const = 0;
		virtual bool do_is_last() const = 0;
		virtual TItem& do_current() const = 0;
};


class NightGetSimpleQueueIterator: public Iterator<NightGetNode> {
public:
	const NightGetSimpleQueueIterator& operator=(const NightGetSimpleQueueIterator&);
	~NightGetSimpleQueueIterator();
private:
	NightGetSimpleQueueIterator(const NightGetSimpleQueue&);
	
	NightGetSimpleQueueIterator(const NightGetSimpleQueueIterator&);
	virtual void do_next();
	virtual void do_prev();
	virtual bool do_is_first() const;
	virtual bool do_is_last() const;
		
	virtual NightGetNode& do_current() const;
	
	friend class NightGetSimpleQueue;
	struct IteratorImpl;
	
	IteratorImpl* _iterPtr;
};

class NightGetPriorityQueueIterator: public Iterator<NightGetNode> {
public:
	~NightGetPriorityQueueIterator();
	const NightGetPriorityQueueIterator& operator=(const NightGetPriorityQueueIterator&);
private:
	NightGetPriorityQueueIterator(const NightGetPriorityQueue&);
	NightGetPriorityQueueIterator(const NightGetPriorityQueueIterator&);

	virtual void do_next();
	virtual void do_prev();
	virtual bool do_is_first() const;
	virtual bool do_is_last() const;
	virtual NightGetNode& do_current() const;

	friend class NightGetPriorityQueue;
	struct IteratorImpl;

	IteratorImpl* _iterPtr;
};

#endif // NIGHT_GET_ITERATOR
