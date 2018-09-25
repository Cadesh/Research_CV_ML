#include <iostream>
#include <string>

using namespace std;

// from https://trac.ffmpeg.org/wiki/Create%20a%20thumbnail%20image%20every%20X%20seconds%20of%20the%20video
//this code executes a ffmpeg system comand
int main()
{
    cout << "System Command Test" << endl;

       //for a video of 363.74 seconds generates 100 frames
       if(system("ffmpeg -i videotest.mp4 -vf fps=1/3.6374 frame/img%03d.jpg") != 0)
       {
          cout << "ffmpeg failed..." << endl;
       }

    //Extra commands

    //command to get a specific time
    //if(system("ffmpeg -i videotest.mp4 -ss 00:00:14.435 -vframes 1 out.jpg") != 0)

    //output one extra image every second named out1.jpg, out2.jpg, ....
    //"ffmpeg -i videotest.mp4 -vf fps=1 out%d.png"

    //Output one image every second named img001.jpg, img002.jpg, ....
    //"ffmpeg -i videotest.mp4 -vf fps=1/60 img%03d.jpg"

    //Output one image every 10 minutes
    //"ffmpeg -i videotest.mp4 -vf fps=1/600 img%04d.jpg"

    return 0;
}
