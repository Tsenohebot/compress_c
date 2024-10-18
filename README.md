# compress_c

<h3>Steps:</h3>

1. Clone Repo
2. Run make
3. Builds executable in build/

<h3>Usage:</h3>

Make:

~~~
    make <option> :
        default (no option) : Builds binary and places into build/, creates build/ if build does not exist.
        clean : Cleans all contents of build/
        nuke : Deletes entire build/ directory
~~~

Executable:

~~~

./HUFFMAN <option> <input> <output>

options: 
    -c  :Compress input file, create output file.
    -d  :Decompress input file, create output file.
~~~