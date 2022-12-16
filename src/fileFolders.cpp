#include "../include/fileFolders.h"



void Folder::readFilenames(std::string suffix) {
    filenames.clear();
    for (const std::filesystem::directory_entry &file : std::filesystem::directory_iterator(pathToFolder)) {
        std::string filename = extractFilename(file);
        std::string filenameSuffix = extractSuffix(filename);
        if (filenameSuffix == suffix) {
            filenames.push_back(filename);
        }
    }
}

std::string Folder::extractFilename(const std::filesystem::directory_entry file) {
    std::filesystem::path pathFilename = file.path().filename();
    return pathFilename.string();
}

std::string Folder::extractSuffix(std::string filename) {
    int suffixStart = filename.find_last_of(SUFFIX_START);
    int suffixLength = filename.length() - suffixStart;
    return filename.substr(suffixStart, suffixLength);
}

std::string Folder::createPathForFile(std::string filename) {
    return pathToFolder.string() + PATH_SEPARATOR + filename;;
}

// -----------------------------------------------------------------------------------------------

void ReferenceFolder::readFiles(std::string suffix) {
    references.clear();
    readFilenames(suffix);
    for (std::string filename : filenames) {
        std::string currentFile = createPathForFile(filename);
        Reference reference;
        reference.genomeFromFile(currentFile);
        references.push_back(reference);
    }
}

void ReferenceFolder::writeFiles(std::string suffix) {
    return;
}

// -----------------------------------------------------------------------------------------------

void SampleFolder::readFiles(std::string suffix) {
    samples.clear();
    readFilenames(suffix);
    for (std::string filename : filenames) {
        std::string currentFile = createPathForFile(filename);
        Sample sample;
        sample.genomeFromFile(currentFile);
        samples.push_back(sample);
    }
}

void SampleFolder::writeFiles(std::string suffix) {
    return;
}