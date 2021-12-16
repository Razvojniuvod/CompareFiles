# CompareFiles
Compares the binary data of 2 or more files each, byte-by-byte, and shows, which files data is matched or not.

# How does it work?
It compares the files with each other, and shows, whenever their data is matched, or not.
Since it uses direct data comparing instead of comparing generated hashes, 
you can be assured that the result is accurate and collision-free.

# For what can this program be used for?
- Validating files for signs of data corruption (if you have multiple copies).
- Comparing the differences of multiple copies of the same files, if you are searching for the one you that has different content from the bunch (to make it easier to know, which ones need to be deleted, for example).

# How to compile this program?
You will need a GCC-compatible C compiler, and the Make utility.
The compiling process is done over the terminal. If you are using Windows, this is done over MSYS2.

For the procedue itself:
- Go to the directory housing the "Makefile" file with your terminal.
- Write and execute in the terminal "make".

The compiled executable is located in the directory "executable".
For optimization purposes, it is compiled for the CPU instruction of the PC that is compiling the code.

# TODO list:
- More thorough status/error messages.
- Allow the reading of multiple filepaths from a newline-terminated text file, if only one file is provided as a terminal argument parameter.
- Add unit support for the "-bs" and "--buffer-size" arguments (for example, you want the buffers to be 64 kilobytes in size. With this implemented, you write it in the terminal as "-bs 64KB"). Not sure if comma's are going to be supported (probably not).
- Multi-language status/error message support.
- The option for showing, at which offset exactly the differences of a file compare pair start (probably a flag for more verbose output).
- Cross-platform filepath Unicode support (this is due to the limitations of the fopen function, specifically aimed at Windows).
- Possible ways to further speed-up block comparing (so far, increasing the buffer size is the only possible way).
- Include the options for installing and uninstalling the program within the Makefile.
- Provide multiple documented return values (showing: all matched (value 0), some matched (non-zero value), none matched (non-zero value), error (non-zero value)).
