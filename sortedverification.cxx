#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Please provide the name of the JSON file." << std::endl;
        return 1;
    }

    // parse the input file
    std::string file_name = argv[1];
    json input_json;
    try {
        input_json = json::parse(std::ifstream(file_name));
    } catch (const json::parse_error& e) {
        std::cout << "Error parsing JSON file: " << e.what() << std::endl;
        return 1;
    }

    // create the output JSON object
    json output_json = json::object();
    output_json["metadata"] = json::object();
    output_json["metadata"]["file"] = file_name;
    output_json["metadata"]["arraySize"] = input_json["metadata"]["arraySize"];
    output_json["metadata"]["numSamples"] = input_json["metadata"]["numSamples"];
    // output_json["metadata"]["samplesWithInversions"] = 0;
    int samplesWithInversions = 0;

    // iterate over the samples
    for (json::iterator it = input_json.begin(); it != input_json.end(); ++it) {
        if (it.key() == "metadata") {
            continue;
        }

        // check if the sample is sorted
        json sample = it.value();
        bool is_sorted = true;
        json inversions = json::object();
        int inversion_count = 0;
        for (unsigned int i = 0; i < sample.size() - 1; i++) {
            if (sample[i] > sample[i + 1]) {
                is_sorted = false;
                inversions[std::to_string(i)] = json::array({sample[i], sample[i + 1]});
            }
        }
        
        // add the sample to the output JSON
        if (!is_sorted) {
            output_json[it.key()] = json::object();
            output_json[it.key()]["ConsecutiveInversions"] = inversions;
            output_json[it.key()]["sample"] = sample;
            samplesWithInversions++;
        }
    }

    output_json["metadata"]["samplesWithInversions"] = samplesWithInversions;
 
    // print the output JSON
    std::cout << output_json.dump(4) << std::endl;

    return 0;
}
