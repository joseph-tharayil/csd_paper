import sys
from bluerecording.writeH5_prelim import initializeH5File

if __name__=='__main__':

    '''
    path_to_simconfig refers to the simulation_config from the 1-timestep simulation used to get the segment positions
    electrode_csv is a csv file containing the position, region, and layer of each electrode
    type is either LineSource or Reciprocity
    '''

    electrode_csv = 'electrode_csv_highRes.csv'

    path_to_simconfig = 'simulation_config.json'

    outputfile = 'coeffs_highRes.h5'

    target = 'hex_O1'

    initializeH5File(path_to_simconfig,outputfile,electrode_csv,target)
