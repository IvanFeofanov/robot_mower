#include "flag.h"

Flag::Flag() :
	state_(false)
{
}

Flag::Flag(bool state) :
	state_(state)
{
}

bool Flag::is_set()
{
	return state_;
}

void Flag::set()
{
	state_ = true;
}

bool Flag::check()
{
	bool tmp = state_;
	state_ = false;
	return tmp;
}
