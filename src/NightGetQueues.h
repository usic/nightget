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
#ifndef NIGHT_GET_QUEUES
#define NIGHT_GET_QUEUES

#include "NightGet.h"
#include "NightGetIterator.h"
#include "NightGetPriority.h"
#include "NightGetNodes.h"

template <class TNode>
class NightGetQueue {
	public:
		virtual ~NightGetQueue(){}
		inline Iterator<TNode>* CreateIterator() 	{ return do_CreateIterator(); }
		inline void shift() 				{ do_shift(); }
		inline void unshift(const TNode& node) 		{ do_unshift( node ); }
		inline void pop() 				{ do_pop(); }
		inline void push(const TNode& node) 		{ do_push(node); }
		inline TNode& front() const		{ return do_front(); }
		inline TNode& back() const		{ return do_back(); }
		inline size_t size() const 			{ return do_size(); }
		inline void clear() 				{ do_clear(); }
		inline bool empty() const			{ return do_empty(); }
		inline void change_priority(const Priority<NightGetNode>& pri)	
			{ do_change_priority(pri); }

	protected:
		NightGetQueue(){}

	private:
		NightGetQueue(const NightGetQueue<TNode>&){} // NI
		const NightGetQueue<TNode>& operator=(const NightGetQueue<TNode>&){} //NI

		virtual Iterator<TNode>* do_CreateIterator() = 0;
		virtual void do_shift() = 0;
		virtual void do_unshift(const TNode&) = 0;
		virtual void do_pop() = 0;
		virtual void do_push(const TNode&) = 0;
		virtual TNode& do_front() const = 0;
		virtual TNode& do_back() const = 0;
		virtual size_t do_size() const = 0;
		virtual void do_clear() = 0;
		virtual bool do_empty() const = 0;
		virtual void do_change_priority(const Priority<NightGetNode>&)=0;
};

class NightGetSimpleQueue: public NightGetQueue<NightGetNode> {
	public:
		NightGetSimpleQueue();
		~NightGetSimpleQueue();
	private:
		NightGetSimpleQueue(const NightGetSimpleQueue&);
		const NightGetSimpleQueue& operator=(const NightGetSimpleQueue&);

		virtual Iterator<NightGetNode>* do_CreateIterator();
		virtual void do_shift();
		virtual void do_unshift(const NightGetNode&);
		virtual void do_pop();
		virtual void do_push(const NightGetNode&);
		virtual NightGetNode& do_front() const;
		virtual NightGetNode& do_back() const;
		virtual size_t do_size() const;
		virtual void do_clear();
		virtual bool do_empty() const;
		virtual void do_rebuild(const Priority<NightGetNode>&) {};
		friend class NightGetSimpleQueueIterator;
		struct QueueImpl;

		QueueImpl* _queuePtr;
};

class NightGetPriorityQueue: public NightGetQueue<NightGetNode> {
	public:
		NightGetPriorityQueue(const Priority<NightGetNode>&);
		~NightGetPriorityQueue();
		virtual void set_priority(const Priority<NightGetNode>&);
		virtual void rebuild();
	private:
		NightGetPriorityQueue(const NightGetPriorityQueue&); // NIY
		const NightGetPriorityQueue& operator=(const NightGetPriorityQueue&); // NIY

		friend class NightGetPriorityQueueIterator;
		virtual Iterator<NightGetNode>* do_CreateIterator();
		virtual void do_shift();
		virtual void do_unshift(const NightGetNode&);
		virtual void do_pop();
		virtual void do_push(const NightGetNode&);
		virtual NightGetNode& do_front() const;
		virtual NightGetNode& do_back() const;
		virtual size_t do_size() const;
		virtual void do_clear();
		virtual bool do_empty() const;
		virtual void do_change_priority(const Priority<NightGetNode>& pri) {
			set_priority(pri);
			rebuild();
		};

		struct QueueImpl;

		QueueImpl* _queuePtr;
};

#endif // NIGHT_GET_QUEUES
