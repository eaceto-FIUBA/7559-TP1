#include "Utils.h"

std::string getDateTime(){

	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	std::string dateTime = asctime(timeinfo);

	//Replace space for underscore
	std::replace( dateTime.begin(), dateTime.end(), ' ', '_');

	//Remove newline character from string
	dateTime.erase(std::remove(dateTime.begin(), dateTime.end(), '\n'), dateTime.end());

	return dateTime;

}
