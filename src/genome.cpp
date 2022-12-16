#include "../include/genome.h"



void Genome::setGenomeInfo(std::string ID_, std::string shortName_, std::string completenes_) {
    if (infoVarAreEmpty()) {
        ID = ID_;
        shortName = shortName_;
        completeness = completenes_;
    } else {
        throw std::invalid_argument("Cannot change existing genome variables");
    } 
}

bool Genome::infoVarAreEmpty() {
    return (ID.empty() && shortName.empty() && completeness.empty());
}

void Genome::setSequences(Sequences sequences_) {
    if (sequences.empty()){
        sequences = sequences_;
    } else {
        throw std::invalid_argument("Cannot change existing genome sequences");
    }
}

void Genome::genomeFromFile(std::string file) {
    GenomeParser genomeParser(file);
    std::pair<FirstLine, Sequences> parsedGenome = genomeParser.parse();
    unparseGenome(parsedGenome);
    calculateSeqSizes();
}

void Genome::unparseGenome(std::pair<FirstLine, Sequences> &parsedGenome) {
    ID = parsedGenome.first.ID;
    shortName = parsedGenome.first.shortName;
    completeness = parsedGenome.first.completeness;
    sequences = parsedGenome.second;
}

void Genome::calculateSeqSizes() {
    for (Sequence sequence : sequences) {
        int seqSize = sequence.dna.size();
        seqSizes.push_back(seqSize);
    }
}

// -----------------------------------------------------------------------------------------------

void Reference::calculateTextIndex() {
    for (Sequence sequence : sequences) {
        std::string dnaSequence = addEndOfLine(sequence.dna); 
        TextIndexBuilder TIB(dnaSequence);
        TextIndex TI = TIB.build();
        textIndeces.push_back(TI);
    }
}

std::string Reference::addEndOfLine(std::string seq) {
    return seq + END_OF_LINE;
}

// -----------------------------------------------------------------------------------------------

void Sample::createKmers(int kmerSize_) {
    if (validKmerSize(kmerSize_) && !sequences.empty()) {
        kmerSize = kmerSize_;
        kmers.clear();
        for (Sequence sequence : sequences) {
            addSequenceKmers(sequence);
        }
    } else {
        throw std::invalid_argument("Invalid kmerSize and/or empty sequences");
    }
}

bool Sample::validKmerSize(int kmerSize_) {
    return (kmerSize_ > SMALLER_VALID_KMERSIZE && kmerSize_ < LARGER_VALID_KMERSIZE);
}

void Sample::addSequenceKmers(Sequence sequence) {
    for (int i = 0; i <= sequence.dna.size() - kmerSize; i++) {
        std::string kmer = sequence.dna.substr(i, kmerSize);
        kmers[kmer] = 1;
    }
}