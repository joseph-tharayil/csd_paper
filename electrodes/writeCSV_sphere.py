import MEAutility as MEA
import pandas as pd
import bluepysnap as bp
import sys
from bluerecording.utils import alignmentInfo, getAtlasInfo
import numpy as np
from scipy.spatial.transform import Rotation as R

'''
This script creates a csv file for an electrode array with a line of electrodes, perpendicular to the long axis of the cortical column,
in each of Layers 6, 5, 4, and 3. The electrodes span a distance of 100 um with 2 um spacing
Each of the electrodes will calculate the objective sphere CSD, with a sphere of radius 10 um
'''

def repositionElectrode(probe,center,azimuth,elevation):

    probe.rotate([0,1,0],elevation*180/np.pi)
    probe.rotate([0,0,1],azimuth*180/np.pi)
    probe.move(center)

    return(probe)

def updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType):

    for p in electrodePositions[numElectrodes:]:
        electrodeTypeList.append(electrodeType)
        numElectrodes += 1

    return electrodeTypeList, numElectrodes


if __name__=='__main__':

    probe_name = sys.argv[1]
    path_to_simconfig = sys.argv[2]
    electrode_csv = sys.argv[3]

    probeList = []

    center, azimuth, elevation = alignmentInfo(path_to_simconfig,'hex0')

    index = 0

    kList = -2082/2 + np.array([350,700+525/2,700+525+190/2,700+525+190+353/2])

    for k in kList:

        for j in np.arange(-25,25):

            probeList.append([0,j*2,k])

    electrodePositions = np.array(probeList)

    r = R.from_euler('yz',[elevation,azimuth])

    electrodePositions = r.apply(electrodePositions)

    electrodePositions += center

    electrodeTypeList = []
    numElectrodes = 0
    

    electrodeType = 'ObjectiveCSD_Sphere'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)
 

    regionList, layerList = getAtlasInfo(path_to_simconfig, electrodePositions)

    electrodeData = pd.DataFrame(data=electrodePositions,columns=['x','y','z'])

    layerData = pd.DataFrame(data=layerList,columns=['layer'])

    regionData = pd.DataFrame(data=regionList,columns=['region'])
    
    electrodeTypeData = pd.DataFrame(data=electrodeTypeList,columns=['type'])

    data = pd.concat((electrodeData,layerData),axis=1)
    data = pd.concat((data,regionData),axis=1)
    data = pd.concat((data,electrodeTypeData),axis=1)

    data.to_csv(electrode_csv)
