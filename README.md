# findNew
Similar to the famous command line "ls" and "dir". It lists all files/folders of a directory.

You can save list of your current directory in a text file. In the future, save another new list of the directory in another text file. Compare the two text files to find different files/folders.

How the program find difference?

1. It compares 2 text files.
2. It checks which file has more lines of contents and which has less.
3. It uses the file with less lines and starts to compare with the other one.
4. Everytime a same string of a same line is in both text files, its result will be hide.

