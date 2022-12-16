#ifndef FILEFOLDER_H
#define FILEFOLDER_H

#include <string>
#include <vector>
#include <filesystem>
#include "genome.h"

typedef std::vector<std::string> Filenames;
typedef std::vector<Genome> Genomes;
typedef std::vector<Reference> References;
typedef std::vector<Sample> Samples;

class Folder {

    protected:
        char SUFFIX_START = '.';
        char PATH_SEPARATOR = '/';
        const std::filesystem::path pathToFolder;
        Filenames filenames;

        void readFilenames(std::string suffix);
        std::string extractFilename(const std::filesystem::directory_entry file);
        std::string extractSuffix(std::string filename);
        std::string createPathForFile(std::string filename);

    public:
        Folder(std::string path) : pathToFolder(path) { }
        virtual ~Folder() { }

        Filenames getFilenames() { return filenames; }
        int numberOfFiles() { return filenames.size(); }
        virtual void readFiles(std::string suffix) = 0;
        virtual void writeFiles(std::string suffix) = 0;

};

class ReferenceFolder : public Folder {

    private:
        References references;

    public:
        ReferenceFolder(std::string path) : Folder(path) { }
        
        References getReferences() { return references; }
        void readFiles(std::string suffix);
        void writeFiles(std::string suffix);
    
};

class SampleFolder : public Folder {

    private:
        Samples samples;

    public:
        SampleFolder(std::string path) : Folder(path) { }
        
        Samples getSamples() { return samples; }
        void readFiles(std::string suffix);
        void writeFiles(std::string suffix);
    
};

#endif
