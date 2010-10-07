#ifndef NIGHT_GET_NODES
#define NIGHT_GET_NODES
#include <string>

#include "NightGet.h"
#include "NightGetDownload.h"

using namespace std;

class NightGetNode {
	public:
		NightGetNode(const NightGetDownloadMethod&);
		NightGetNode(const NightGetDownloadMethod& dmethod, const string& url, const string& path, const string& username="", const string& password="", const node_id& id=0);
		virtual ~NightGetNode();
		NightGetNode(const NightGetNode&);
		const NightGetNode& operator=(const NightGetNode&);

		inline void set_url(const string& url)		{ do_set_url(url); }
		inline void set_path(const string& path) 	{ do_set_path(path); }
		inline void set_id(const node_id& id) 		{ do_set_id(id); }
		inline void set_user(const string& username)	{ do_set_user(username); }
		inline void set_password(const string& pass)	{ do_set_password(pass); }

		inline const string& get_url() const 		{ return do_get_url(); }
		inline const string& get_path() const 		{ return do_get_path(); }
		inline const node_id& get_id() const		{ return do_get_id(); }
		inline const string& get_user() const 		{ return do_get_user(); }
		inline const string& get_password() const	{ return do_get_password(); }

		inline const state_t& state() const		{ return do_state(); }
		inline const state_t& download()		{ return do_download(); }
		inline void cleanup()				{ do_cleanup(); }


	private:
		NightGetNode();
		virtual void do_set_url(const string&);
		virtual void do_set_path(const string&);
		virtual void do_set_id(const node_id&);
		virtual void do_set_user(const string& );
		virtual void do_set_password(const string& );

		virtual const string& do_get_url() const;
		virtual const string& do_get_path() const;
		virtual const node_id& do_get_id() const;
		virtual const string& do_get_user() const;
		virtual const string& do_get_password() const;

		virtual const state_t& do_state() const;
		virtual const state_t& do_download();
		virtual void do_cleanup();

		struct Attrs;

		Attrs* _attrs;
};

#endif // NIGHT_GET_NODES
