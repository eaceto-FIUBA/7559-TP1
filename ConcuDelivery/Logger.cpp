#include "Logger.h"

Logger* Logger::instance = NULL;
const string Logger::fileName = LOG_PATH + getDateTime() + LOG_EXTENSION;

Logger::Logger(): logFile ( (char*) fileName.c_str() ){

}

Logger* Logger::getInstance() {

	if ( instance == NULL )
                instance = new Logger();
	return instance;

}

void Logger::destroy () {

	if ( instance != NULL ) {
		Logger::getInstance()->log(logINFO, "Logger destroyed.");
		delete ( instance );
		instance = NULL;
	}

}

std::string Logger::getTag( LogLevel level ){

	switch(level){
		case logERROR:
			return "[ERROR]";
		case logDEBUG:
			return "[DEBUG]";
		case logINFO:
			return "[INFO]";
		case logWARNING:
			return "[WARNING]";
		default:
			return "";
	}
}

void Logger::log( LogLevel level, string message ){

	if (logFile.tomarLock() == 0) {

		std::string tag = getTag(level);

		stringstream s;
		s << tag << " " << getDateTime() << " Proceso-"<< getpid() << ": " << message << std::endl;

		string full_message(s.str());
		const char * msg_toSave = full_message.c_str();

		if (logFile.escribir(msg_toSave, strlen(msg_toSave)) == -1) {
				std::cerr << "Ha ocurrido un error al escibir el mensaje" << std::endl;
				std::cerr << "\"" << msg_toSave << "\"" << std::endl;
		}

		logFile.liberarLock();
    }
    else {
    	std::cerr << "Proceso: " << getpid() << ": Error al abrir el archivo: " << fileName << ". Mensaje: " << message << std::endl;
    }

}
