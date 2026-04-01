# SPDX-License-Identifier: GPL-3.0-or-later
import sys
import pandas as pd
from bluerecording.writeH5 import writeH5File
from bluerecording.utils import process_writeH5_inputs

if __name__=='__main__':


    path_to_simconfig = 'simulation_config.json' # simulation_config.json with one-timestep compartment report
    segment_position_folder = 'positions/' # Folder with segment positions; output of getPositions.py
    outputfile = 'coeffs_20.h5' 
    
    neurons_per_file = 1000

    numFilesPerFolder = 50 # Number of files per subfolder in segment positions folder

    conductivity = [0.277]
    circuitSubvolume = 'hex0'

    writeH5File(path_to_simconfig,segment_position_folder,outputfile,neurons_per_file,numFilesPerFolder,conductivity,circuitSubvolume,None,None) 
