#include "stdafx.h"
#include "Song.h"


Song::Song(int num_parts)
{
	for (int i = 0; i < num_parts; i++) {
		lines.push_back(Line());
	}
	clefs.resize(num_parts);
	parts = num_parts;
}

Song::~Song(void)
{
}


void Song::AddMeasure(const Measure& measure, int part) {
	lines.at(part).push_back(measure);
}

void Song::Lilyprint(std::string * out) {
	char test[100];
	for (int i = 0; i < parts; i++) {
		sprintf_s(test, "part%d = \\new Voice {\n", i);
		out->append(test);
		for (int j = 0; j < lines.at(i).size(); j++) {
			lines.at(i).at(j).Lilyprint(out);
		}
		out->append("}\n\n");
	}
	out->append("\\score {\n  \\new StaffGroup <<\n");
	for (int i = 0; i < parts; i++) {
		sprintf_s(test, "    \\new Staff << \part%d >>\n", i);
		out->append(test);
	}
	out->append("  >>\n  \\layout {}\n    \\midi {}\n}");
}