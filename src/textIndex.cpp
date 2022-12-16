#include "../include/textIndex.h"



void BaseText::initializeBaseText() {
    if (validText()) {
        textSize = text.size();
        toUpperText();
        createCharIndexMap();
        fillCharCounts();
    } else {
        // clear text
        throw std::invalid_argument("textIndex.h, initializeBaseText: Text missing correct END_OF_STRING ('$')");
    }
}

bool BaseText::validText() {
    int lastIndex = text.size() - 1;
    char lastCharacter = text[lastIndex];
    return lastCharacter == END_OF_TEXT;
}

void BaseText::toUpperText() {
    for (int i = 0; i < textSize; i++) {
        char character = text[i];
        char upperChar = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
        text[i] = upperChar;
    }
}

void BaseText::createCharIndexMap() {
    addCharToMap();
    int index = 0;
    for (auto &charIndexPair : charIndexMap) {
        charIndexPair.second = index;   // Updating the index
        index++;
    }
}

void BaseText::addCharToMap() {
    for (char character : text) {
        if (charIndexMap.find(character) == charIndexMap.end()) {
            charIndexMap[character] = 0;
        }
    }
}

void BaseText::fillCharCounts() {
    numOfDifChar = charIndexMap.size();
    charCounts.resize(numOfDifChar);
    for (char character : text) {
        int charIndex = getCharIndex(character);
        charCounts[charIndex]++;
    }
}

int BaseText::getCharIndex(char character) {
    try {
        return charIndexMap.at(character);
    }
    catch(std::out_of_range const & err) {
        std::cerr << "textIndex.h, getCharIndex: Character " << character << " not in charIndexMap." << std::endl;
        return -1;
    }
}

// -----------------------------------------------------------------------------------------------

std::vector<int> SuffixArrayBuilder::build() {
    reserveSpace();
    sortCharacters();
    computeCharClasses();
    length = 1;
    while (length < textSize) {
        sortDoubled();
        updateClasses();
        length *= 2;
    }
    return suffixArray;
}

void SuffixArrayBuilder::reserveSpace() {
    sortedSubstrings.resize(textSize);
    classes.resize(textSize);
    suffixArray.resize(textSize);
    reversedSuffixArray.resize(textSize);
}

void SuffixArrayBuilder::sortCharacters() {
    Counts addedCharCounts = cumulativeCharCounts();
    for (int i = textSize - 1; i >=0; i--) {
        int charIndex = getCharIndex(text[i]);
        int sortedIndex = --addedCharCounts[charIndex];
        sortedSubstrings[sortedIndex] = i;
        suffixArray[sortedIndex] = i;
        reversedSuffixArray[i] = sortedIndex;
    }
}

Counts SuffixArrayBuilder::cumulativeCharCounts() {
    Counts addedCharCounts = charCounts;
    for (int i = 1; i < charCounts.size(); i++) {
        addedCharCounts[i] = addedCharCounts[i] + addedCharCounts[i - 1];
    }
    return addedCharCounts;
}

void SuffixArrayBuilder::computeCharClasses() {
    for (int i = 1; i < textSize; i++){
        int index = sortedSubstrings[i];
        int nextIndex = sortedSubstrings[i - 1];
        classes[index] = computeClass(index, nextIndex);
    }
}

int SuffixArrayBuilder::computeClass(int index, int nextIndex) {
    if (text[index] != text[nextIndex]) {
        return classes[nextIndex] + 1;
    } else {
        return classes[nextIndex];
    }
}

void SuffixArrayBuilder::sortDoubled() {
    Counts cumulativeSubstringCounts = fillSubstringCounts();
    previousSuffixArray = suffixArray;
    for (int i = textSize- 1; i >=0; i--) {
        int start = calculateStart(i);
        int index = classes[start];
        cumulativeSubstringCounts[index]--;
        suffixArray[cumulativeSubstringCounts[index]] = start;
        reversedSuffixArray[start] = cumulativeSubstringCounts[index];
    }
}

int SuffixArrayBuilder::calculateStart(int textIndex) {
    return (previousSuffixArray[textIndex] - length + textSize) % textSize;
}

Counts SuffixArrayBuilder::fillSubstringCounts() {
    Counts substringCounts(textSize);
    for (int i = 0; i < textSize; i++) {
        substringCounts[classes[i]]++;
    }
    for (int i = 1; i < textSize; i++) {
        substringCounts[i] += substringCounts[i - 1];
    }
    return substringCounts;
}

void SuffixArrayBuilder::updateClasses() {
    previousClasses = classes;
    for (int i = 1; i < textSize; i++) {
        int current = suffixArray[i];
        int previous = suffixArray[i - 1];
        classes[current] = updateClass(current, previous);
    }
}

int SuffixArrayBuilder::updateClass(int current, int previous) {
    if (conditionOccurs(current, previous)) {
        return classes[previous] + 1;
    } else {
        return classes[previous];
    }
}

bool SuffixArrayBuilder::conditionOccurs(int current, int previous) {
    int mid = (current + length) % textSize;
    int midPrevious = (previous + length) % textSize;
    return (previousClasses[current] != previousClasses[previous] || previousClasses[mid] != previousClasses[midPrevious]);
}

// -----------------------------------------------------------------------------------------------

TextIndex TextIndexBuilder::build() {
    textIndex.charIndexMap = charIndexMap;
    // Suffix Array needs to be calculated before all other functions
    buildSuffixArray();
    reserveSpace();
    buildBWT();
    buildStarts();
    buildCharCountMatrix();
    buildLCP();
    return textIndex;
}

void TextIndexBuilder::buildSuffixArray() {
    SuffixArrayBuilder SA(text);
    SA.build();
    textIndex.suffixArray = SA.getSuffixArray();
    textIndex.reversedSuffixArray = SA.getReversedSuffixArray();
}

void TextIndexBuilder::reserveSpace() {
    textIndex.bwt.resize(textSize);
    textIndex.starts.resize(numOfDifChar);
    textIndex.charCountMatrix.resize(textSize + 1, std::vector<int> (numOfDifChar));
    textIndex.lcpArray.resize(textSize);
}


void TextIndexBuilder::buildBWT() {
    for (int i = 0; i < textSize; i++) {
        textIndex.bwt[i] = getIndexOfCharPreviousToCurrentChar(i);
    }
}

int TextIndexBuilder::getIndexOfCharPreviousToCurrentChar(int index) {
    return ((textIndex.suffixArray[index] + textSize - 1) % textSize);
}

void TextIndexBuilder::buildStarts() {
    for (auto charAndInt : charIndexMap) {
        int charIndex = charAndInt.second;
        if (charIndex == 0) {
            textIndex.starts[charIndex] = 0;
        } else {
            textIndex.starts[charIndex] = textIndex.starts[charIndex - 1] + charCounts[charIndex - 1];
        } 
    }
}

void TextIndexBuilder::buildCharCountMatrix() {
    std::vector<int> counts(numOfDifChar);
    for (int i = 0; i < textSize; i++) {
        char character = text[textIndex.bwt[i]];
        int charIndex = getCharIndex(character);
        counts[charIndex]++;
        textIndex.charCountMatrix[i + 1] = textIndex.charCountMatrix[i];    // Next row equals current row
        textIndex.charCountMatrix[i + 1][charIndex] = counts[charIndex];
    }
}    

void TextIndexBuilder::buildLCP() {
    std::vector<bool> visited(textSize, false);
    for (int i = 0; i < textSize; i++) {
        if (!visited[i]) {
            visited[i] = true;
            int index = textIndex.reversedSuffixArray[i];
            int lcp = lcp_integer(index, index + 1, 0);
            textIndex.lcpArray[index] = lcp;
            while (lcp > 0) {
                i++;
                if (i >= textSize) {
                    textIndex.lcpArray[index] = 0;
                    break;
                }
                index = textIndex.reversedSuffixArray[i];
                lcp--;
                lcp = lcp_integer(index, index + 1, lcp);
            }
        }
    }
}

int TextIndexBuilder::lcp_integer(int index_1, int index_2, int lcp = 0) {
    int count = lcp;
    while (index_1 < textSize && index_2 < textSize && text[index_1 + lcp] == text[index_2 + lcp]) {
        index_1++;
        index_2++;
        count++;
    }
    return count;
}



// void TextIndexWriter::write() {
//     file << textIndex.info << std::endl;
//     writeVector(textIndex.SuffixArrayBuilder);
//     writeVector(textIndex.reversedSuffixArrayBuilder);
//     writeVector(textIndex.bwt);
//     writeVector(textIndex.starts);
//     writeMatrix(textIndex.charCountMatrix);
//     writeVector(textIndex.lcpArray);
// }

// void TextIndexWriter::writeVector(std::vector<int> &outputVector) {
//     file << outputVector.size() << std::endl;
//     file << outputVector << std::endl;
// }

// void TextIndexWriter::writeMatrix(Matrix &matrix) {
//     int numberOfRows = matrix.size();
//     int numberOfCols = matrix[0].size();
//     file << numberOfRows << " " << numberOfCols << std::endl;
//     for (std::vector<int> row : matrix) {
//         file << row << std::endl;
//     }
// }



// std::ostream& operator<<(std::ostream &os, std::vector<int> &v) {
//     for (int i : v) {
//         os << i << " ";
//     }
//     return os;
// }

// // void BWT::read(std::ifstream &file) {
// //     std::string line;
// //     if (getline(file, line)) {
// //         // I'm not sure why there's an extra line
// //     }
// //     if (getline(file, line)) {
// //         info = line;
// //     }
// //     int size, n;
// //     // Suffix Array
// //     file >> size;
// //     for (int i = 0; i < size; i++) {
// //         file >> n;
// //         suffix_array.sa.push_back(n);
// //     }
// //      // Reverse Suffix Array
// //     file >> size;
// //     for (int i = 0; i < size; i++) {
// //         file >> n;
// //         suffix_array.rsa.push_back(n);
// //     }
// //     // Last Column
// //     file >> size;
// //     for (int i = 0; i < size; i++) {
// //         file >> n;
// //         last_column.push_back(n);
// //     }
// //     // Starts
// //     file >> size;
// //     for (int i = 0; i < size; i++) {
// //         file >> n;
// //         starts.push_back(n);
// //     }
// //     // LCP Array
// //     file >> size;
// //     for (int i = 0; i < size; i++) {
// //         file >> n;
// //         lcp_array.push_back(n);
// //     }
// //     // Counts Matrix
// //     int rows, columns;
// //     file >> rows >> columns;
// //     counts_matrix.resize(rows, std::vector<int>(columns));
// //     for (int row = 0; row < rows; row++) {
// //         for (int col = 0; col < columns; col++) {
// //             file >> n;
// //             counts_matrix[row][col] = n;
// //         }
// //     }
// // }


