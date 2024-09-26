# iCSD produces spurious results in dense electrode arrays

This repository contains the code used in the paper "iCSD generates spurious signals for dense electrode arrays". Steps to reproduce the figures in that paper are as follows:

## Installation
Download the model data from our [Zenodo repository](https://zenodo.org/records/11113043) and extract it into the `config` folder.

Install Neurodamus and BlueRecording according to the instructions in the [BlueRecording repository](github.com/BlueBrain/BlueRecording). This repo assumes that your system meets the requirements described there.

## Basic workflow
This paper relies on the BlueRecording workflow described in [this paper](https://www.biorxiv.org/content/10.1101/2024.05.14.591849v1) and [this repo]((github.com/BlueBrain/BlueRecording)). Briefly, to simulate LFP and objective CSD, the BlueRecording workflow involves
- Simulating one timestep from the circuit model in question, in order to generate a compartment report
- Interpolating segment positions from the circuit model
- Creating a "weights file" which describes the contribution of the transmembrane current from each segment to the desired signal (either LFP or obejctive CSD, at each electrode)
- Running a full neural simulation, which reads the weights file and reports the desired signal

## Reproducing Figure 2

### Analytic investigation (Figure 2 a)
In order to reproduce Figure 2a, run the notebook Analytic.ipynb. The formula implemented in this notebook (Eq. 5 in the paper) is derived using the Mathematica notebook csds.nb

### In silico characteristic length of the current distribution (Figure 2 b)
First, generate the electrode array used to calculate $o_sCSD$ in the Neurodamus simulation. A full explanation of this process is available in the [BlueRecording repository](github.com/BlueBrain/BlueRecording). 
- In the folder `electrodes`, run `launch.sh` to generate a compartment report for the circuit model
- Run `GetPositions.sh` to interpolate the segment position for each of the compartments in the model
- Run `WriteCSV_Sphere.sh` to generate the csv file that defines the electrode array.
- Run `InitializeSphereArray.sh` to initialize the h5 file read by Neurodamus
- Run `PopulateSphereArray.sh` to populate the h5 file created in the previous step.

Next, launch each of the simulations in the folder `osCSD` by running `launch.sh` in each of the subfolders. 

Then, launch the script `Geteeg.sh` in the `osCSD` folder to sum the recorded osCSD over all cells. 

Finally, run the notebook `osCSD_correlations.ipynb` to produce Figure 2b.

## Reproducing Figure 3
Generate electrode arrays with 40 $\mu$ m and 20 $\mu$ m spacing, to calculate LFP and $o_DCSD$
- In the folder `electrodes`, run `WriteCSV.sh` (respectively `WriteCSV_HighRes.sh`) to generate the csv file that defines the electrode array.
- Run `InitializeArray.sh` (respectively `InitializeArray_HighRes.sh`) to initialize the h5 file read by Neurodamus
- Run `PopulateArray.sh` (respectively `PopulateArray_HighRes.sh`) to populate the h5 file created in the previous step.

Then launch the simulations in the folders `lowRes` and `highRes` by running the script `launch.sh` in each of the subfolders.

Next, sum the LFP and $o_DCSD$ signals over cells by running the scripts `Geteeg.sh` in each of those folders.

Finally, run the notebook `csdCorrelations.ipynb` to generate Figure 3.

# Acknowledgment
The development of this software was supported by funding to the Blue Brain Project, a research center of the École polytechnique fédérale de Lausanne (EPFL), from the Swiss government's ETH Board of the Swiss Federal Institutes of Technology.
