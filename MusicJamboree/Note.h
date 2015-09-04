#pragma once
class Note
{
public:
	int value;
	int duration;
	int dots;
	bool tied;
	bool slurred;
	bool stacatto;
	bool stacked;
	bool flat;
	bool sharp;
	bool natural;
public:
	Note(int val, int dur);
	Note(int val, int dur, int dots);
	~Note(void);
	void Lilyprint(std::string * out);
};

