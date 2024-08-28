import numpy as np
import h5py
from glob import glob
from tqdm import tqdm
import xarray as xr
import os, sys
import scipy
import pandas as pd
from scipy.ndimage import gaussian_filter
from sklearn.decomposition import PCA
from scipy import stats
from mpi4py import MPI
from scipy import interpolate

from allensdk.brain_observatory.ecephys.ecephys_project_cache import EcephysProjectCache

from allenAnalysis.utils import *

from allenAnalysis.icsd import StandardCSD

def calculateCsd(std_input):

    csd_dict = dict(
        std_csd = StandardCSD(**std_input),
    )
    
    for method, csd_obj in csd_dict.items():
                
        #plot raw csd estimate
        csd = csd_obj.get_csd()
        csd = csd_obj.filter_csd(csd)

    return csd

def getSinksAndSources(csd):
    
    sinks_objective = np.zeros_like(csd)
    sources_objective = np.zeros_like(csd)
    mask_sinks_objective = csd<0
    mask_sources_objective = csd > 0
    
    sinks_objective[mask_sinks_objective] = np.abs(csd[mask_sinks_objective])
    sources_objective[mask_sources_objective] = np.abs(csd[mask_sources_objective])
    
    sinks_objective /= sinks_objective.sum()
    sources_objective /= sources_objective.sum()

    return sinks_objective, sources_objective

def getDistance(csd1, csd2):

    sinks1, sources1 = getSinksAndSources(csd1)
    sinks2, sources2 = getSinksAndSources(csd2)

    sinks_mat, _, _, _, _ = compute_dist_matrix_exp([sinks1], [sinks2], mode = 'exp_to_reco')
    sources_mat, _, _, _, _ = compute_dist_matrix_exp([sources1], [sources2], mode = 'exp_to_reco')

    distance = sinks_mat+sources_mat

    return distance

def upscaleCsd(csd,factor):

    if (factor & (factor-1) == 0) and factor != 0: # checks if factor is a power of 2
        pass
    else:
        raise ValueError('Factor must be power of 2')
        

    
    for iteration in range(int(factor/2)):
        
        newCsd = np.zeros([csd.shape[0]*2-1,csd.shape[1]])

        for i in range(newCsd.shape[0]):
            if i%2 == 0:
                newCsd[i] = csd[int(i/2)]
            else:
                newCsd[i] = ( csd[int( (i-1)/2 )] + csd[int( (i+1)/2 )] ) / 2

        csd = newCsd
        

    if csd.shape[0] != 51:
        csd = np.insert(csd,[-1],csd[-1],axis=0)
        csd = np.insert(csd,[-1],csd[-1],axis=0)

    return csd
        

if __name__=='__main__':
    

    ## Loads signals
    typesL = np.load('EEGs.npy',allow_pickle=True).item()
    totalL = np.load('total.npy')
    # totalL = totalL[0:len(totalL):10]

    totalL = totalL[2000:2050]
    
    lfpIdx = np.arange(51)
    sphere_fullDensity_idx = np.arange(51,102)
    disk_fullDensity_idx = np.arange(102,153)
    sphere_halfDensity_idx = np.arange(153,179)
    disk_halfDensity_idx = np.arange(179,205)
    sphere_quarterDensity_idx = np.arange(205,218)
    disk_quarterDensity_idx = np.arange(218,231)
    
    totalLFP = totalL[:,lfpIdx]
    objective_sphere_full = totalL[:,sphere_fullDensity_idx]
    objective_disk_full = totalL[:,disk_fullDensity_idx]
    objective_sphere_half = totalL[:,sphere_halfDensity_idx]
    objective_disk_half = totalL[:,disk_halfDensity_idx]
    objective_sphere_quarter = totalL[:,sphere_quarterDensity_idx]
    objective_disk_quarter = totalL[:,disk_quarterDensity_idx]
    
    ### Calculates CSD 
    
    lfp_data = totalLFP.T * 1E-3 * pq.V        # [mV] -> [V]
    
    z_data = np.arange(0,51) * 1e-6 * pq.m
    sigma = 0.277 * pq.S / pq.m                         # [S/m] or [1/(ohm*m)]
    sigma_top = 0.277 * pq.S / pq.m                     # [S/m] or [1/(ohm*m)]
    
    std_input = {
        'lfp' : lfp_data,
        'coord_electrode' : z_data,
        'sigma' : sigma,
        'f_type' : 'gaussian',
        'f_order' : (3, 1),
    }

    csd_full = calculateCsd(std_input)
    ####
    lfp_data = totalLFP[:,0:totalLFP.shape[1]:2].T * 1E-3 * pq.V        # [mV] -> [V]
    
    z_data = np.arange(0,51,2) * 1e-6 * pq.m
    sigma = 0.277 * pq.S / pq.m                         # [S/m] or [1/(ohm*m)]
    sigma_top = 0.277 * pq.S / pq.m                     # [S/m] or [1/(ohm*m)]
    
    std_input = {
        'lfp' : lfp_data,
        'coord_electrode' : z_data,
        'sigma' : sigma,
        'f_type' : 'gaussian',
        'f_order' : (3, 1),
    }

    csd_half = calculateCsd(std_input)

    ####
    lfp_data = totalLFP[:,0:totalLFP.shape[1]:4].T * 1E-3 * pq.V        # [mV] -> [V]
    
    z_data = np.arange(0,51,4) * 1e-6 * pq.m
    sigma = 0.277 * pq.S / pq.m                         # [S/m] or [1/(ohm*m)]
    sigma_top = 0.277 * pq.S / pq.m                     # [S/m] or [1/(ohm*m)]
    
    std_input = {
        'lfp' : lfp_data,
        'coord_electrode' : z_data,
        'sigma' : sigma,
        'f_type' : 'gaussian',
        'f_order' : (3, 1),
    }

    csd_quarter = calculateCsd(std_input)

    ####### Calculate distances

    othercsds = [upscaleCsd(csd_half,2), upscaleCsd(csd_quarter,4), objective_sphere_full.T, upscaleCsd(objective_sphere_half.T,2) , upscaleCsd(objective_sphere_quarter.T,4), objective_disk_full.T, upscaleCsd(objective_disk_half.T,2), upscaleCsd(objective_disk_quarter.T,4)]
    

    distances = []

    for csd in othercsds:
        distances.append(getDistance(csd_full,csd))

    np.save('distances.npy',distances)
    
