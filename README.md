# ALIZE-LIA_RAL-extensions

Tools and utilities for speaker recognition built using the ALIZE toolkit. 

> **Disclaimer**: This is not the official repository for the ALIZE/LIA_RAL platform and should not be considered as a stable version of the toolkit. The sole goal of this repository is to provide users familiar with ALIZE/LIA_RAL additional tools and utilities that can be useful in their work.

## Download and install 

In order to copy all sources into a local directory, start by doing (git must be installed first) :
```sh
$ git clone https://github.com/wa3dbk/ALIZE-LIA_RAL-extensions.git
$ cd ALIZE-LIA_RAL-extensions
```

Now, you should be able to see two directories : 
- **ALIZE_3.0 :** This directory corresponds to the ALIZE library. After compilation, a library file named libalize_xxx.a should have been produced (The xxx depnds on your OS).
- **LIA_RAL_3.0 :** This directory corresponds to the LIA_RAL utilities. 

Now you have to compile ALIZE first, then LIA_RAL (LIA_RAL uses the Alize library). You'll need autotools and automake to compile alize so if you don't have all the required binaries installed on your system (aclocal, automake and autoconf), you can run :


```sh
$ sudo apt-get install autotools-dev
$ sudo apt-get install automake
```

If you're using MacOS you'll probably need use brew of macPorts.

Now that everything is ready, you can compile Alize (these steps are mentioned in the README file):

```sh
$ cd ALIZE_3.0/
$ aclocal
$ automake
$ autoconf
$ ./configure
$ make
```

Then, you can compile LIA_RAL by following the same steps:

```sh
$ cd LIA_RAL_3.0/
$ aclocal
$ automake
$ autoconf
$ ./configure --with-alize YourDirectory/ALIZE_3.0/ --enable-MT
$ make
```

Where :
- --with-alize : contains the absolute path to ALIZE_3.0/
- --enable-MT : enables the use of multithreading in LIA_RAL binaries

If nothing went wrong in the previous step, you should have all LIA_RAL binaries under LIA_RAL_3.0/bin/.


## ALIZE Utilities 

#### ReadMat
Allows to read any ALIZE binary file corresponding to a matrix or an i-vector (.matx and .y files). 

Example of use: 
```bash
$ ./bin/ReadMat --inputFile ./mat/TV.matx --loadMatrixFormat DB
```

will read TV.matx to STDOUT. 


#### SaveApost
Computes and saves the posterior probability vectors for each speech frame with respect to a world model. 

Example of use: 
```bash
$ ./bin/SaveApost /SaveApost --inputFeatureFilename myfilename --inputWorldFilename ubm --inputModelFilename ubm --minLLK 200.0 --maxLLK 200.0 --loadMixtureFileExtension .gmm --loadMixtureFileFormat RAW --mixtureFilesPath ./
```


