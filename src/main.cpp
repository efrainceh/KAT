#include <iostream>
#include "../include/inputOutput.h"
#include "../include/KAT.h"


int main(int argc, char* argv[]) {

    try {
        std::cout << "Parsing input..." << std::endl;
        InputParser parser;
        Input input = parser.parse(argc, argv);
        std::cout << "Starting alignment..." << std::endl;
        KAT aligner(input);
        Results result = aligner.align();
        std::cout << "Writing results..." << std::endl;
        ResultsWriter writer;
        writer.write(result);
    } catch (std::invalid_argument) {
        std::cout << "ERROR" << std::endl;
    }

    return 0;
    
}