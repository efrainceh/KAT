#ifndef GENOME_H
#define GENOME_H

#include <string>
#include <vector>
#include <map>
#include "textIndex.h"
#include "genomeParser.h"

typedef std::map<std::string, int> Kmers;
typedef std::vector<TextIndex> TextIndeces;

class Genome {
        
    protected:
        std::string ID;
        std::string shortName;
        std::string completeness;
        Sequences sequences;
        std::vector<int> seqSizes;

        bool infoVarAreEmpty();
        void unparseGenome(std::pair<FirstLine, Sequences> &parsedGenome);
        void calculateSeqSizes();

    public:
        Genome() { }

        void genomeFromFile(std::string file);
        void setGenomeInfo(std::string ID_, std::string shortName_, std::string completenes_);
        void setSequences(Sequences sequences_);
        std::string getID() { return ID; }
        std::string getShortName() { return shortName; }
        std::string getCompleteness() { return completeness; }
        Sequences getSequences() { return sequences; }
        std::vector<int> getSeqSizes() { return seqSizes; }
        int getNumberOfSequences() { return sequences.size(); }
    
};

class Reference : public Genome {

    private:
        TextIndeces textIndeces;
        char END_OF_LINE = '$';

        std::string addEndOfLine(std::string seq);

    public:
        Reference() { }

        void calculateTextIndex();
        TextIndeces getTextIndeces() { return textIndeces; }
        
};

class Sample : public Genome {

    private:
        int SMALLER_VALID_KMERSIZE = 4;
        int LARGER_VALID_KMERSIZE = 1001;
        int kmerSize;
        Kmers kmers;

        bool validKmerSize(int kmerSize_);
        void addSequenceKmers(Sequence sequence);

    public:
        Sample() { }

        void createKmers(int kmerSize_);
        int getKmerSize() { return kmerSize; }
        Kmers getKmers() { return kmers; }
        
};

#endif
