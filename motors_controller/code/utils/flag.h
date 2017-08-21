#ifndef FLAG_H
#define FLAG_H

class Flag
{
public:
	Flag();
	Flag(bool state);
	
	bool is_set();
	void set();
	bool check();

private:
	bool state_;
};

#endif