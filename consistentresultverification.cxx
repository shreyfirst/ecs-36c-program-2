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

  json result;
  int samplesWithConflictingResults = 0;
  for (json::iterator it1 = json1.begin(); it1 != json1.end(); ++it1) {
    json& sample1 = it1.value();
    json& sample2 = json2[it1.key()];
    if (sample1 != sample2) {
      json& mismatches = result[it1.key()]["Mismatches"];
      int mismatchIndex = 0;
      for (unsigned int i = 0; i < sample1.size(); i++) {
        if (sample1[i] != sample2[i]) {
          mismatches[++mismatchIndex] = json::array({sample1[i], sample2[i]});
        }
      }
      samplesWithConflictingResults++;
    }
    result[it1.key()]["File1"] = sample1;
    result[it1.key()]["File2"] = sample2;
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
