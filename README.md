# Unix-Shell-in-C
This is a Unix Shell coded entirely in C that supports built-in commands and advanced shell features.
## Running The Shell
Run `make shell` in the command line in the same directory as all the other files to create the executable.
Run the executable in your command line environment to start using it. 

## Advanced Shell Features
- Sequencing  `echo one; echo two` &nbsp; &nbsp; (exectues command on left side and then executes command on right side)
- Input Redirection  `sort < foo.txt` &nbsp; &nbsp; (the command on the left side recieves its arguments from the text file on the right side)
- Output Redirection  `sort foo.txt < output.txt` &nbsp; &nbsp; (the output of the command on the left side goes to the text file on the right side)
- Pipes `sort foo.txt | uniq` &nbsp; &nbsp; (pipes the output of the left side command to the input of the right side command)

## Built-in Commands
- cd `cd [path]` &nbsp; &nbsp; (changes the current directory to the path specified)
- source `source [filename]` &nbsp; &nbsp; (executes a script by taking a file and executing each line in that file as a command)
- prev `prev` &nbsp; &nbsp; (prints the previous command line and executes it)
- help `help` &nbsp; &nbsp; (explains the available built-in commands)
