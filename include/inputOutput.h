#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <string>
#include <vector>
#include <fstream>
#include "fileFolders.h"
#include "referenceAligner.h"

std::ostream& operator<<(std::ostream &os, Matches &matches);

struct Input {

    std::string pathToInputFolder;
    std::string filesSuffix;
    int kmerSize;
    std::vector<int> kmerSizes;

};

struct Results {

    std::string pathToInputFolder;
    std::vector<Alignment> alignments;
    Filenames referenceFilenames;
    Filenames sampleFilenames;
    Matrix finalTable;      // Total number of matches per sample per reference

};

class InputParser {

    private:
        int FAILED_INTEGER_CONVERSION = -1;
        int MINIMUM_NUMBER_OF_ARGUMENTS = 5; // path, number_of_arguments, 3 variables
        //int CORRECT_NUMBER_OF_ARGUMENTS = 5; // path, number_of_arguments, 3 variables
        int MINIMUM_KMER_SIZE = 4;
        int MAXIMUM_KMER_SIZE = 1001;
        Input input;
        
        bool minimumNumberOfArguments(int numberOfArguments);
        //bool correctNumberOfArguments(int numberOfArguments);
        int parseKmerSize(std::string kmerSize);
        bool validKmerSize(int size);

    public:
        InputParser() { }
        
        Input parse(int argc, char* argv[]);

};

class BaseWriter {

    protected:
        std::ofstream file;
        std::string filename;

    public:
        BaseWriter(std::string filename_): file(filename_), filename(filename_) { }

};

class AlignmentWriter : public BaseWriter {
    
    private:
    
        Alignment alignment;

        void writeAlignmentInfo();
        void writeRefSeqSizes(std::vector<int> refSeqSizes);
        void writeAlignmentResults();
        void writeMatchesOfIndex(int indexOfTI, MultipleMatches &multipleMatches);

    public:
        AlignmentWriter(std::string filename_) : BaseWriter(filename_) { }

        void writeFile(Alignment alignment_);

};

class TableWriter : public BaseWriter {

    private:
        std::string OUTPUT_SUFFIX = ".csv";
        std::string GENOME_COL_NAME = "genomes";
        Matrix table;
        Filenames referenceFilenames;
        Filenames sampleFilenames;
        int numberOfReferences;
        int numberOfSamples;

        void createFilename();
        void writeSampleFilenames();
        void writeDataInTable();
        void writeSingleRowOfData(int row);
        bool isLastColumn(int col);

    public:
        TableWriter(std::string filename_) : BaseWriter(filename_) { }

        void writeTable(Matrix table_, Filenames referenceFilenames_, Filenames samplesFilenames_);

};

class ResultsWriter {

    private:
        std::string RESULTS_FOLDER = "/Results";
        std::string ALIGNMENT_FILENAME_LINK = "_";
        std::string ALIGNMENT_FILENAME_SUFFIX = ".txt";
        std::string TABLE_FILENAME = "alignment_table.csv";
        Results results;

        void goToResultsFolder();
        std::filesystem::path createResultsPath();
        void writeResultsTable();
        void writeResultsAlignmentFiles();
        std::string createAlignmentFilename(Alignment &alignment);

    public:
        ResultsWriter() { }

        void write(Results results_);

};

#endif