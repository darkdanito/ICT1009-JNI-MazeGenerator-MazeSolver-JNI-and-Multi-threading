#include "Algorithm.h"

using namespace std;

#ifndef HUNT_KILL_H
#define HUNT_KILL_H

class HuntKill : public Algorithm 
{
	private:																	// Functions Constructors - for own class usage
		void HuntKill::hunt();
		void HuntKill::kill(size_t first, size_t second);
	
	public:																		// Functions Constructors 
		ERR_ENUM generate();
};
#endif