#ifndef REFERENCE_ALIGNER_H
#define REFERENCE_ALIGNER_H

#include <string>
#include <vector>
#include <utility>
#include "genome.h"

typedef std::vector<int> Matches;
typedef std::map<std::string, Matches> MultipleMatches;
typedef std::map<int, MultipleMatches> ReferenceMatches;

struct Alignment {

    std::string referenceID;
    int numberOfSeqInRef;
    std::vector<int> refSeqSizes;
    std::string sampleID;
    ReferenceMatches sampleToRefMatches;
    int numberOfMatches;
    float matchPercentage;      // (number of unique hits to reference / total number of kmers in reference ) * 100 

};

class Aligner {
/*
    One text aligned to one Text Index
*/

    private:
        int NON_EXISTING_CHAR = -1;
        int NO_MATCHES = 0;
        Matches EMPTY_MATCHES = {};
        int matches;
        int top;
        int bottom;
        int charIndex;
        Matrix countMatrix;
        std::vector<int> starts;
        std::vector<int> suffixArray;
        CharIndexMap charIndexMap;

        void getVarFromTextIndex(TextIndex textIndex);
        bool textIndexNotEmpty(TextIndex &textIndex);
        void setMatchTopBottom();
        int getCharIndex(char character);
        int findMatches();
        int findTop();
        int findBottom();
        Matches findMatchIndeces();

    public:

        Aligner(TextIndex textIndex) {
            getVarFromTextIndex(textIndex);
        }

        Matches align(std::string text);

};

class MultipleAligner : public Aligner {
/*
    Multiple text aligned to one Text Index
*/

    private:
        int totalNumberOfMatches;

    public:
        MultipleAligner(TextIndex textIndex) : Aligner(textIndex) { }
        
        MultipleMatches multipleAlign(std::vector<std::string> multipleText);
        int getNumberOfMatches() { return totalNumberOfMatches; }

};

class ReferenceAligner {
/*
    Multiple text aligned to all Text Index in a Reference
*/

    private:
        std::string referenceID;
        int numberOfSeqInRef;
        std::vector<int> refSeqSizes;
        std::string sampleID;
        TextIndeces TI;
        int kmerSize;
        std::vector<std::string> vectorKmers;
        int totalNumberOfMatches;
        float matchPercentage;
        MultipleMatches matchesPerTextIndex;
        ReferenceMatches matchesToReference;

        void extractVarFromReference(Reference reference);
        void extractVarFromSample(Sample sample);
        void convertKmersMapToVector(Kmers kmers);
        void calculateMatchPercentage();
        int calculatePossibleKmersInReference();
        Alignment createAlignment();
        
    public:
        ReferenceAligner(Reference reference) {
            extractVarFromReference(reference);
        } 

        Alignment align(Sample sample);

};

#endif