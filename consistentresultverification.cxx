#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " file1.json file2.json" << std::endl;
    return 1;
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
    result[key]["File1"] = test1;
    result[key]["File2"] = test2;
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

  std::cout << result.dump(4) << std::endl;
  return 0;
}
