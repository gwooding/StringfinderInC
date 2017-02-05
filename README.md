# StringfinderInC

This is a simple program I wrote whilst learning about IO in C.  Currently it allows a user to look up whether words exist in groups of files (the filepaths are hardcoded for now).  The program can read in strings of unknown length in batch sizes set by the user.  The user can also choose AND or OR operators for querying multiple words at a time.

To run the code using make:  
make Logfind

Example:
Digest files 500 characters at a time, logging when any word in the list is found in the files  
./Logfind 500 -o word1 word2 word3

Example:
Digest files 400 characters at a time, logging when all words in the list are found in the files  
./Logfind 400 word1 word2 word3



