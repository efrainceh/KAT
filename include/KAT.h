#ifndef KAT_H
#define KAT_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "inputOutput.h"
#include "genome.h"
#include "fileFolders.h"
#include "referenceAligner.h"

class KAT {

    private:
        std::string REFERENCES_FOLDER = "/References";
        std::string SAMPLES_FOLDER = "/Samples";
        Input input;
        Results result;
        References references;
        Samples samples;   

        void readReferences();
        void calculateReferencesTextIndex();
        void readSamples();
        void calculateSamplesKmers();
        void alignAllSamplesToAllReferences();

    public:
        KAT(Input input_): input(input_) {
            result.pathToInputFolder = input.pathToInputFolder;
        }
        
        Results align();

};

#endif