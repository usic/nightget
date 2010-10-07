// #define NDEBUG

#include "NightGet.h"
// #include "NightGetController.h"
#include "NightGetDatabase.h"
#include "NightGetDBImpl.h"
int main(int argc, char** argv)
{
try {
	NightGetDatabase<NightGetNode, Converter, threads>* ngdb = new MySQLNightGetDatabase<NightGetNode, Converter, threads>;
	IdPriority priority;
	ExampleNightGet _ctrl (priority,ngdb,1);
	_ctrl.buildQueue();

	if ( _ctrl.startNightGet() ) {
		log("NightGet couldn't start");
		delete ngdb;
		return EXIT_FAILURE;
	}

	int downloads = 0;

	((downloads = _ctrl.endNightGet()) >= 0) ? log("NightGet ends successfully") : log("NightGet ends with an error");
#ifndef NDEBUG
	cerr << "downloads = " << downloads << endl;
#endif
	delete ngdb;
	return EXIT_SUCCESS;

} catch (ThreadException& e) {
#ifndef NDEBUG
	cerr << e.what() << endl;
#endif
	log(e.what());
} catch (std::exception& e) {
	perror(e.what());
	log(e.what());
	_exit(EXIT_FAILURE);
}
}
