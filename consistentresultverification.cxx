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

  json json1, json2;
  try {
    json1 = json::parse(std::ifstream(argv[1]));
    json2 = json::parse(std::ifstream(argv[2]));
  } catch (json::parse_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::vector<std::string> keys;
  for (json::iterator it = json1.begin(); it != json1.end(); ++it) {
    if (it.key() != "metadata") {
      keys.push_back(it.key());
    }
  }

  json result;
  int samplesWithConflictingResults = 0;
  for (const auto& key : keys) {
    json& test1 = json1[key];
    json& test2 = json2[key];
    if (test1 != test2) {
      json& mismatches = result[key]["Mismatches"];
      int mismatchIndex = 0;
      for (unsigned int i = 0; i < test1.size(); i++) {
        if (test1[i] != test2[i]) {
          mismatches[std::to_string(mismatchIndex)] = json::array({test1[i], test2[i]});
          mismatchIndex++;
        }
      }
      samplesWithConflictingResults++;
    }
    result[key][argv[1]] = test1;
    result[key][argv[2]] = test2;
  }
  result["metadata"]["samplesWithConflictingResults"] = samplesWithConflictingResults;
  result["metadata"]["File1"] = json::object({
    {"name", argv[1]},
    {"arraySize", json1["metadata"]["arraySize"]},
    {"numSamples", json1["metadata"]["numSamples"]},
  });
  result["metadata"]["File2"] = json::object({
    {"name", argv[2]},
    {"arraySize", json2["metadata"]["arraySize"]},
    {"numSamples", json2["metadata"]["numSamples"]},
  });

  cout << result.dump(2) << endl;//prints result
  return 0;//return0
}//endmain