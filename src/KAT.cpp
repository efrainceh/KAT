#include "../include/KAT.h"

#include <chrono>


Results KAT::align() {
    auto start = std::chrono::high_resolution_clock::now();
    // std::cout << "Reading References..." << std::endl;
    readReferences();
    // std::cout << "Calculating References text indeces..." << std::endl;
    calculateReferencesTextIndex();
    // std::cout << "Reading Samples..." << std::endl;
    readSamples();
    // std::cout << "Creating Samples kmers..." << std::endl;
    calculateSamplesKmers();
    // std::cout << "Aligning..." << std::endl;
    alignAllSamplesToAllReferences();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << duration.count()<< std::endl;
    return result;
}

void KAT::readReferences() {
    std::string pathToReferences = input.pathToInputFolder + REFERENCES_FOLDER;
    ReferenceFolder referenceFolder(pathToReferences);
    referenceFolder.readFiles(input.filesSuffix);
    references = referenceFolder.getReferences();
    result.referenceFilenames = referenceFolder.getFilenames();
}

void KAT::calculateReferencesTextIndex() {
    for (int indexOfRef = 0; indexOfRef < references.size(); indexOfRef++) {
        references[indexOfRef].calculateTextIndex();
    }
}

void KAT::readSamples() {
    std::string pathToSamples = input.pathToInputFolder + SAMPLES_FOLDER;
    SampleFolder sampleFolder(pathToSamples);
    sampleFolder.readFiles(input.filesSuffix);
    samples = sampleFolder.getSamples();
    result.sampleFilenames = sampleFolder.getFilenames();
}

void KAT::calculateSamplesKmers() {
    int kmerSize = input.kmerSize;
    result.kmerSize = input.kmerSize;
    for (int indexOfSample = 0; indexOfSample < samples.size(); indexOfSample++) {
        samples[indexOfSample].createKmers(kmerSize);
    }
}

void KAT::alignAllSamplesToAllReferences() {
    for (Reference reference : references) {
        ReferenceAligner refAligner(reference);
        std::vector<int> totalMatchesToRefPerSample;
        std::vector<double> totalPercentagesToRefPerSample;
        for (Sample sample: samples) {
            Alignment refAlignment = refAligner.align(sample);
            result.alignments.push_back(refAlignment);
            totalMatchesToRefPerSample.push_back(refAlignment.numberOfMatches);
            totalPercentagesToRefPerSample.push_back(refAlignment.matchPercentage);
        }
        result.hitTable.push_back(totalMatchesToRefPerSample);
        result.percentageTable.push_back(totalPercentagesToRefPerSample);
    }
}





