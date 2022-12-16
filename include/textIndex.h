#ifndef TEXTINDEX_H
#define TEXTINDEX_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

typedef std::vector<std::vector<int>> Matrix;
typedef std::map<char, int> CharIndexMap;
typedef std::vector<int> Counts;

struct TextIndex {

    std::string info;
    CharIndexMap charIndexMap;
    std::vector<int> suffixArray;
    std::vector<int> reversedSuffixArray;
    std::vector<int> bwt;
    std::vector<int> starts;
    Matrix charCountMatrix;
    std::vector<int> lcpArray;

};

class BaseText {

    protected:
        char END_OF_TEXT = '$';
        std::string text;
        int textSize;
        int numOfDifChar;
        CharIndexMap charIndexMap;
        Counts charCounts;

        bool validText();
        void initializeBaseText();
        void toUpperText();
        void createCharIndexMap();
        void addCharToMap();
        int getCharIndex(char character);
        void fillCharCounts();

    public:
        BaseText(std::string text_) : text(text_) {
            initializeBaseText(); 
            // try {
            //     initializeBaseText();
            // } catch (std::invalid_argument const &err) {
            //     std::cerr << err.what() << std::endl;
            // }
        }

        std::string getText() { return text; }
        CharIndexMap getCharIndexMap() { return charIndexMap; }

};

class SuffixArrayBuilder : public BaseText {
// IT CANNOT HANDLE TEXT WITH !, ", #, AND $. ALL LOWER ASCII THAN $

    private:
        int length;
        std::vector<int> sortedSubstrings;
        std::vector<int> previousClasses;
        std::vector<int> classes;
        std::vector<int> previousSuffixArray;
        std::vector<int> suffixArray;
        std::vector<int> reversedSuffixArray;

        void reserveSpace();
        void sortCharacters();
        Counts cumulativeCharCounts();
        void computeCharClasses();
        int computeClass(int index, int nextIndex);
        void sortDoubled();
        int calculateStart(int textIndex);
        Counts fillSubstringCounts();
        void updateClasses();
        int updateClass(int current, int previous);
        bool conditionOccurs(int current, int previous);

    public:
        SuffixArrayBuilder(std::string const text_) : BaseText(text_) { }

        std::vector<int> build();
        std::vector<int> getSuffixArray() { return suffixArray; }
        std::vector<int> getReversedSuffixArray() { return reversedSuffixArray; }

};

class TextIndexBuilder : public BaseText {

    private:
        TextIndex textIndex;

        void buildSuffixArray();
        void reserveSpace();
        void buildBWT();
        int getIndexOfCharPreviousToCurrentChar(int index);
        void buildStarts();
        void buildCharCountMatrix();
        void buildLCP();
        int lcp_integer(int index_1, int index_2, int lcp);

    public:
        TextIndexBuilder(std::string text_) : BaseText(text_) { };
        
        TextIndex build();
        std::string getText() { return text; }
         
};

#endif