
//NEEDS
// 1. Salvar em arquivo texto

//----------------------------------------------------------------
/// @name VideoCopy
/// @author Andre Rosa
/// @date 24 AGO 2018
/// @detail copies videos from the source folder to the destiny folder according to the categories 
/// training.csv is the default file that contains the list of videos
//----------------------------------------------------------------

/*
INSTRUCTIONS
the parameter list:
1st - number of videos to copy, if you use 0 the program will copy all videos
2nd, 3rd.... the Categories to be found
You can also select negative arguments with a minus '-' like in -geek (all videos NOT gooek)

EXAMPLE FROM THE COMMAND PROMPT
   physically copy all videos with "geek" and "grade-9" categories ->     VideoCopy c 0 geek grade-9
   generate a text file with five videos with "grade-11" categories ->    VideoCopy t 5 grade-11
   physically coly 10 videos with "grade-7" and NOT terrorism ->          VideoCopy c 10 grade-7 -terrorism

THE ORIGIN AND DESTINATION FOLDER ARE DEFINED IN THE config.txt FILE in the program directory
*/

#include "video.h"

using namespace std;
using namespace vid;

//----------------------------------------------------------------
//parameters programName[0], quantity[1], categories[2...]
int main(int argc, char* argv[]) { 

	vid::Videos videoList;     //the CSV content will be read to this vector
	vid::CSV csv;

	cout << "----------------------------------------" << endl;
	cout << "|              VideoCopy               |" << endl;
	cout << "|        Seneca College - 2018         |" << endl;
	cout << "----------------------------------------" << endl << endl;

	string dirOrigin("");     //folder of files origin //.\\origin
	string dirDestiny("");    //folder of files destiny //.\\destiny

	readFolders(dirOrigin, dirDestiny);
	cout << "Original video folder:    " << dirOrigin << endl;
	cout << "Destination video folder: " << dirDestiny << endl << endl;

	int numVideos = atoi(argv[2]); //number of videos to copy to destiny
	char operation = *argv[1]; //this parameter can be 'c' to copy or 't' to text generation

	ifstream fs("training.csv", ifstream::out);  //CSV file to be read
	if (fs.is_open()) {
		cout << "File training.csv is open for reading." << endl;
	}

	//read the CSV and copies to csv object
	csv.readCSV(fs);
	fs.close();

	//csv.printCSV(); // print CSV, used only for code test

	//convert original CSV vector to the vector with only the necessary info
	videoList.convertFromCSV(csv.getCSV());

	//categories to find, got from the argv
	vector<string> catToFind;
	for (int i = 3; i < argc; i++) {  //the first 2 parameters are the program name and the quantity of files so we begin the loop with 2
		catToFind.push_back(argv[i]);
	}
	//videoList.setFirstFilter(argv[3]);
	//for testing
	//catToFind.clear();
	//catToFind.push_back("grade-7");

	//process vector to find categories
	videoList.filterVideosByCategories(catToFind);
	//videoList.printAllVideos();

	//copy number of videos ramdomly
	if (operation == 'c') {
		videoList.copyVideos(dirOrigin, dirDestiny, numVideos); //if numVideos = 0 copy all
	}
	if (operation == 'ct') {
		videoList.copyVideos(dirOrigin, dirDestiny, numVideos); //if numVideos = 0 copy all
	}
	else
	{
		//generate text file
		videoList.generateTxtReport(numVideos);
	}

	return 0;
}
//---------------------------------------------------
