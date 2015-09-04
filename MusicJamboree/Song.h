#pragma once

#include <vector>
#include "Measure.h"

#define Line std::vector<Measure>

enum Clef {
	BASS,
	TREBLE
};

class Song
{

public:
	int parts;
	std::vector<Line> lines;
	std::vector<Clef> clefs;

public:
	Song(int num_parts);
	~Song(void);
	void Lilyprint(std::string * out);
	void AddMeasure(const Measure& measure, int part);
};

