#include <iostream>
#include <string>
#include "categories.h"

using namespace std;

constexpr char pathCatFile[] = "allCategories.csv";
constexpr char pathTrainDir[] = "/mnt/vol/Andre2/Trained/";


bool fileExist(const std::string& filePath)
{
    std::ifstream infile(filePath);
    return infile.good();
}

int main()
{
    vector <std::string> catMissing;


    //1. Open the list of categories
    std::cout << "Reading " << "allCategories.csv" << std::endl;
    svm::Categories::loadAllCategoryList(pathCatFile);
    svm::Categories::deleteLesserCategories(20);
    vector<string> vecCat = svm::Categories::getVector();

    //2. Loop the list and check the file for each
    for (auto cat: vecCat) {
       auto str =  "svm_" + cat + ".dat";
       str = pathTrainDir + str;
      // std:: cout << str << endl;
       bool ok =  fileExist (str);
       if (ok == false) {
           catMissing.push_back(cat);
           std::cout << cat << endl;
       }
    }

    //3. Print the missiong ones
    std::cout << "NUMBER OF CATEGORIES MISSING: " << catMissing.size() <<std::endl;
    std::cout << "CATEGORIES MISSING: " << std::endl;

    for (string cat: catMissing) {
      //    std::cout << cat << endl;
    }

    return 0;
}

