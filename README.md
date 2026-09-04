# Information content and interpretation of CSD non-trivially depends on array density

This repository contains the code used in the paper "Information content and interpretation of CSD non-trivially depends on array density". Steps to reproduce the figures in that paper are as follows:

## System requirements

The generation of BlueRecording weight files and postprocessing of data can be run on a Linux system or on WSL. 

Simulations of neural activity can be run either on a linux system with slurm and the spack package manager, or in a Docker container. See the [Neurodamus](https://github.com/openbraininstitute/neurodamus) repo for more details.

## Installation
Download the model data from its [Zenodo repository](https://zenodo.org/records/11113043) and extract it into the `config` folder.

Install Neurodamus and BlueRecording according to the instructions in the [BlueRecording repository](github.com/BlueBrain/BlueRecording). 

## Basic workflow
This paper relies on the BlueRecording workflow described in [this paper](https://doi.org/10.1371/journal.pcbi.1013023) and [this repo](github.com/BlueBrain/BlueRecording). Briefly, to simulate LFP and objective CSD, the BlueRecording workflow involves
- Simulating one timestep from the circuit model in question, in order to generate a compartment report
- Interpolating segment positions from the circuit model
- Creating a "weights file" which describes the contribution of the transmembrane current from each segment to the desired signal (either LFP or obejctive CSD, at each electrode)
- Running a full neural simulation, which reads the weights file and reports the desired signal

## Reproducing the figures

### Reproducing Figure 1

In order to reproduce Figure 1c-e, run the notebook Analytic.ipynb. The formulae implemented in this notebook (Eq. 9 and 11 in the paper) are derived using the Mathematica notebook csds.nb and limitCase.nb, respectively.

### Reproducing Figures 2-6

**Note that you can skip many of the steps described in this section by downloading the postprocessed LFP data from [our Zeonodo repository](https://zenodo.org/records/14998743). Once that data is downloaded, you can skip directly to Step 2 in "Reproducing Figure 3", "Reproducing Figure 5" and "Reproducing Figure 6".**

In order to create electrode files to calculate the LFP signals from neural simulations, the positions of the neural segments in the model must be calculated:
- In the folder `electrodes`, run `launch.sh` to generate a 1-timestep compartment report (i.e., an h5 file that lists each of the neural segments in the model.
- In the same folder, run `GetPositions.sh` to interpolate the 3D positions of each of these neural segments

Then, generate weights files for electrode arrays with 20 $\mu m$ spacing, to calculate LFP and $o_DCSD$. This repo contains the code to generate several different weights files. One of these weights files, `coeffs_highRes.h5`, is used in the simulations contained in the folder `highRes/` and is used as an input to generate the weights file `derivative.h5` used in the simulations in the folder `secondDeriv/`. Another, `coeffs_radii.h5`, is used in the simulations in the folder `radii/`. The aforementioned arrays are used to generate the data in Figure 2 and in Supplementary Figure S1. In contrast, the simulations in the folder `hypothesisTesting/`, which are used to generate the data in most other figures, use the weights file `coeffs_all.h5`, or the weights file `derivatives_all.h5`, which is generated therefrom. This latter set of weights files is better aligned with the cortical column (differing from the first set by 13 degrees), and, for computational efficiency, only uses the central column of the circuit.

To generate the weights files:
   * a. In the folder `electrodes`, run `writeCSV_all.py`, `writeCSV_radii.py` or `writeCSV_highRes.py` to generate the csv file that defines the electrode array.
   * b. Run `run_initialize_all.py`, `run_initialize_radii.py` or `run_initialize_highRes.py` to initialize the h5 files
   * c. Run `run_write_weights_all.py`, `run_write_weights_radii.py` or `run_write_weights_highRes.py` to populate the h5 file created in the previous step. In order to obtain an output in a reasonable amount of time, you should run these scripts using MPI with as many threads as possible.
     
Finally, run the notebook `derivatives.ipynb`. This script generates a weights file that lists the contributions to the "standard" CSD from a unit current at each neural segment, by taking the negative second derivative of the coefficients calculated for the LFP in the previous step. The resulting weights files are named `derivative.h5` and `derivative_all.h5`.
  
#### Reproducing Figure 2

1. Run the Jupyter notebook `derivativeHistograms.py` to generate Figure 2.

#### Reproducing Figure 3

1. Launch the simulations in the folder `hypothesisTesting/nonshiftedPositions_rerun` by running the script `launch.sh` in each of the subfolders. LFP and $o_DCSD$ signal contributions from each cell in the simulation will be calculated.

2. Next, sum the LFP and $o_DCSD$ signals over cells by running the scripts `geteeg.py`, followed by `eegTotal.py` in this folder.

3. Finally, run the notebook `hypothesisTesting/rerun.ipynb` to generate the panels of Figure 3.

#### Reproducing Figure 4

1. Panels a-f of Figure 4 are generated by the notebook `hypothesisTesting/rerun.ipynb`
2. Panels g and h of Figure 4 are generated by running `plot_finite_differences.ipynb`

#### Reproducing Figure 5

1. Run the simulations in `hypothesisTesting/nonshiftedPositions_nnCSD`
2. Next, sum the LFP and $o_DCSD$ signals over cells by running the scripts `geteeg.py`, followed by `eegTotal.py` in this folder.
3. The panels of Figure 5 are generated by running the notebook `hypothesisTesting/rerun.ipynb`

#### Reproducing Figure 6

1. First, run the simulations in each of the following folders (assuming that you have already run the simulations in `hypothesisTesting/nonshiftedPositions_rerun`):
   * hypothesisTesting/noActive_50um
   * hypothesisTesting/noNoise_50um
   * hypothesisTesting/noRecurrentConnecvitity_50um
   * hypothesisTesting/noActive_withNoise_50um
2. In each of the folders listed above, sum the LFP and $o_DCSD$ signals over cells by running the scripts `geteeg.py`, followed by `eegTotal.py`
3. Generate the figures by running the notebook `hypothesisTesting/hypotheses.ipynb`

### Reproducing Supplementary Figures

#### Reproducing Figure S1
Assuming that you have already run the simulations in `hypothesisTesting/nonshiftedPositions_rerun`, simply run the notebook `plotLFP.ipynb`

#### Reproducing Figure S2
See [here](https://github.com/steevelaquitaine/spikebias/tree/master/assets/validation)

#### Reproducing Figure S3
1. Run the simulations in the folder `highRes`
2. Next, sum the LFP and $o_DCSD$ signals over cells by running the scripts `geteeg.py`, followed by `eegTotal.py` in this folder.
3. Run the notebook `csdCorrelations_radii.ipynb` to generate panel b
4. Run the notebook `csdCorrelations_radii_resting.ipynb` to generate panels a, c and d

# Citation
If you use this software, we kindly ask you to cite the following paper: [Tharayil et al. **(2025)** iCSD can produce spurious results in dense electrode arrays. *bioRxiv*](https://doi.org/10.1101/2025.05.02.651822)

# Acknowledgment
The development of this software was supported by funding to the Blue Brain Project, a research center of the École polytechnique fédérale de Lausanne (EPFL), from the Swiss government's ETH Board of the Swiss Federal Institutes of Technology.
