#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Utils.h"
#include "LockFile.h"

enum LogLevel {logERROR, logWARNING, logINFO, logDEBUG};

using namespace std;

class Logger {


	private:

		static Logger* instance;
		static const string fileName;
		LockFile logFile;
		Logger();
		std::string getTag( LogLevel level );
		static bool debugMode;

	public:

		static Logger* getInstance();
		static void destroy ();
		void log( LogLevel level, string message );
		void setLogLevel(bool debug);

};


#endif //LOGGER_H
