#include "stdafx.h"
#include "Note.h"

//int value;
//int duration;
//int dots;
//bool tied;
//bool slurred;
//bool stacatto;
//bool stacked;
//bool flat;
//bool sharp;
//bool natural;

static const char NOTE_NAMES[8] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'c'};

Note::Note(int val, int dur, int dots = 0)
{
	value = val;
	duration = dur;
}


Note::~Note(void)
{

}

void PrintNoteName (int value, std::string * out) {
	int octaves = value / 8;
	int note = value % 8;
	if (note < 0) {
		note = note + 8;
	}
	out->push_back(NOTE_NAMES[note]);
	while(octaves < 0) {
		out->push_back(',');
		octaves += 1;
	}
	while(octaves > 0) {
		out->push_back('\'');
		octaves -= 1;
	}
}

void Note::Lilyprint (std::string * out) {
	char temp[5];
	PrintNoteName(this->value, out);
	sprintf_s(temp, "%d", this->duration);
	out->append(temp);
	for (int i = 0; i < dots; i++) {
		out->push_back('.');
	}
	out->push_back(' ');
}
