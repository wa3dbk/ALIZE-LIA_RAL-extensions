# ALIZE-LIA_RAL-extensions

Tools and utilities for speaker recognition built using the ALIZE toolkit. 

> **Disclaimer**: This is not the official repository for the ALIZE/LIA_RAL platform and should not be considered as a stable version of the toolkit. The sole goal of this repository is to provide users familiar with ALIZE/LIA_RAL additional tools and utilities which can be useful in their work. 

## Downlad and install 

In order to copy this repository's sources into a local directory, start by doing :
```sh
$ git clone https://github.com/wa3dbk/ALIZE-LIA_RAL-extensions.git .
$ cd ALIZE-LIA_RAL-extensions
```

Now, you should be able to see two directories : 
- **ALIZE_3.0 :** This directory corresponds to the ALIZE library. After compilation, a library file named libalize_xxx.a should have been produced (The xxx depnds on your OS).
- **LIA_RAL_3.0 :** This directory corresponds to the LIA_RAL utilities. 

## ALIZE Utilities 

#### ReadMat
Allows to read any ALIZE binary file corresponding to a matrix or an i-vector (.matx and .y files). 

Example of use: 
```bash
$ ./bin/ReadMat --inputFile ./mat/TV.matx --loadMatrixFormat DB
```

will read TV.matx to STDOUT. 

