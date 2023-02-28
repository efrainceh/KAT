#include "../include/inputOutput.h"
#include <iostream>



Input InputParser::parse(int argc, char* argv[]) {
    if (minimumNumberOfArguments(argc)) {
        input.projectName = argv[2];
        input.pathToInputFolder = argv[3];
        input.filesSuffix = argv[4];
        input.kmerSize = parseKmerSize(argv[5]);
    } else {
        throw std::out_of_range("The minimum number of inputs is 4.");
    }
    return input;
}

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
            Number of unique matches: 3
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
    file << "Number of unique matches: " << alignment.numberOfUniqueMatches << std::endl;
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

void TableWriter::writeTable(Matrix table_, Matrix uniqueHitTable_, doubleMatrix percentageTable_, int kmerSize_, Filenames referenceFilenames_, Filenames sampleFilenames_) {
    /* 
    Table example:

        Sample   kmer_size Reference unique_hits hits percentage
        Sample1    20        Ref1      3          5      0.05
        Sample2    20        Ref1      2          2      0.001
        Sample1    20        Ref2     70          70     0.1
        Sample2    20        Ref2      0          0      0.0
    */
    
    table = table_;
    uniqueHitTable = uniqueHitTable_;
    percentageTable = percentageTable_;
    referenceFilenames = removeSuffix(referenceFilenames_);
    sampleFilenames = removeSuffix(sampleFilenames_);
    numberOfReferences = referenceFilenames.size();
    numberOfSamples = sampleFilenames.size();
    kmerSize = kmerSize_;
    writeColumnNames();
    writeDataInTable();
}

std::vector<std::string> TableWriter::removeSuffix(Filenames filenames) {
    std::vector<std::string> filenamesNoSuffix;
    for (std::string filename : filenames) {
        int suffixStart = filename.find_last_of(SUFFIX_START_CHAR);
        filenamesNoSuffix.push_back(filename.substr(0, suffixStart));
    }
    return filenamesNoSuffix;
}

void TableWriter::writeColumnNames() {
    for (int col = 0; col < COLUMNS.size(); col++) {
        if (isLastColumn(col)) {
            file << COLUMNS[col] << std::endl;
        } else {
            file << COLUMNS[col] << DELIMITER;
        }
    }
}

void TableWriter::writeDataInTable() {
    for (int sampleNumber = 0; sampleNumber < numberOfSamples; sampleNumber++) {
        writeSampleData(sampleNumber);
    }
}

void TableWriter::writeSampleData(int sampleNumber) {
    for (int referenceNumber = 0; referenceNumber < numberOfReferences; referenceNumber++) {
        file << sampleFilenames[sampleNumber] << DELIMITER;
        file << kmerSize << DELIMITER;
        file << referenceFilenames[referenceNumber] << DELIMITER;
        file << table[referenceNumber][sampleNumber] << DELIMITER;
        file << uniqueHitTable[referenceNumber][sampleNumber] << DELIMITER;
        file << percentageTable[referenceNumber][sampleNumber] << std::endl;
    }    
}

bool TableWriter::isLastColumn(int col) {
    return col == COLUMNS.size() - 1;
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
    std::string filename = results.projectName + TABLE_SUFFIX;
    TableWriter writer(filename);
    writer.writeTable(results.hitTable, results.uniqueHitTable, results.percentageTable, results.kmerSize, results.referenceFilenames, results.sampleFilenames);
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

