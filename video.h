#ifndef VIDEO_H
#define VIDEO_H

//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> //remove
#include <cstdlib> //random
#include <ctime> //random
#include <sstream> //isstringstream

using namespace std;

namespace vid {
	//----------------------------------------------------------------
	void readFolders(string &dirOrigin, string &dirDestiny) {
		std::string line;
		std::ifstream infile("config.txt");

		getline(infile, line);
		dirOrigin = line;

		getline(infile, line);
		dirDestiny = line;

		return;
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	class VideoFile {
		string fileName;
		vector<string> vecCategories;
	public:
		void setName(const string & name) { this->fileName = name; }
		string getName() { return this->fileName; }
		void setCategories(const vector<string>& v) { this->vecCategories = v; }

		//------------------------------------------------------
		///@detail check the category of the video, if video belongs to this category returns true
		//------------------------------------------------------
		bool checkCategory(const string &category) {
			bool checked = false;
			bool isNOT = false;
			string cat = category;

			//check if it is NOT category
			if (cat.at(0) == '-') {
				cat.erase(0,1);
				isNOT = true;
			}

			//check if it is NOT in category
			if (isNOT == false) {
				for (size_t i = 0; i < this->vecCategories.size(); i++) {
					if (this->vecCategories[i] == cat) {
						checked = true;
						i = this->vecCategories.size();
					}
				}
			}
			else { //isNOT == true
				checked = true;
				for (size_t i = 0; i < this->vecCategories.size(); i++) {
					if (this->vecCategories[i] == cat) {
						checked = false;
						i = this->vecCategories.size();
					}
				}
			}

			return checked;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		void printVideoData() {
			cout << "video " << ": " << this->getName() << endl;
			cout << "  categories: ";
			for (auto row : this->vecCategories)
				cout << row << " ";

			cout << "\n\n\n";
		}

		bool Copy(const string &origin, const string &destiny) {

			string lOrig = origin + "\\" + this->getName() + ".mp4";
			if (this->fileExist(lOrig) == true) {
				string lDest = destiny + "\\" + this->getName() + ".mp4";
				std::ifstream  src(lOrig, std::ios::binary);
				std::ofstream  dst(lDest, std::ios::binary);
				//cout << "++ video copied:    " << lDest << endl;
				return true;
			}
			else {
				cout << "-- video not found: " << lOrig << endl;
				return false;
			}
		}

		bool fileExist(const string& filePath)
		{
			std::ifstream infile(filePath);
			return infile.good();
		}

	}; //end of VideoFile class
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	///@detail A container for the list of videos
	//----------------------------------------------------------------
	class Videos {
		vector<VideoFile> vecOriginal; ///initial list of videos, this list is updated after each filtering
		string fileName; ///name of the file to save videos
		vector <size_t> rdNum; ///list of random videos index 
	public:
		//void setFirstFilter(const string &str) { this->firstFilter = str; }
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		void convertFromCSV(vector<vector<string>>& csv) {
			cout << "Start converting .csv data.";
			for (size_t i = 1; i < csv.size(); i++) {
				VideoFile locVid;
				locVid.setName(csv[i][0]); //position 0 is the name of the file
				//new we have a problem the 14 field of the CSV has "" between words therefore we are going to explode it
				//cout << csv[i][0] << endl;
				vector<string> v = this->SplitStrings(csv[i][1], ','); //position 14 is the list of categories
				locVid.setCategories(v);
				this->vecOriginal.push_back(locVid);
				//cout << "row: " << i << endl;
				if ((i % 500) == 0) { cout << "."; }
			}
			cout << endl;
		}
		//-----------------------------------------------------------------

		//-----------------------------------------------------------------
		///@detail split a string into an vector of strings suing a separator
		///@param string to be split (s), separator (c)
		//-----------------------------------------------------------------
		vector<string> SplitStrings(string &s, const char& c) {
			string buff{ "" };
			vector<string> v;

			//erase the unecesary charaters from the category list, these -> [ ] "
			s.erase(remove(s.begin(), s.end(), '"'), s.end());
			s.erase(remove(s.begin(), s.end(), '['), s.end());
			s.erase(remove(s.begin(), s.end(), ']'), s.end());

			for (auto n : s)
			{
				if (n != c) buff += n; else
					if (n == c && buff != "") { v.push_back(buff); buff = ""; }
			}
			if (buff != "") v.push_back(buff);
			return v;
		}
		//------------------------------------------------------

		//------------------------------------------------------
		///@detail Prints all videos in the original list
		//------------------------------------------------------
		void printAllVideos() {
			for (size_t i = 0; i < this->vecOriginal.size(); i++)
				this->vecOriginal[i].printVideoData();
		}
		//------------------------------------------------------
		//------------------------------------------------------
		void filterVideo(const string & category) {
			vector<VideoFile> vecResult;

			for (size_t i = 0; i < this->vecOriginal.size(); i++) {
				if (this->vecOriginal[i].checkCategory(category) == true) {
					vecResult.push_back(vecOriginal[i]);
				}
			}
			//after the last one the original is updated
			vecOriginal = vecResult;
			cout << "filtering by " << category << ", videos found: " << vecOriginal.size();

			//for (auto vid : vecOriginal) {
			//	cout << "name found: " << vid.getName() << " with category "<< category << endl; 
			//}

			cout << endl;
		}
		//------------------------------------------------------

		//----------------------------------------------------------------
		void filterVideosByCategories(vector<string>&catToFind) {
			for (auto cat : catToFind) {
				this->filterVideo(cat);
			}
		}
		//----------------------------------------------------------------

		void generateTxtReport(const size_t& number) {
		/*	ofstream ofile;
			string filename;
			//create file
			if (this->firstFilter.at(0) == '-') {
				filename = "non" + this->firstFilter + ".txt";
		    }
			else {
				filename = this->firstFilter + ".txt";
			}
			ofile.open(filename);

			int count = 0;
			if ((number == 0) || (number >= this->vecOriginal.size())) {
				for (auto vid : this->vecOriginal) {
					ofile << vid.getName() << "\n";
					count++;

				}
			}
			else {  //random videos
				vector<size_t> rdNum = generateRandom(this->vecOriginal.size(), number);
				for (auto num : rdNum) {
					    ofile << this->vecOriginal[num].getName() << "\n";
						count++;
		
				}
			}
			ofile.close();
			cout << "Video list saved on file " << filename << count;*/
		}

		void copyVideos(const std::string& origin, const std::string& destiny, const size_t& number) {

			int count = 0;
			if ((number == 0) || (number >= this->vecOriginal.size())) {
				for (auto vid : this->vecOriginal) {
					if (vid.Copy(origin, destiny)) {
						count++;
					}

				}
			}
			else {  //random videos
				vector<size_t> rdNum = generateRandom(this->vecOriginal.size(), number);
				for (auto num : rdNum) {
					if (this->vecOriginal[num].Copy(origin, destiny)) {
						count++;
					}
				}
			}
			cout << "Number of videos copied : " << count;
		}

		//----------------------------------------------------------------
		///@detail generate an array of 'n' random numbers from 0 to range
		///@param range - the uper limit of the random numbers, n - number of random numbers to generate
		//----------------------------------------------------------------
		vector<size_t>& generateRandom(const size_t &range, const size_t &n) {

			for (size_t i = 0; i < range; i++) {
				rdNum.push_back(i);
			}

			std::srand((unsigned int)std::time(0)); // seed the rng
			// randomly shuffle elements in the array
			std::random_shuffle(rdNum.begin(), rdNum.end());

			//now pop back the unused ints
			for (size_t i = (rdNum.size() - 1); i >= n; i--) {
				rdNum.pop_back();
			}
			return rdNum;
		}

	}; //end of class Videos
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	///@enum CSVState
	///@detail Used to control if the next char in the CSV has ""
	//----------------------------------------------------------------
	enum CSVState { UnquotedField, QuotedField, QuotedQuote };
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	///@class CSV
	///@detail CSV class loads a csv file to a 2D vector while eliminating quoted " and double quoted fields ""
	//----------------------------------------------------------------
	class CSV {

		vector<vector<string>> csv;

	public:
		//returning a reference to avoid a Linux gcc bug
		vector<vector<string>>& getCSV() { return csv; } ///@detail Return the vector with all csv

		//----------------------------------------------------------------
		/// Read CSV file. Accept "quoted fields ""with quotes"""
		//----------------------------------------------------------------
		void readCSV(istream &in) {
			int count = 0;
			vector<vector<string>> table;
			string row;

			cout << "loading.";
			while (!in.eof()) {
				getline(in, row);
				if (in.bad() || in.fail()) {
					break;
				}
				auto fields = readCSVRow(row);
				table.push_back(fields);
				count++;
				if ((count % 500) == 0) { cout << "."; }
			}
			csv = table;
			cout << endl << "Loaded " << count << " lines from .csv file." << endl << endl;
		}
		//----------------------------------------------------------------

		//----------------------------------------------------------------
		/// @detail Used for testing only
		//----------------------------------------------------------------
		void printCSV() {
			cout << "from CSV..." << endl;
			for (size_t i = 0; i < this->csv.size(); i++) {
				cout << this->csv[i][0] << " categories -> " << this->csv[i][14] << endl;
			}
		}
		//----------------------------------------------------------------

		//----------------------------------------------------------------
		vector<string> readCSVRow(const string &row) {
			CSVState state = CSVState::UnquotedField;
			vector<string> fields{ "" };
			size_t i = 0; // index of the current field
			for (char c : row) {
				switch (state) {
				case CSVState::UnquotedField:
					switch (c) {
					case ',': // end of field
						fields.push_back(""); i++;
						break;
					case '"': state = CSVState::QuotedField;
						break;
					default:  fields[i].push_back(c);
						break;
					}
					break;
				case CSVState::QuotedField:
					switch (c) {
					case '"': state = CSVState::QuotedQuote;
						break;
					default:  fields[i].push_back(c);
						break;
					}
					break;
				case CSVState::QuotedQuote:
					switch (c) {
					case ',': // , after closing quote
						fields.push_back(""); i++;
						state = CSVState::UnquotedField;
						break;
					case '"': // "" -> "
						fields[i].push_back('"');
						state = CSVState::QuotedField;
						break;
					default:  // end of quote
						state = CSVState::UnquotedField;
						break;
					}
					break;
				}
			}
			return fields;
		}
		//----------------------------------------------------------------


	};

} //end of namespace vid


#endif // VIDEO_H
