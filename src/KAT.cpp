#include "../include/KAT.h"



Results KAT::align() {
    std::cout << "Reading References..." << std::endl;
    readReferences();
    std::cout << "Calculating References text indeces..." << std::endl;
    calculateReferencesTextIndex();
    std::cout << "Reading Samples..." << std::endl;
    readSamples();
    std::cout << "Creating Samples kmers..." << std::endl;
    calculateSamplesKmers();
    std::cout << "Aligning..." << std::endl;
    alignAllSamplesToAllReferences();
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
    for (int indexOfSample = 0; indexOfSample < samples.size(); indexOfSample++) {
        samples[indexOfSample].createKmers(kmerSize);
    }
}

void KAT::alignAllSamplesToAllReferences() {
    for (Reference reference : references) {
        ReferenceAligner refAligner(reference);
        std::vector<int> totalMatchesToRefPerSample;
        for (Sample sample: samples) {
            Alignment refAlignment = refAligner.align(sample);
            result.alignments.push_back(refAlignment);
            totalMatchesToRefPerSample.push_back(refAlignment.numberOfMatches);
        }
        result.finalTable.push_back(totalMatchesToRefPerSample);
    }
}





