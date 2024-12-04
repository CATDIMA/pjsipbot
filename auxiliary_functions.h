#ifndef PJSIPBOT_FUNCTIONS_H
#define PJSIPBOT_FUNCTIONS_H

#include <string>
#include <fstream>

const std::string recordsPath{"./records/"};

std::string getUniqueFilename(void);
void readConfLine(std::fstream&, std::string&);
void readConfLine(std::fstream&, int&);

#endif