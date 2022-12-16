#ifndef GENOME_PARSER_H
#define GENOME_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <utility>

struct FirstLine {

    std::string ID;
    std::string shortName;
    std::string completeness;

};

struct Sequence {

    std::string info;
    std::string dna;

};

typedef std::vector<Sequence> Sequences;

bool operator==(const Sequence seq1, const Sequence seq2);
bool operator!=(const Sequence seq1, const Sequence seq2);

class FirstLineParser {
/*
    Input Line Schema: ">ID + space + shortName + , + space + completeness + \r"
    ID is a single word without spaces
    shortName can have spaces, but no commas
    completeness can have spaces and commas, it is whatever remains of the string
    example: ">NC_003155.5 Streptomyces avermitilis MA-4680 = NBRC 14893, complete sequence"
*/

    private:
        int INDEX_START = 1;
        int SKIPS_TO_START_OF_SHORT_NAME = 1;
        int SKIPS_TO_START_OF_COMPLETENESS = 2;
        char ID_DELIMITER = ' ';
        char SHORT_NAME_DELIMITER = ',';
        char COMPLETENESS_DELIMITER = '$';
        char END_OF_LINE = '\n';
        std::string FAILED = "FAILED";
        int currentCharIndex;
        std::string line;
        FirstLine firstLine;
        
        void addDelimitersToLine();
        void parseLine();
        bool notEndOfLine();
        std::string parseUntilDelimiter(char delimiter);
        void moveIndexToNextStart(int skipPositions);

    public:
        FirstLineParser() { }

        FirstLine parseFirstLine(std::string line_);

};

class GenomeParser : public FirstLineParser {

    private:
        char NEXT_SEQUENCE_DELIMITER = '>';
        std::string line;
        std::ifstream file;
        std::string filename;
        FirstLine firstLine;
        Sequences sequences;

        void parseFile();
        bool validFirstLine();
        void readDnaSequences();
        Sequence readOneDnaSequence();
        std::string parseDnaSequenceInfo(); 
        std::string parseDna();
        bool lineIsDna();

    public:
        GenomeParser(std::string filename_) : file(filename_), filename(filename_) { }
        
        std::pair<FirstLine, Sequences> parse();

};

#endif