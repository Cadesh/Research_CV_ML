#include "opencv2/opencv.hpp"
#include <fstream>


using namespace cv;
using namespace std;

int main()
{

// Open video file
VideoCapture video("videotest.mp4");

double fps = video.get(CV_CAP_PROP_FPS);
double nframes = video.get(CAP_PROP_FRAME_COUNT);

cout << "Frames per second using video.get(CV_CAP_PROP_FPS) : " << fps << endl;
cout << "Frames count  : " << nframes << endl;
ofstream myfile;
myfile.open ("example.txt");

for (int i=0;i<nframes;i++)

{


myfile<< "Frame Number=  "<<";"<< i<< endl;

}
myfile.close();
video.release();
return 0;

}
