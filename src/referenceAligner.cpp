#include "../include/referenceAligner.h"

#include <iostream>



void Aligner::getVarFromTextIndex(TextIndex textIndex) {
    if (textIndexNotEmpty(textIndex)) {
        countMatrix = textIndex.charCountMatrix;
        starts = textIndex.starts;
        suffixArray = textIndex.suffixArray;
        charIndexMap = textIndex.charIndexMap;
    } else {
        throw std::invalid_argument("ALIGNER FAIL: textIndex is empty");
    }
}

bool Aligner::textIndexNotEmpty(TextIndex &textIndex) {
    return !(textIndex.charCountMatrix.empty() && textIndex.starts.empty() && textIndex.suffixArray.empty() && textIndex.charIndexMap.empty());
}

Matches Aligner::align(std::string text) {
    setMatchTopBottom();
    for (int i = text.size() - 1; i >= 0; i--) {
        charIndex = getCharIndex(text[i]);
        // early exit when character does not exist
        if (charIndex == NON_EXISTING_CHAR) { 
            return EMPTY_MATCHES;
        }
        matches = findMatches();
        top = findTop();
        bottom = findBottom();
        // early exit when text does not align
        if (matches == NO_MATCHES) {
            return EMPTY_MATCHES;
        }
    }
    return findMatchIndeces();
}

void Aligner::setMatchTopBottom() {
    matches = 0;
    top = 0;
    bottom = countMatrix.size() - 1;
}

int Aligner::getCharIndex(char character) {
    if (charIndexMap.find(character) != charIndexMap.end()) {
        return charIndexMap.at(character);
    } else {
        return NON_EXISTING_CHAR;
    }
}

int Aligner::findMatches() {
    return countMatrix[bottom][charIndex] - countMatrix[top][charIndex];
}

int Aligner::findTop() {
    return starts[charIndex] + countMatrix[top][charIndex];
}

int Aligner::findBottom() {
    return starts[charIndex] + countMatrix[bottom][charIndex];
}

Matches Aligner::findMatchIndeces() {
    return std::vector<int>(suffixArray.begin() + top, suffixArray.begin() + top + matches);
}

// -----------------------------------------------------------------------------------------------

MultipleMatches MultipleAligner::multipleAlign(std::vector<std::string> multipleText) {
    totalNumberOfMatches = 0;
    MultipleMatches multipleMatches;
    for (std::string text : multipleText) {
        Matches matches = align(text);
        if (!matches.empty()) {
            multipleMatches[text] = matches;
            totalNumberOfMatches += matches.size();
        }  
    }
    return multipleMatches;
}

// -----------------------------------------------------------------------------------------------

void ReferenceAligner::extractVarFromReference(Reference reference) {
    referenceID = reference.getID();
    TI = reference.getTextIndeces();
    numberOfSeqInRef = reference.getNumberOfSequences();
    refSeqSizes = reference.getSeqSizes();
}

Alignment ReferenceAligner::align(Sample sample) {
    extractVarFromSample(sample);
    totalNumberOfMatches = 0;
    for (int indexOfTI = 0; indexOfTI < TI.size(); indexOfTI++) {
        MultipleAligner multipleAligner(TI[indexOfTI]);
        matchesPerTextIndex = multipleAligner.multipleAlign(vectorKmers);
        matchesToReference[indexOfTI] = matchesPerTextIndex;
        totalNumberOfMatches += multipleAligner.getNumberOfMatches();
    }
    calculateMatchPercentage();
    return createAlignment();
}

void ReferenceAligner::extractVarFromSample(Sample sample) {
    sampleID = sample.getID();
    kmerSize = sample.getKmerSize();
    Kmers kmers = sample.getKmers();
    convertKmersMapToVector(kmers);
}

void ReferenceAligner::convertKmersMapToVector(Kmers kmers) {
    for (auto kmerAndInt : kmers) {
        vectorKmers.push_back(kmerAndInt.first);
    }
}

void ReferenceAligner::calculateMatchPercentage() {
    int possibleKmersInReference = calculatePossibleKmersInReference();
    matchPercentage = 100 * ((double)totalNumberOfMatches / possibleKmersInReference);
}

int ReferenceAligner::calculatePossibleKmersInReference() {
    int possibleKmersInReference = 0;
    for (int seqSize : refSeqSizes) {
        possibleKmersInReference += (seqSize - kmerSize + 1);
    }
    return possibleKmersInReference;
}

Alignment ReferenceAligner::createAlignment() {
    Alignment alignment;
    alignment.referenceID = referenceID;
    alignment.numberOfSeqInRef = numberOfSeqInRef;
    alignment.refSeqSizes = refSeqSizes;
    alignment.sampleID = sampleID;
    alignment.sampleToRefMatches = matchesToReference;
    alignment.numberOfMatches = totalNumberOfMatches;
    alignment.matchPercentage = matchPercentage;   
    return alignment;
}

