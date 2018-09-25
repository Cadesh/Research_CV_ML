

#include "categories.h"

vector<svm::Category> svm::Categories::mVecCategories; ///vector that holds all categories
vector<std::string> svm::Categories::mCat;

namespace svm {


	//---------------------------------------------------------------
	//---------------------------------------------------------------
	size_t Categories::convertCSV(const vector<vector<string>>& csv) {
		mVecCategories.clear();
        for (size_t i = 2; i < csv.size(); i++) { //start on number 2 because the .csv file lines 1 and 2 are labels
			Category locCat;
			locCat.mName = (csv[i][0]); //position 0 is the name of the file

			locCat.mFrequency = std::stoi(csv[i][1]);
			mVecCategories.push_back(locCat);
			
		}
		return mVecCategories.size();
	}
	//-----------------------------------------------------------------

	//------------------------------------------------------------------
	///@detail Returns the number of available categories
	//------------------------------------------------------------------
    size_t Categories::getCategoriesQuant() {
		return mVecCategories.size();
	}

	//------------------------------------------------------------------
	///@detail Deletes from the categories any category with less than quant videos
	//------------------------------------------------------------------
	void Categories::deleteLesserCategories(const size_t &quant){ 
		vector<Category> lCat;

		for (Category cat : mVecCategories) {
			if (cat.mFrequency >= quant) {
				lCat.push_back(cat);
			}
		}
		mVecCategories = lCat;
	}

	//------------------------------------------------------------------
	///@detail load the .csv file of categories
	//------------------------------------------------------------------
	void Categories::loadAllCategoryList(const string &filePath) {

		CSV csv;
		string file = filePath;
		ifstream fs(file, ifstream::out);  //CSV file to be read
		if (fs.is_open()) {
			//read the CSV and copies to csv object
			csv.readCSV(fs);
			fs.close();
		    convertCSV(csv.getCSV());
		}
		else {
			cout << "Can not find " << filePath  << endl;
		}

	}

    //------------------------------------------------------------------
    ///@detail return the category vector as a string vector
    //------------------------------------------------------------------
    vector<string>& Categories::getVector() {
        mCat.clear();
        for (auto cat: mVecCategories) {
            mCat.push_back(cat.mName);
        }
        return mCat;
    }

}

