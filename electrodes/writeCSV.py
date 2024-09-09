import MEAutility as MEA
import pandas as pd
import bluepysnap as bp
import sys
from bluerecording.utils import alignmentInfo, getAtlasInfo
import numpy as np

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

    probe = MEA.return_mea(probe_name)

    center, azimuth, elevation = alignmentInfo(path_to_simconfig,'hex0')

    repositionElectrode(probe, center, azimuth, elevation)

    electrodePositions = probe.positions[23:74]

    electrodeTypeList = []
    numElectrodes = 0
    
    electrodeType = 'LineSource'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)

    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74]))
    electrodeType = 'ObjectiveCSD_Sphere'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)

        
    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74]))
    electrodeType = 'ObjectiveCSD_Disk'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)

    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74:2]))
    electrodeType = 'ObjectiveCSD_Sphere'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)
    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74:2]))
    electrodeType = 'ObjectiveCSD_Disk'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)

    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74:4]))
    electrodeType = 'ObjectiveCSD_Sphere'
    electrodeTypeList, numElectrodes = updateTypeList(electrodeTypeList, numElectrodes, electrodePositions, electrodeType)
    electrodePositions = np.vstack((electrodePositions,probe.positions[23:74:4]))
    electrodeType = 'ObjectiveCSD_Disk'
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
