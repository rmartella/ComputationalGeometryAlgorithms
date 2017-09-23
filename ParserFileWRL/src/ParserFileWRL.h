/*
 * ParserFileWRL.h
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#ifndef PARSERFILEWRL_H_
#define PARSERFILEWRL_H_

#include "definition.h"

namespace justin {

class ParserFileWRL {
public:
	ParserFileWRL(const char * file);
	virtual ~ParserFileWRL();
	std::vector<Polygon> * parserFile();
private:
	std::ifstream * fileOpen;
};

} /* namespace justin */

#endif /* PARSERFILEWRL_H_ */
