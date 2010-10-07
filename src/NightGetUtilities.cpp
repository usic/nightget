#include "NightGetUtilities.h"

int string2int(const string& str) {
	int i;
	istringstream ss(str);
	ss >> i;
	return i;
}
/*string int2string(int i) {
	ostringstream ss;
	ss << i;
	return ss.str();
}*/

#include <time.h>
string get_today() {
	time_t tm;
	time(&tm);
	struct tm* ltm = localtime(&tm);

	ostringstream ss;
	ss << (ltm->tm_year+1900) << '-' 
		<< (ltm->tm_mon+1) << '-'	//without leading zeros 
		<< ltm->tm_mday << ' '		//without leading zeros 
		<< ltm->tm_hour << ':'		//without leading zeros 
		<< ltm->tm_min << ':'		//without leading zeros 
		<< ltm->tm_sec;			//without leading zeros 
	return ss.str();
}

#include "syslog.h"
void log(const char* str) {
	openlog("NightGet:", LOG_PID | LOG_CONS, LOG_DAEMON);
	syslog(LOG_NOTICE, str);
	closelog();
}

