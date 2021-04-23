# Using a denoising autoencoder for localization - Denoising cellular-based wireless localization data
Hello and welcome!\
This repository contains the code and scripts made for [Edvard Von Pfaler](linkedin.com/in/edvard-von-pfaler-a925671a9) and [Alexander Danielsson](https://www.linkedin.com/in/alexander-danielsson-b85068193/)'s bachelor thesis work for the course IA150X at KTH.\
The bachelor thesis evaluted how different parameters affected the performance of using a denoising
autoencoder (DAE) to remove noise in positioning data.

The repository contains three programs:
* datagen.cpp - tool to generate data for the experiments
* denoise1.py - script used for experiment 1
* denoise2.py - script used for experiment 2 and 3
* noiseloss.py - script that calculates the mean squared error between a noisy data set and a clean data set

## Description
### datagen.cpp
Generates training and evaluation data for the DAE consisting of two pairs of two files each.
Each file containing N distance vectors, and each of the two pairs (the training data pair, and the evaluation data pair) consisted of the following two files:
* Clean Data File - Each element in the distance vector coresponds to a true distance between the UE and one of the base stations
* Noisy Data File - The distance vector on line i is derived from the distance vector on line i in the clean training data, with each element having stochastic noise added to it

#### Usage:
datagen [-flag | --flag, value]
#### Flags:
  * -N, --numdata           the number of vector pairs in each set
  * -b, --numbasestations   the number of basestations
  * -w, --width             the width of the area
  * -h, --height            the height of the area
  * -c, --clean             name of the file for the clean training data
  * -n, --noisy             name of the file for the noisy training data
  * -vc, --validate_clean   name of the file for the clean validation data
  * -vn, --validate_noisy   name of the file for the noisy validation data
  
#### Customization:
The program defaults to stochastic noise with a number of paramaters defined on line 42-46. If another noise profile is
desired these lines need to be modifed (used values in the report are present as comments next to each line).
  
### denoise1.py
Iteratively initialize, trains and evaluates the performance of DAEs configured with different numbers of nodes
in its hidden layer (HL), starting with a DAE having 5 nodes per HL, and then iteratively incrementing the number of
nodes per HL by 5 for each iteration, up till a DAE with 200 nodes per HL is used.

#### Usage:
python denoise1.py

#### Customization:
The script defaults to using a DAE with only one HL, and must be modified manually by copying and pasting copies of line
76 to add more HLs. The number of copies of line 64 determines the number of HLs present in the DAE when the script is ran.

The script defaults to read the training data from the files "clean.dat" and "noisy.dat", and evaluation data from
"test.dat" and "noisytest.dat". If you want the script to read the data from some other files, please change lines 16-25 accordingly.

### denoise2.py
Iteratively initialize, trains and evaluates the performance of DAEs configured with different numbers of nodes
in its hidden layer (HL), starting with a DAE having 20 nodes per HL, then trying one with 45, followed by one with 100.

#### Usage:
python denoise2.py

#### Customization:
The script defaults to using a DAE with only one HL, and must be modified manually by copying and pasting copies of line
89 to add more HLs. The number of copies of line 72 determines the number of HLs present in the DAE when the script is ran.

The script defaults to read the training data from the files "clean.dat" and "noisy.dat", and evaluation data from
"test.dat" and "noisytest.dat". If you want the script to read the data from some other files, please change lines 16-25 accordingly.

### noiseloss.py
A script that takes a pair of files consisting of clean and noisy distance vectors and then calculates the mean square
error for the data set.

#### Usage:
python noiseloss.py

#### Customization:
The script defaults to read the data from the files "test.dat" and "noisytest.dat". If you want the script to read
the data from some other files, please change lines 8-11 accordingly.
