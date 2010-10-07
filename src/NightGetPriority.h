#ifndef NIGHT_GET_PRIORITY
#define NIGHT_GET_PRIORITY

#include "NightGet.h"
template <class TPrior>
class Priority {
	public:
		inline bool operator()(const TPrior& lvalue,const TPrior& rvalue) const {return do_compare(lvalue,rvalue); }
		inline Priority<TPrior>* clone() const { return do_clone(); }
		virtual ~Priority(){}
	protected:
		Priority(){}
	private:
		Priority(const Priority&);
		Priority& operator=(const Priority&);
		virtual bool do_compare(const TPrior&,const TPrior&) const = 0;	
		virtual Priority<TPrior>* do_clone() const = 0;
};

#include "NightGetNodes.h"
class IdPriority: public Priority<NightGetNode> {
	public:
		IdPriority() {};
		virtual ~IdPriority(){};
	private:
		IdPriority(const IdPriority&);
		IdPriority& operator=(const IdPriority&);
		virtual bool do_compare(const NightGetNode& lv,const NightGetNode& rv) const {
			return ( lv.get_id() < rv.get_id() );
		};	
		virtual IdPriority* do_clone() const {
			return new IdPriority;
		};
};

#endif // NIGHT_GET_PRIORITY
