#include <iostream>//include iostream
#include <fstream>//include fstream for reading files
#include "json.hpp"//include json library
using json = nlohmann::json; //shortens json delcarations
using namespace std;//uses std namespace

int main(int argc, char* argv[]) {//main function
    if (argc < 2) { //checks if there is a filename in the command line
        cout << "Please provide the name of the JSON file." << endl; //error message if there is no file 
        return 1;//return
    }//endif

    // parse the input file
    string file_name = argv[1];//keeps the file name that was inputted by the user
    json input_json; //creates a json object to store the file 
    try {//try and catch if there are errors in the parsing
        input_json = json::parse(ifstream(file_name)); //parses the file into the json 
    } catch (const json::parse_error& error) {//defines the parsing error 
        cout << "Error parsing JSON file: " << error.what() << endl; //prints error message
        return 1;//returns
    }//endtrycatch

    
    json output_json = json::object();// create the output JSON object
    output_json["metadata"] = json::object();//assigns the metadata of the output json 
    output_json["metadata"]["file"] = file_name; //assigns the filename of the output json
    output_json["metadata"]["arraySize"] = input_json["metadata"]["arraySize"]; //assigns the size of the array of the output json
    output_json["metadata"]["numSamples"] = input_json["metadata"]["numSamples"];//assigns the total number of samples to the output json
    // output_json["metadata"]["samplesWithInversions"] = 0;
    int samplesWithInversions = 0;//keeps value of samples that have inversions in them

    // iterate over the samples
    for (json::iterator it = input_json.begin(); it != input_json.end(); ++it) {//for loop iterates over the input samples 
        if (it.key() == "metadata") {//doesn't check metadata because it is not a sample
            continue;//continues if iterator is not checking the metadata
        }//endif
        json sample = it.value(); // check if the sample is sorted
        bool is_sorted = true; //keeps t/f if the sample is sorted 
        json inversions = json::object();//json object that keeps the sample with inversions 
        for (int i = 0; i < sample.size() - 1; i++) {//for loop iterates through the sample to check if it is sorted
            if (sample[i] > sample[i + 1]) {//compares consecutive sample array indexes 
                is_sorted = false;//if the sample has two consecutive indexes that are not sorted the sample is not sorted
                inversions[to_string(i)] = json::array({sample[i], sample[i + 1]});//adds the indexes where the inversion is to an array
            }//endif
        }//endfor

        // add the sample to the output JSON
        if (!is_sorted) {//if checks if the json is not sorted 
            output_json[it.key()] = json::object();//adds the output of the object that isn't sorted
            output_json[it.key()]["ConsecutiveInversions"] = inversions;//adds the consecutive inversions to the output
            output_json[it.key()]["sample"] = sample; //outputs the sample number where inversion occurs
            samplesWithInversions++; //increments number of samples with inversions 
        }//endif
    }//endfor

    output_json["metadata"]["samplesWithInversions"] = samplesWithInversions; //outputs the sample numbers that have consecutive inversions
 
    
    cout << output_json.dump(2) << endl; // print the output JSON

    return 0;//return
}//end main
