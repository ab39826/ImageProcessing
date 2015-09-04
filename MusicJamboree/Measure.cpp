#include "stdafx.h"
#include "Measure.h"

using namespace std;
using namespace cv;


Measure::Measure(void)
{
	timenum = 0;
	timeden = 0;
	endbar = SINGLE_BAR;
	//notes = vector<Note>();
	cout << "Measure constructed";
}

Measure::~Measure(void)
{
	cout << "Measure destroyed";
}

void Measure::AddNote (const Note& newnote) {
	notes.push_back(newnote);
}

void Measure::Lilyprint (string * out) {
	out->append("  ");
	for (int i = 0; i < notes.size(); i++) {
		notes.at(i).Lilyprint(out);
	}
	switch(endbar) {
	case DOUBLE_BAR:
		out->append("\\bar \"||\"");
		break;
	case FINISH:
		out->append("\\bar \"|.\"");
		break;
	case REPEAT_START:
		out->append("\\bar \"|:\"");
		break;
	case REPEAT_END:
		out->append("\\bar \":|\"");
		break;
	}
	out->push_back('\n');
}