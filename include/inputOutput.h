#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <string>
#include <vector>
#include <fstream>
#include "fileFolders.h"
#include "referenceAligner.h"

typedef std::vector<std::vector<double>> doubleMatrix;

std::ostream& operator<<(std::ostream &os, Matches &matches);

struct Input {

    std::string projectName;
    std::string pathToInputFolder;
    std::string filesSuffix;
    int kmerSize;

};

struct Results {

    std::string projectName;
    std::string pathToInputFolder;
    int kmerSize;
    std::vector<Alignment> alignments;
    Filenames referenceFilenames;
    Filenames sampleFilenames;
    Matrix uniqueHitTable;  // Total number of unique matches per sample per reference
    Matrix hitTable;      // Total number of matches per sample per reference
    doubleMatrix percentageTable;

};

class InputParser {

    private:
        int FAILED_INTEGER_CONVERSION = -1;
        int MINIMUM_NUMBER_OF_ARGUMENTS = 6; // path, number_of_arguments, 3 variables
        int MINIMUM_KMER_SIZE = 4;
        int MAXIMUM_KMER_SIZE = 1001;
        Input input;
        
        bool minimumNumberOfArguments(int numberOfArguments);
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
        std::vector<std::string> COLUMNS{"sample", "kmer_size", "reference", "hits", "unique_hits", "percentage"};
        char DELIMITER = ',';
        char SUFFIX_START_CHAR = '.';
        Matrix table;
        Matrix uniqueHitTable;
        doubleMatrix percentageTable;
        Filenames referenceFilenames;
        Filenames sampleFilenames;
        int numberOfReferences;
        int numberOfSamples;
        int kmerSize;

        void createFilename();
        void writeColumnNames();
        void writeSampleData(int sampleNumber);
        void writeDataInTable();
        bool isLastColumn(int col);
        std::vector<std::string> removeSuffix(Filenames filenames);

    public:
        TableWriter(std::string filename_) : BaseWriter(filename_) { }

        void writeTable(Matrix table_, Matrix uniqueHitTable_, doubleMatrix percentageTable_, int kmerSize, Filenames referenceFilenames_, Filenames samplesFilenames_);

};

class ResultsWriter {

    private:
        std::string RESULTS_FOLDER = "/Results";
        std::string ALIGNMENT_FILENAME_LINK = "_";
        std::string ALIGNMENT_FILENAME_SUFFIX = ".txt";
        std::string TABLE_SUFFIX = ".csv";
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