# KAT: Kmer Alignment Tool

The Kmer Alignment Tool takes DNA genome sequences, choppes them up into kmers, and then aligns those kmers to Reference DNA files. For the alignment it uses a combination of Suffix Arrays and the Burrows-Wheeler Transform.

## Installation

The installation zip file includes a makefile. Just go to the folder where you downloaded the files and type `make` to build the project and generate the executable. The executable is called KAT1.0 and can now be found in the bin folder. 

## How to Use

### Input:

To program requires 4 arguments, so from the terminal you would run `path_to_executable 6 project_name ~/my_project_folder .fasta 20`

- **Path to executable**: the path to where you saved the KAT1.0 executable, e.g `~/bin/KAT1.0`
- **Number of inputs**: 6
- **Project Name**: this is the name that will be used to output table.
- **Project Folder path**: this folder should contain a Sample folder and a Reference folder, inside which you should have the fasta genome files.
- **Genome file extension**: currently only supports “.fasta”. However, the program can handle files with full genomes, contigs, and chromosomes.
- **kmer size**: This should be an integer between 5 and 1000. It represents the number of basepairs for chopping up the samples files.

![Project folder structure](/images/folder_structure.jpg)

### Output:

A Results folder is created inside the Project Folder. This folder contains:

- A project_name.csv file with the number of hits and hit percentage for each sample-reference pair. Percentage = 100 * (number of hits / number of unique kmers in the reference). If the reference has more than one sequence (contigs, chromosomes) then it calculates the number of unique kmers for all together, with kmers that are found in more than one sequence also counting as one unique kmer.

- A text file for each Reference-Sample pair that lists the hit kmers and their positions in the reference.

#### Table example:

![Table example](/images/table_example.jpg)

#### Textfile example: NZ_CP008781.1_NZ_CP012480.1.txt

Reference ID
Number of sequences in Reference file
Number of basepairs in each sequence in Reference file
Sample ID
Number of unique matches
Total number of matches
found kmer / index of sequence in reference / index of matches for Khmer in that specific sequence of reference

![Textfile examples: NZ_CP055292.1_NC_003155.5.txt](/images/textfile_example.jpg)

In the example, kmer AAAAAAAAAC is found in Reference sequence 0, positions 211595 2296603 1409627 1170432


