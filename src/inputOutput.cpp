#include "../include/inputOutput.h"
#include <iostream>



Input InputParser::parse(int argc, char* argv[]) {
    if (minimumNumberOfArguments(argc)) {
        input.pathToInputFolder = argv[2];
        input.filesSuffix = argv[3];
        input.kmerSize = parseKmerSize(argv[4]);
    } else {
        throw std::out_of_range("The minimum number of inputs is 3.");
    }
    return input;
}

// bool InputParser::correctNumberOfArguments(int numberOfArguments) {
//     return numberOfArguments == CORRECT_NUMBER_OF_ARGUMENTS;
// }

bool InputParser::minimumNumberOfArguments(int numberOfArguments) {
    return numberOfArguments >= MINIMUM_NUMBER_OF_ARGUMENTS;
}

int InputParser::parseKmerSize(std::string kmerSize) {
    try {
        int size = stoi(kmerSize);
        if (validKmerSize(size)) {
            return size;
        }
    } catch (const std::invalid_argument) {
        return FAILED_INTEGER_CONVERSION;
    }
    return FAILED_INTEGER_CONVERSION;
}

bool InputParser::validKmerSize(int size) {
    if (size > MINIMUM_KMER_SIZE && size < MAXIMUM_KMER_SIZE) {
        return true;
    } else {
        throw std::invalid_argument("kmer size should be bigger than 4 and smaller than 1000");
        return false;
    } 
}

// -----------------------------------------------------------------------------------------------

void AlignmentWriter::writeFile(Alignment alignment_) {
    /*
    First position, string: kmer that matches
    Second position, int: index of sequence in reference
    Next positions, int: index of matches for kmer in that specific sequence of reference 
        example:
            Reference: referenceID
            Sample: sampleID
            Number of matches: 9 
            ATCAG 0 5 20 34
            AGGTG 0 3 14 55
            ATCAG 1 3
            GGGTT 1 10 15
    */
    alignment = alignment_;
    writeAlignmentInfo();
    writeAlignmentResults();
}

void AlignmentWriter::writeAlignmentInfo() {
    file << "Reference: " << alignment.referenceID << std::endl;
    file << "Number of Reference Sequences: " << alignment.numberOfSeqInRef << std::endl;
    writeRefSeqSizes(alignment.refSeqSizes);
    file << "Sample: " << alignment.sampleID << std::endl;
}

void AlignmentWriter::writeRefSeqSizes(std::vector<int> refSeqSizes) {
    file << "Basepair per sequence:";
    for (int size : refSeqSizes) {
        file << " " << size;
    }
    file << std::endl;
}

void AlignmentWriter::writeAlignmentResults() {
    file << "Number of matches: " << alignment.numberOfMatches << std::endl;
    for (auto indexAndMultipleMatches : alignment.sampleToRefMatches) {
        int indexOfTI = indexAndMultipleMatches.first;
        writeMatchesOfIndex(indexOfTI, indexAndMultipleMatches.second);
    }
}

void AlignmentWriter::writeMatchesOfIndex(int indexOfTI, MultipleMatches &multipleMatches) {
    for (auto kmerAndMatches : multipleMatches) {
        file << kmerAndMatches.first << " " << indexOfTI << " " << kmerAndMatches.second;
    }
}

// -----------------------------------------------------------------------------------------------

void TableWriter::writeTable(Matrix table_, Filenames referenceFilenames_, Filenames sampleFilenames_) {
    /*
            Sample1 Sample2 Sample3
        Ref1    1       5       0
        Ref2    20      3       2
        Ref3    0       50      100
        Ref4    2       9       3
    */
    
    table = table_;
    referenceFilenames = referenceFilenames_;
    sampleFilenames = sampleFilenames_;
    numberOfReferences = referenceFilenames.size();
    numberOfSamples = sampleFilenames.size();
    writeSampleFilenames();
    writeDataInTable();
}

void TableWriter::writeSampleFilenames() {
    file << GENOME_COL_NAME << ',';
    for (int col = 0; col < numberOfSamples; col++) {
        if (isLastColumn(col)) {
            file << sampleFilenames[col] << std::endl;
        } else {
            file << sampleFilenames[col] << ',';
        }
    }
    file << std::endl;
}

void TableWriter::writeDataInTable() {
    for (int row = 0; row < numberOfReferences; row++) {
        file << referenceFilenames[row] << ',';
        writeSingleRowOfData(row);
    }
}

void TableWriter::writeSingleRowOfData(int row) {
    for (int col = 0; col < numberOfSamples; col++) {
        if (isLastColumn(col)) {
            file << table[row][col] << std::endl;
        } else {
            file << table[row][col] << ',';
        }
    }
}

bool TableWriter::isLastColumn(int col) {
    return col == numberOfSamples - 1;
}

// -----------------------------------------------------------------------------------------------

void ResultsWriter::write(Results results_) {
    results = results_;
    goToResultsFolder();
    writeResultsTable();
    writeResultsAlignmentFiles();
}

void ResultsWriter::goToResultsFolder() {
    std::filesystem::path resultsFolderPath = createResultsPath();
    std::filesystem::create_directory(resultsFolderPath);
    std::filesystem::current_path(resultsFolderPath);
}

std::filesystem::path ResultsWriter::createResultsPath() {
    return results.pathToInputFolder + RESULTS_FOLDER;
}

void ResultsWriter::writeResultsTable() {
    TableWriter writer(TABLE_FILENAME);
    writer.writeTable(results.finalTable, results.referenceFilenames, results.sampleFilenames);
}

void ResultsWriter::writeResultsAlignmentFiles() {
    for (Alignment alignment : results.alignments) {
        std::string filename = createAlignmentFilename(alignment);
        AlignmentWriter writer(filename);
        writer.writeFile(alignment);
    }
}

std::string ResultsWriter::createAlignmentFilename(Alignment &alignment) {
    return alignment.referenceID + ALIGNMENT_FILENAME_LINK + alignment.sampleID + ALIGNMENT_FILENAME_SUFFIX;
}

// -----------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream &os, Matches &matches) {
    for (int match : matches) {
        os << match << " ";
    }
    os << std::endl;
    return os;
}

