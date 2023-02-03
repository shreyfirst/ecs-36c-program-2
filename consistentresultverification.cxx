#include <iostream>//iostream
#include <fstream>//for file reading
#include "json.hpp"//include to use json library

using json = nlohmann::json; // shortens json declarations 
using namespace std; //uses std namespace

int main(int argc, char** argv) {
  if (argc < 3) {//checks if there is only 2 filenames in command line
    cerr << "Usage: " << argv[0] << " file1.json file2.json" << std::endl;//error message
    return 1; //program exits when error message occurs
  }

  json json1, json2;//initializes 2 json object for the two inputted files
  try {//try/catch if there is errors during parsing
    json1 = json::parse(std::ifstream(argv[1]));//parses through first file 
    json2 = json::parse(std::ifstream(argv[2]));//parses second file
  } catch (json::parse_error& error) {//exits if there is an error in the parsing
    cerr << error.what() << std::endl;//outputs error message
    return 1; //exit
  }//end try

  json result;//initializes json object for the result
  int samplesWithConflictingResults = 0; //keeps count of samples 
  for (json::iterator it1 = json1.begin(); it1 != json1.end(); ++it1) { //for loop that iterates through the first json
    json& sample1 = it1.value(); //holds iterator value 
    json& sample2 = json2[it1.key()]; // makes sure the same sample is being compared 
    if (sample1 != sample2) { //checks if samples are the same 
      json& mismatches = result[it1.key()]["Mismatches"]; //json variable to hold the number of mismatches
      int mismatchIndex = 0;//variable to hold the index of mismatch
      for (int i = 0; i < sample1.size(); i++) {//for loop iterates through the sample1 json 
        if (sample1[i] != sample2[i]) { //checks if samples have mismatch at same index
          mismatches[++mismatchIndex] = json::array({sample1[i], sample2[i]}); //saves the mismatches into a json arrayß
        }//endif
      }//endfor
      samplesWithConflictingResults++;//increments the conflicting samples number
    }//endif
    result[it1.key()]["File1"] = sample1; //sets the sample1 value 
    result[it1.key()]["File2"] = sample2; //sets the sample2 value
  }//endfor
  result["metadata"]["samplesWithConflictingResults"] = samplesWithConflictingResults; //converts the number of conflicting samples into json
  result["metadata"]["File1"] = json::object({ //json output to file1
    {"name", argv[1]}, //sets the name of file to first file name
    {"arraySize", json1["metadata"]["arraySize"]},//outputs the size of the array in the json 
    {"numSamples", json1["metadata"]["numSamples"]},//outputs the number of samples 
  });//endresult
  result["metadata"]["File2"] = json::object({//json output for file2
    {"name", argv[2]}, //outputs the name of the file to second file name
    {"arraySize", json2["metadata"]["arraySize"]},//outputs the size of the array for the second file to json
    {"numSamples", json2["metadata"]["numSamples"]},//outputs the number of samples of the second file
  });//endresult

  cout << result.dump(2) << endl;//prints result
  return 0;//return0
}//endmain