import MEAutility as MEA
import pandas as pd
import bluepysnap as bp
import sys
from bluerecording.utils import alignmentInfo, getAtlasInfo
import numpy as np
from scipy.spatial.transform import Rotation as R

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

            # probe = MEA.return_mea(probe_name)

            # probe.move([i*40,j*40,0])

            # repositionElectrode(probe, center, azimuth, elevation)

            # if index == 0:

            #     electrodePositions = probe.positions[23:74]
            # else:
            #     electrodePositions = np.vstack((electrodePositions,probe.positions[23:74]))

            # index += 1



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
