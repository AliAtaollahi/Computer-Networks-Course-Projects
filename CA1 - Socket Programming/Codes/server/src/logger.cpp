#include "logger.hpp"
#include "manual.hpp"

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

Logger::Logger()
{
}

void Logger::log(std::string roll, int fd, std::string command)
{
	ofstream log_file;
	log_file.open(Paths::LOG_CLUSTER_PATH, ios::app);
	time_t date = time(0);
	char* dt = ctime(&date);
	dt[strlen(dt) - 1] = '\0';
	log_file << '[' << dt << " " << roll << " with fd " << fd << "] " << command << endl;
	log_file.close();
}