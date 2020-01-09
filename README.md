The code `make_sierpinski` generates instances of Sierpinski curves. All
vertices have integer coordinates.

# Compilation:
Any standard C compiler will do. E.g.:

    gcc -O2 -Wall -o make_sierpinski make_sierpinski.c

# Usage
    make_sierpinski --number <N> --output <OUTPUTFILE>

where N is a positive integer and OUTPUTFILE is the name of the output file.

# Generating polygonal data:

The runtime-option "--number N" specifies the order of the Sierpinski curve.
The curve is written to the file named XYZ, as specified in "--output XYZ".

Please direct bug reports or suggestions to Martin Held at held@cs.sbg.ac.at.
