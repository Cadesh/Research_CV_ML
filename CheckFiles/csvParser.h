//---------------------------------------------------------------------------
///@author Andre Rosa
///@detail A class to read csv files
//---------------------------------------------------------------------------

#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>
#include <iostream> //cout

using namespace std;

namespace svm {

	//----------------------------------------------------------------
	///@enum CSVState
	///@detail Used to control if the next char in the CSV has ""
	//----------------------------------------------------------------
	enum CSVState { NoQuotes, Quoted, DoubleQuotes };
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
			vector<vector<string>> table;
			string row;

			while (!in.eof()) {
				getline(in, row);
				if (in.bad() || in.fail()) {
					break;
				}
				auto fields = readCSVRow(row);
				table.push_back(fields);
			}
			csv = table;
		}
		//----------------------------------------------------------------

		//----------------------------------------------------------------
		/// @detail Used for testing only
		//----------------------------------------------------------------
		void printCSV() {
			cout << "from CSV..." << endl;
			for (size_t i = 0; i < csv.size(); i++) {
				cout << csv[i][0] << " categories -> " << csv[i][14] << endl;
			}
		}
		//----------------------------------------------------------------

		//----------------------------------------------------------------
		static vector<string> readCSVRow(const string &row) {
			CSVState state = CSVState::NoQuotes;
			vector<string> fields{ "" };
			size_t i = 0; // index of the current field
			for (char c : row) {
				switch (state) {
				case CSVState::NoQuotes:
					switch (c) {
					case ',': // end of field
						fields.push_back(""); i++;
						break;
					case '"': state = CSVState::Quoted;
						break;
					default:  fields[i].push_back(c);
						break;
					}
					break;
				case CSVState::Quoted:
					switch (c) {
					case '"': state = CSVState::DoubleQuotes;
						break;
					default:  fields[i].push_back(c);
						break;
					}
					break;
				case CSVState::DoubleQuotes:
					switch (c) {
					case ',': // , after closing quote
						fields.push_back(""); i++;
						state = CSVState::NoQuotes;
						break;
					case '"': // "" -> "
						fields[i].push_back('"');
						state = CSVState::Quoted;
						break;
					default:  // end of quote
						state = CSVState::NoQuotes;
						break;
					}
					break;
				}
			}
			return fields;
		}
		//----------------------------------------------------------------
	};

}

#endif // CSV_H
