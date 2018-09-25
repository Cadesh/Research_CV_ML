//---------------------------------------------------------------------------
///@author Andre Rosa
///@detail A class to keep the vector of categories
//---------------------------------------------------------------------------

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <string>
#include <fstream>
#include <omp.h> // for parallel programming
#include "csvParser.h"

using namespace std;

namespace svm {

	struct Category {
		string mName;           ///category name
		size_t mFrequency = 0;  ///the number of available videos 
	};

	///@class A container for the vector of all categories
	///@detail this class has a vector of cateogries`names
    /// and related methods to create folders for each categories
	class Categories {
	private:
		static vector<Category> mVecCategories; ///vector that holds all categories
        static vector<std::string> mCat;

        //Categories() {};
	public:

		static size_t getCategoriesQuant(); ///@details return the current number of categories.
        static vector<string>& getVector(); ///@details resturn the vector with categories names.

		///@function convert the loaded cvs file into categories vector structure. 
		///@param in: csv structure. out: an integer with the number of categories loaded.
		///@detail The loop begins the conversion on line 2 because of the header of the .csv file
		static size_t convertCSV(const vector<vector<string>>&);

		///@detail delete form the vector all categories with less than 'quant' videos
		static void deleteLesserCategories(const size_t &quant);

		///@detail load the .csv file of categories
		static void loadAllCategoryList(const string &filePath);


	};

} //end of namespace svm

#endif // CATEGORIES_H
