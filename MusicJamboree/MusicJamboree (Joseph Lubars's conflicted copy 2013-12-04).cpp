#include "stdafx.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 const char* filename = argc >= 2 ? argv[1] : "pic1.jpg";
 Mat src = imread(filename, 0);
 if(src.empty())
 {
     cout << "can not open " << filename << endl;
     return -1;
 }

 Mat bw, dest, imbw;
 threshold(src, bw, 0, 1, THRESH_BINARY_INV | THRESH_OTSU); //Convert to BW image using Otsu
 threshold(src, imbw, 0, 255, THRESH_BINARY | THRESH_OTSU);
 Mat rowsums, colsums;
 int rows = bw.size[0];
 int cols = bw.size[1];
 reduce(bw, rowsums, 1, CV_REDUCE_SUM, CV_32SC1);
 reduce(bw, colsums, 0, CV_REDUCE_SUM, CV_32SC1);

 Mat rowsums2, colsums2, colsums3;
 normalize(colsums, colsums2, 0, 255, NORM_MINMAX, CV_8UC1);
 threshold(colsums2, colsums3, 5, 1, THRESH_BINARY);

 int colthresh = sum(colsums3)[0] * 8 / 10;
 
 compare(rowsums, colthresh, rowsums2, CMP_GT); //Now, rows with a staff line have the value 255
 
 // The following few lines just print out the lines in red on the picture and display it
 //cvtColor(src, dest, CV_GRAY2RGB);

 //for(int i = 0; i < rows; i++) {
	// if (rowsums2.at<unsigned char>(i, 0) == 255) {
	//	 line(dest, Point2d(0, i), Point2d(cols - 1, i), Scalar(0, 0, 255));
	// }
 //}

 //namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
 //imshow( "Hough Circle Transform Demo", dest );
 //waitKey();
 // End display code

 // The next step is to find staff line spacing:

 Vector<int> staffrows;
 Vector<float> avgrows;
 int first = -1;
 int last = -1;

 for (int i = 0; i < rows; i++) {
	 if (rowsums2.at<unsigned char>(i, 0) == 255) {
		 staffrows.push_back(i);
		 if (i - last != 1) {
			 if (first != -1) {
				 avgrows.push_back(((float)last + (float)first) / 2.0);
			 }
			 first = i;
		 }
		 last = i;
	 }
 }
 if (first != -1) {
	avgrows.push_back(((float)last + (float)first) / 2.0);
 }

 // Now staffrows has all the rows with staff lines and avgrows has the center of these lines.

 if (avgrows.size() % 5 != 0) {
	 cout << "An error might have occured in staff line detection.";
 }
 if (avgrows.size() < 5) {
	 cout << "No staves were detected, exiting now.";
	 return -1;
 }

 // Let's calculate average staff distance as the average between the first 5 lines
 // aka distance from line 1 to line 5 divided by 4

 float avgStaffDistance = (avgrows[4] - avgrows[0]) / 4.0;

 cout << "Average staff distance is " << avgStaffDistance << ".\n";

 int stavesInGroup = 1;
 int barLineIndex = 5;

 while (barLineIndex < avgrows.size()) {
	 Mat lilColSums;
	 int found = 0;
	 int startrow = (int)(avgrows[barLineIndex - 1] + avgStaffDistance);
	 int endrow = (int)(avgrows[barLineIndex] - avgStaffDistance);
	 int target = (endrow - startrow) * 8 / 10;
	 Mat testMat = bw.rowRange(startrow, endrow);
	 reduce(testMat, lilColSums, 0, CV_REDUCE_SUM, CV_32SC1);
	 for (int i = 0; i < cols; i++) {
		 if (lilColSums.at<int>(0, i) > target) {
			 found = 1;
			 break;
		 }
	 }
	 if (!found) {
		 break;
	 }
	 stavesInGroup += 1;
	 barLineIndex += 5;
 }

 // We find groups of staves (staves played together at the same time) by looking for vertical lines
 // joining adjacent staves.  The number of staves in the first group is stored in "stavesInGroup".
 // Right now we assume every group in the piece of music has the same number of staves.

 cout << "Staves in group: " << stavesInGroup << "\n";
 cout << "Total staves: " << avgrows.size() / 5 << "\n";

 if (((avgrows.size() / 5 ) % stavesInGroup) != 0) {
	 cout << "Bad number of stave groups, incompatible with total number of staves.  Exiting.";
	 return -1;
 }

 barLineIndex = 0; 
 int group = 0;

 while(barLineIndex < avgrows.size()) {
	 int startrow = (int)(avgrows[barLineIndex] - 3 * avgStaffDistance);
	 int endrow = (int)(avgrows[barLineIndex + 4] + 3 * avgStaffDistance);
	 if (startrow < 0) {
		 startrow = 0;
	 }
	 if (endrow > rows) {
		 endrow = rows;
	 }
	 Mat testMat = imbw.rowRange(startrow, endrow);

	 // Anurag TODO: Here is the matrix with the line.
	 // avgStaffDistance has the average distance between lines in a staff.
	 // avgrows is a vector with the approximate centers of all the staff lines.
	 // avgrows[barLineIndex - barLineIndex+4] will have the centers of the region you're processing.
	 // staffrows is like avgrows, but it has all the rows considered to be staff lines, in case
	 //   staff lines are more than a pixel thick.  You can do some magic to find all the lines you need
	 //   to erase.
	 // stavesInGroup has the number of staves grouped together, group has the current group index.


	 // The next 3 lines show you an easy way to display an image using opencv. 

	 namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
	 imshow( "Hough Circle Transform Demo", testMat );
	 waitKey();

	 barLineIndex += 5;
	 group = (group + 1) % stavesInGroup;
 }

 return 0;
}