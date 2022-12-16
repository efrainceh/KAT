#include "../include/genomeParser.h"



FirstLine FirstLineParser::parseFirstLine(std::string line_) {
    line = line_;
    if (!line.empty()) {
        addDelimitersToLine();
        parseLine();
    } else {
        throw std::invalid_argument("genomeParser.h, parseFirstLine: Empty line in FirstLineParser");
    }
    return firstLine;
}

void FirstLineParser::addDelimitersToLine() {
    line = line + COMPLETENESS_DELIMITER + END_OF_LINE;
}

void FirstLineParser::parseLine() {
    currentCharIndex = INDEX_START;
    firstLine.ID = parseUntilDelimiter(ID_DELIMITER);
    moveIndexToNextStart(SKIPS_TO_START_OF_SHORT_NAME);
    firstLine.shortName = parseUntilDelimiter(SHORT_NAME_DELIMITER);
    moveIndexToNextStart(SKIPS_TO_START_OF_COMPLETENESS);
    firstLine.completeness = parseUntilDelimiter(COMPLETENESS_DELIMITER);
}

std::string FirstLineParser::parseUntilDelimiter(char delimiter) {
    std::string tempString = "";
    while (line[currentCharIndex] != delimiter) {
        if (notEndOfLine()) {
            tempString += line[currentCharIndex];
            currentCharIndex++;
        } else {
            currentCharIndex = INDEX_START;
            return FAILED;
        }
    }
    return tempString;
}

bool FirstLineParser::notEndOfLine() {
    if (line[currentCharIndex] == END_OF_LINE) {
        throw std::invalid_argument("genomeParser.h, notEndOfLine: Invalid line in FirstLineParser");
        return false;
    }
    return true;
}

void FirstLineParser::moveIndexToNextStart(int skipPositions) {
    currentCharIndex += skipPositions;
}

// -----------------------------------------------------------------------------------------------

std::pair<FirstLine, Sequences> GenomeParser::parse() {
    if (!file.is_open()) {
        std::string error_msg = "genomeParser.h, parse: Error opening file: " + filename;
        throw std::out_of_range(error_msg);
    }
    parseFile();
    if (file.bad()) {
        std::string error_msg = "genomeParser.h, parse: Error reading file: " + filename;
        throw std::out_of_range(error_msg);
    }
    return std::make_pair(firstLine, sequences);
}

void GenomeParser::parseFile() {
    if (validFirstLine()) {
        firstLine = parseFirstLine(line);
        readDnaSequences();
    } else {
        throw std::invalid_argument("Invalid First Line in file");
    }
}

bool GenomeParser::validFirstLine() {
    getline(file, line);
    return !line.empty() && line.at(0) == NEXT_SEQUENCE_DELIMITER;
}

void GenomeParser::readDnaSequences() {
    while (!file.eof()) {
        Sequence dna = readOneDnaSequence();
        sequences.push_back(dna);
    }
}

Sequence GenomeParser::readOneDnaSequence() {
    Sequence sequence;
    sequence.info = parseDnaSequenceInfo();
    sequence.dna = parseDna();
    return sequence;
}

std::string GenomeParser::parseDnaSequenceInfo() {
    return line.substr(1, line.size() - 1);;
}

std::string GenomeParser::parseDna() {
    std::string dna = "";
    while (getline(file, line) && lineIsDna()) {
        dna += line;
    }
    return dna;
}

bool GenomeParser::lineIsDna() {
    return line.empty() || line.at(0) != NEXT_SEQUENCE_DELIMITER;
}

// -----------------------------------------------------------------------------------------------

bool operator==(const Sequence seq1, const Sequence seq2) {
    return (seq1.info == seq2.info && seq1.dna == seq2.dna);
}

bool operator!=(const Sequence seq1, const Sequence seq2) {
    return !(seq1 == seq2);
}