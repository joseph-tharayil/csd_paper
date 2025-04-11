# iCSD produces spurious results in dense electrode arrays

This repository contains the code used in the paper "iCSD generates spurious signals for dense electrode arrays". Steps to reproduce the figures in that paper are as follows:

## System requirements

We assume that you are running this code on a Linux system with slurm and the spack package manager. BlueRecording has not been tested on any other system. 

## Installation
Download the model data from our [Zenodo repository](https://zenodo.org/records/11113043) and extract it into the `config` folder.

Install Neurodamus and BlueRecording according to the instructions in the [BlueRecording repository](github.com/BlueBrain/BlueRecording). This repo assumes that your system meets the requirements described there.

## Basic workflow
This paper relies on the BlueRecording workflow described in [this paper](https://www.biorxiv.org/content/10.1101/2024.05.14.591849v1) and [this repo]((github.com/BlueBrain/BlueRecording)). Briefly, to simulate LFP and objective CSD, the BlueRecording workflow involves
- Simulating one timestep from the circuit model in question, in order to generate a compartment report
- Interpolating segment positions from the circuit model
- Creating a "weights file" which describes the contribution of the transmembrane current from each segment to the desired signal (either LFP or obejctive CSD, at each electrode)
- Running a full neural simulation, which reads the weights file and reports the desired signal

## Reproducing Figure 1

In order to reproduce Figure 1c-e, run the notebook Analytic.ipynb. The formulae implemented in this notebook (Eq. 5 and 6 in the paper) are derived using the Mathematica notebook csds.nb and limitCase.nb, respectively.

## Reproducing Figures 2-4

**Note that you can skip many of the steps described in this section by downloading the postprocessed LFP data from [our Zeonodo repository](https://zenodo.org/records/14998743). Once that data is downloaded, you can skip directly to Step 4 in "Reproducing Figrue 2" and to Step 7 in "Reproducing Figures 3 and 4".**

In order to create electrode files to calculate the LFP signals from neural simulations, the positions of the neural segments in the model must be calculated:
- In the folder `electrodes`, run `launch.sh` to generate a 1-timestep compartment report (i.e., an h5 file that lists each of the neural segments in the model.
- In the same folder, run `GetPositions.sh` to interpolate the 3D positions of each of these neural segments

### Reproducing Figure 2
1. Generate electrode arrays with 20 $\mu m$ spacing, to calculate LFP and $o_DCSD$
   * a. In the folder `electrodes`, run `WriteCSV_HighRes.sh` to generate the csv file that defines the electrode array.
   * b. Run `InitializeArray_HighRes.sh` to initialize the h5 file read by Neurodamus
   * c. Run `PopulateArray_HighRes.sh` to populate the h5 file created in the previous step.

2. Then launch the simulations in the folder `highRes` by running the script `launch.sh` in each of the subfolders. LFP and $o_DCSD$ signal contributions from each cell in the simulation will be calculated.

3. Next, sum the LFP and $o_DCSD$ signals over cells by running the script `Geteeg.sh` in the `highRes` folder.

4. Finally, run the notebook `csdCorrelations.ipynb` to generate Figure 2.

### Reproducing Figures 3 and 4

1. Generate electrode arrays with 20 $\mu m$ spacing to calculate $o_DCSD$ with various radii $\rho$
- In the folder `electrodes`, run `WriteCSV_Radii.sh` to generate the csv file that defines the electrode array.
- Run `InitializeArray_Radii.sh` to initialize the h5 file read by Neurodamus
- Run `PopulateArray_Radii.sh` to populate the h5 file created in the previous step.

2. Then launch the simulations in the folder `radii` by running the script `launch.sh` in each of the subfolders.

3. Next, sum the $o_DCSD$ signals over cells by running the script `Geteeg.sh` in the folder `radii`

4. Run the notebook `derivatives.ipynb` to generate the weights file that calculated non-negative CSD (nnCSD). Briefly, this notebook takes the negative second derivative of the LFP weights calculated above, and zeros out the negative values.

5. Launch the simulations in the folder `secondDeriv` by running the script `launch.sh` in each of the subfolders; this will calculate the nnCSD.

6. Next, sum the $nnCSD$ signals over cells by running the script `Geteeg.sh` in the folder `secondDeriv`

7. Finally, run the notebook `csdCorrelations_radii.ipynb` to generate Figures 3 and 4.

# Acknowledgment
The development of this software was supported by funding to the Blue Brain Project, a research center of the École polytechnique fédérale de Lausanne (EPFL), from the Swiss government's ETH Board of the Swiss Federal Institutes of Technology.
