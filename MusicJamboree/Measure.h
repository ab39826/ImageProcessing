#pragma once

#include "Note.h"
#include <vector>

//using namespace std;

enum BarlineStyle {
	SINGLE_BAR,
	DOUBLE_BAR,
	REPEAT_START,
	REPEAT_END,
	FINISH
};

class Measure
{
public:
	int timenum;
	int timeden;
	BarlineStyle endbar;
	std::vector<Note> notes;
	
public:
	Measure(void);
	~Measure(void);
	void AddNote(const Note& newnote);
	void Lilyprint(std::string * out);
};


