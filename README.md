#The program is solution to the following problem:

Write program that counts the number of distinct unique words in a file whose name is passed as an argument to a program.
For example, given the file content "a horse and a dog" the program must output "4" (the word 'a' appears twice but only accounts for one distinct unique occurrence).
The input text is guaranteed to contain only 'a'..'z' and space characters in ASCII encoding.
The program should be able to handle large inputs (e.g. 32 GiB)
One can assume that all unique words fit into memory when using the chosen data structure.
The solution must utilize all available CPU resources.

#Usage:
- Compilation: make
- Binary name: keepit
- Use '-h' to display all available options.     

#Description of the code:

The code has been written under three main assumptions:
- it should be used on Linux systems only,
- no libraries apart from STL should be required (although BOOST already provides functionalities e.g. file memory mapping),
- it should be self-explanatory (therefore no comments).

The file specified with option '-f' is mapped into memory by the FileHandler. Memory mapping for large files is more efficient than reading data to buffer. Through the TextAnalyzer object, number N of consecutive threads wrapped inside AlgorithmThreadWrapper (where N is passed to program as an argument '-j N'; by default optimally max) is running the algorithm (IAnalyzerAlgorithm) specified with '-a' option over the text, which is fetched from the FileHandler in a thread-safe way. After all the threads are done, the TextAnalyzer merges results from all threads and displays the number of unique words found in the file.

#Known limitations:

- Currently available version of the code does not contain implemention of the Trie algorithm (option '-a 2'),
- Some corner cases are not yet handled e.g. no memory available,
- Option parser does not handle cases when the option value is missing e.g. '-f' without filepath (did not focus on parsing).