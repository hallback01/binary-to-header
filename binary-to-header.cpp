#include <fstream>
#include <vector>
#include <iostream>

bool validate_array_name(std::string& array_name) {

    //digits at the first position is not allowed
    if(isdigit(array_name[0])) {
        return false;
    }

    //characters must be inside the alphabet
    for(size_t i = 0; i < array_name.size(); i++) {
        if(!isalpha(array_name[i]) && !isdigit(array_name[i]) && array_name[i] != '_') {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) {

    //there must be a total of 4 arguments
    if(argc != 4) {
        std::cout << "Wrong usage. Please input the correct arguments like below.\n";
        std::cout << "binary-to-header <input> <output> <array-name>\n";
        return 1;
    }

    //else get the input and output arguments
    std::string input_file = argv[1];
    std::string output_location = argv[2];
    std::string array_name = argv[3];

    //check if the array name is valid
    if(!validate_array_name(array_name)) {
        std::cout << "The array cannot be named '" << array_name << "'. Conditions:\n";
        std::cout << " --- Numbers at the beginning is not allowed.\n";
        std::cout << " --- Only characters from the english alphabet is allowed(lower, and uppercase).\n";
        std::cout << " --- Only the special character '_' is allowed.\n";
        return 1;
    }

    //try to open the input file
    std::ifstream input(input_file, std::ios::binary);
    if(input.is_open()) {

        //if it was opened successfully, read the contents into a vec
        std::cout << "[1] Reading input file '" << input_file << "'." << std::flush;
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
        std::cout << " Done.\n";

        //then convert the binary data into a string, where every entry in the buffer corresponds to an entry in an array in this string.
        //first, declare the string with a "#pragma once" statement and the declaration of an array
        std::cout << "[2] Converting binary file into array with the name '" << array_name << "'." << std::flush;
        std::string file = "#pragma once\n\nconst unsigned char " + array_name + "[" + std::to_string(buffer.size()) + "] = {";
        for(size_t i = 0; i < buffer.size(); i++) {

            //then for every element, append it's value into the array in the string
            size_t value = buffer[i];
            file.append(std::to_string(value));

            //then append a colon to separate them
            file.append(",");
        }

        //lastly, close the definition and add a semicolon
        file.append("\n};");
        input.close();
        std::cout << " Done.\n";

        //now save the file at the output location
        std::cout << "[3] Saving array to output file '" << output_location << "." << std::flush;
        std::ofstream output(output_location);
        output << file;
        output.close();
        std::cout << " Done.\n";

    } else {
        std::cout << "Could not open " << input_file << ". Does it exist?\n";
        return 1;
    }

    return 0;
}