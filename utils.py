import scipy.signal as ss
from glob import glob
import numpy as np
import os
import sys
from scipy.ndimage import gaussian_filter
from cinplaAnalysis import icsd
import quantities as pq
from tqdm import tqdm
import matplotlib.pyplot as plt
import ot

def get_mua(ecp, filter_order = 5, fs = 20000, fc = 500, q = 20):
    '''
        This function gives you the MUA from the ECP
        Parameters
        ---------
                ecp : extracellular potential
                filter_order : order of butterworth filter
                fs : sampling frequency (Hz)
                fc : cut-off frequency
                q : downsampling order
        Returns
        ---------
                mua : multi-unit activity
    '''
    # creating high-pass filter
    Wn = fc/fs/2
    
    b, a = ss.butter(filter_order, Wn, btype = 'highpass')
    
    mua = ss.filtfilt(b, a, ecp, axis = 0)

    # downsample to 1 kHz
    for q_ in [10, q // 10]:
        mua = ss.decimate(mua, q_, axis = 0)
    
    mua = abs(mua)
    
    return mua


def get_lfp(ecp, filter_order = 5, fs = 10000, fc = 500, q = 20, downsample = True, do_filter = True):
    '''
        This function gives you the LFP from the ECP
        Parameters
        ---------
                ecp : extracellular potential
                filter_order : order of butterworth filter
                fs : sampling frequency (Hz)
                fc : cut-off frequency
                q : downsampling order
        Returns
        ---------
                lfp : local field potential
    '''
    if do_filter:
        # creating high-pass filter
        Wn = fc/fs/2

        b, a = ss.butter(filter_order, Wn, btype = 'low')

        lfp = ss.filtfilt(b, a, ecp, axis = 0)
    else:
        lfp = ecp

    if downsample:
        for q_ in [10, q // 10]:
            lfp = ss.decimate(lfp, q_, axis = 0)
    
    return lfp

def find_nearest(array, value):
    array = np.asarray(array)
    idx = (np.abs(array - value)).argmin()
    return idx, array[idx]

def get_stimulus_lfp(session, lfp, presentation_nr_start = 0, presentation_nr_stop = -1,\
                    time_before_stim_onset = 0.0, time_after_stim_offset = 0.0, stim_name = 'flashes'):
    '''
    This function extracts the experimental LFP during the presentation of a selected stimulus.
    Parameters
    ---------
            session : loaded nwb file with session data
            lfp : lfp from whole period
            presentation_nr_start : which stimulus presentation to start at
            presentation_nr_start : which stimulus presentation to stop at
            time_before_stim_onset : time before stimulus onset to include in each trial
            time_after_stim_onset : time after stimulus onset to include in each trial
            stim_name : stimulus key name
        Returns
        ---------
            lfp : local field potential
    '''
    
    stimulus_presentation = session.stimulus_presentations[
        session.stimulus_presentations['stimulus_name'] == stim_name
    ]
    
    start_time_stim = stimulus_presentation['start_time'].values[presentation_nr_start]+time_before_stim_onset
    
    stop_time_stim = stimulus_presentation['stop_time'].values[presentation_nr_stop]+time_after_stim_offset
    
    idx_start_stim = find_nearest(lfp.time.values, start_time_stim)[0]
    idx_stop_stim = find_nearest(lfp.time.values, stop_time_stim)[0]
    
    return lfp[idx_start_stim:idx_stop_stim]

def subtract_lfp_baseline_all_sims(lfp_orig, tstim_onset = 250, contributions = False, contributions_summed = False):
    '''
    This function subtracts LFP before stimulus onset (baseline) from LFP during stimulus presentations. 
    Parameters
    ---------
            lfp_orig : lfp before subtraction of baseline
            tstim_onset : time at which the stimulus is presented (ms)
            contributions : boolean that indicates whether this is cell type population contributions to total LFP
            contributions_summed : boolean that indicates whether this is population (contributions from excitatory
                                   and inhibitory cell types in each layer already summed) contributions to total LFP
        Returns
        ---------
            lfp_out : local field potential after subtracting baseline
    '''
        
    #TODO: Fix the stupid data organization that forces all these if-statements
    
    lfp_out = dict()

    if contributions:
        for sim_name in lfp_orig.keys():
            print(sim_name)
            lfp_dict_pops = dict()
            for pop_name in lfp_orig[sim_name].keys():
                lfp_dict = dict()
                
                lfp_temp = lfp_orig[sim_name][pop_name]['trial_avg'].T
                
                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
            
                lfp_dict['trial_avg'] = lfp_temp.T
                
                lfp_trials_temp = []
                for itrial in range(lfp_orig[sim_name][pop_name]['all_trials'].shape[1]):
                    
                    lfp_temp = lfp_orig[sim_name][pop_name]['all_trials'][:,itrial].T
                    
                    lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
                    
                    lfp_trials_temp.append(lfp_temp.T)
                
                lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
                lfp_dict_pops[pop_name] = lfp_dict
                                    
            lfp_out[sim_name] = lfp_dict_pops
            
    elif contributions_summed:
        for sim_name in lfp_orig.keys():
            print(sim_name)
            lfp_dict_pops = dict()
            for pop_name in lfp_orig[sim_name]['all_trials'].keys():
                lfp_dict = dict()
                
                lfp_temp = lfp_orig[sim_name]['trial_avg'][pop_name].T
                
                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
            
                lfp_dict['trial_avg'] = lfp_temp.T
                
                lfp_trials_temp = []
                for itrial in range(lfp_orig[sim_name]['all_trials'][pop_name].shape[1]):
                    
                    lfp_temp = lfp_orig[sim_name]['all_trials'][pop_name][:,itrial].T
                    
                    lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
                    
                    lfp_trials_temp.append(lfp_temp.T)
                
                lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
                lfp_dict_pops[pop_name] = lfp_dict
                                    
            lfp_out[sim_name] = lfp_dict_pops
    else:
        for sim_name in lfp_orig.keys():
            lfp_dict = dict()
            print(sim_name)
            lfp_temp = lfp_orig[sim_name]['trial_avg'].T

            lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)

            lfp_dict['trial_avg'] = lfp_temp.T

            lfp_trials_temp = []
            for itrial in range(lfp_orig[sim_name]['all_trials'].shape[1]):
                lfp_temp = lfp_orig[sim_name]['all_trials'][:,itrial].T

                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)

                lfp_trials_temp.append(lfp_temp.T)
                
            lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
            lfp_out[sim_name] = lfp_dict
            
    return lfp_out

def compute_csd(lfp, method = 'delta', gauss_filter = (1.4,0), coord_electrodes = np.linspace(0,1000E-6,26) * pq.m,\
                diam = 800E-6 * pq.m, sigma = 0.3*pq.S/pq.m, sigma_top = 0.3*pq.S/pq.m, h = 40*1E-6*pq.m, mode = 'sim'):
    
    '''
    This function computes CSD from LFP using the delta iCSD (https://doi.org/10.1016/j.jneumeth.2005.12.005)
    Parameters
    ---------
        lfp : local field potential
        method : method by which CSD is calculated in iCSD
        gauss_filter : smoothing parameter, given in sigma
        coord_electrodes : depth of electrodes on probe
        diam : diameter of laterally constant CSD assumed
        sigma : conductivity in extracellular medium
        sigma_top : conductivity in extracellular medium at top channel
        h : spacing between electrodes
        mode : indicates whether it is calculated for simulation or experimental LFP
    Returns
    ---------
        csd : current source density
    '''

    # simulation LFP is given in mV, while experimental LFP is given in V
    if mode == 'sim':
        lfp = lfp*1E-3*pq.V
    elif mode == 'exp':
        lfp = lfp*pq.V
    else:
        lfp = lfp*1E-3*pq.V
    
    delta_input = {
    'lfp' : lfp,
    'coord_electrode' : coord_electrodes,
    'diam' : diam,
    'sigma' : sigma,
    'sigma_top' : sigma_top,
    'f_type' : 'gaussian',  # gaussian filter. Not used
    'f_order' : (0, 0),     # 3-point filter
    }
    
    step_input = {
    'lfp' : lfp,
    'coord_electrode' : coord_electrodes,
    'diam' : diam,
    'h' : h,
    'sigma' : sigma,
    'sigma_top' : sigma_top,
    'tol' : 1E-12,          # Tolerance in numerical integration
    'f_type' : 'gaussian',
    'f_order' : (3, 1),
    }
    
    spline_input = {
    'lfp' : lfp,
    'coord_electrode' : coord_electrodes,
    'diam' : diam,
    'sigma' : sigma,
    'sigma_top' : sigma_top,
    'num_steps' : len(coord_electrodes)*4,      # Spatial CSD upsampling to N steps
    'tol' : 1E-12,
    'f_type' : 'gaussian',
    'f_order' : (20, 5),
    }

    if method == 'delta':
        csd_dict = dict(
            delta_icsd = icsd.DeltaiCSD(**delta_input)
        )
    elif method == 'step':
        csd_dict = dict(
            step_icsd = icsd.StepiCSD(**step_input)
        )
    elif method == 'spline':
        csd_dict = dict(
            spline_icsd = icsd.SplineiCSD(**spline_input)
        )
        
    #TODO: Set up the input for the other methods
    '''elif method == 'step':
        step_icsd = icsd.StepiCSD(**step_input),
    elif method == 'spline':
        spline_icsd = icsd.SplineiCSD(**spline_input),
    elif method == 'standard':
        std_csd = icsd.StandardCSD(**std_input),'''
  

    for method_, csd_obj in list(csd_dict.items()):
        csd_raw = csd_obj.get_csd()
        
    # Converting from planar to volume density
    if method == 'delta':
        csd_raw = csd_raw / h
        
    # Apply spatial filtering
    csd = gaussian_filter(csd_raw, sigma = gauss_filter)*csd_raw.units
    
    return csd

def find_nearest(array, value):
    array = np.asarray(array)
    idx = (np.abs(array - value)).argmin()
    return idx, array[idx]

def find_files(path, filename = 'lfp.npy'):
    
    all_paths = glob(path)
    
    result = dict()
    for path in all_paths:
        for root, dirs, files in os.walk(path):
            if len(glob(os.path.join(root, filename)))>0:

                file_path = glob(os.path.join(root,filename))[0]
                sim_name = file_path.split('/')[-2]
                print(sim_name)
                try:
                    file = np.load(file_path, allow_pickle=True)[()]
                except:
                    print('File could not be loaded')
                result[sim_name] = file
    return result
                
def find_all_fir_rates_files_sim(path,filename):
    
    result = dict()
    for root, dirs, files in os.walk(path):
        if len(glob(os.path.join(root,filename)))>0:
            
            fir_rate_file_path = glob(os.path.join(root,filename))[0]
            sim_name = fir_rate_file_path.split('/')[-2]
            fir_rate_file = np.load(fir_rate_file_path, allow_pickle=True)[()]
            result[sim_name] = fir_rate_file
    return result


def compute_dist_matrix_exp(file_list_exp, file_list_sim = None, mode = 'pairwise_exp', return_G0 = False,time_distance_scale_factor=1):
    """
    This function takes in lists of either sinks or sources in the CSD from different animals, and computes the 
    Wasserstein between them
    
    Args:
        file_list (list): list of sinks of sources extracted from the CSD in different animals. Each element 
        in the list contains all sinks or all sources from a single animal

    Returns:
        distance_matrix (arr): An array containing the pairwise wasserstein distances between the sinks/sources 
                               of different animals
        M (arr): The cost matrix
        G0_all (arr): Matrix containing the movement of all sink/source elements
        x_s (arr): vector used for constructing the cost matrix
        x_t (arr): vector used for constructing the cost matrix
    """
    
    if mode == 'pairwise_exp':
        n_files = len(file_list_exp)
        distance_matrix = np.zeros((n_files, n_files))
        if return_G0:
            G0_all = np.zeros((n_files, n_files, np.shape(file_list_exp)[1]*np.shape(file_list_exp)[2], \
                              np.shape(file_list_exp)[1]*np.shape(file_list_exp)[2]))
        #G0_all = list()
        for i in tqdm(range(n_files)):
            #G0_temp = list()
            for j in range(i, n_files):
                animal_a = file_list_exp[i]
                animal_b = file_list_exp[j]
                animal_a_reshape = np.reshape(animal_a, (-1))
                animal_b_reshape = np.reshape(animal_b, (-1))

                x = np.linspace(0, 1, animal_a.shape[0], endpoint=True)
                y = np.linspace(0, 1, animal_b.shape[1], endpoint=True)
                xx, yy = np.meshgrid(x,y)

                x_s = np.array([np.reshape(xx, (-1)), np.reshape(yy, (-1))]).T
                x_t = x_s

                # Constructing a cost matrix. It denotes how much it should cost to move in space vs in time
                M = ot.dist(x_s, x_t)
                M /= M.max()

                # Calculating how much you have to move the sinks/sources of animal a to match the sinks/sources
                # of animal b
                G0 = ot.emd(animal_a_reshape, animal_b_reshape, M)

                distance_matrix[i,j] = (M * G0).sum()
                distance_matrix[j,i] = (M * G0).sum()
                
                if return_G0:
                    G0_all[i,j] = G0
                    G0_all[j,i] = G0
                
                #G0_temp.append(G0)
            #G0_all.append(G0_temp)

        if return_G0:
            return distance_matrix, M, G0_all, x_s, x_t
        else:
            return distance_matrix
    elif mode == 'pairwise_exp_trials':
        n_files = len(file_list_exp)
        distance_matrix = np.zeros((n_files, n_files))
        #G0_all = np.zeros((n_files, n_files, np.shape(file_list_exp)[1]*np.shape(file_list_exp)[2], \
        #                  np.shape(file_list_exp)[1]*np.shape(file_list_exp)[2]))
        for i in tqdm(range(n_files)):
            #G0_temp = list()
            for j in range(i, n_files):
                animal_a = file_list_exp[i]
                animal_b = file_list_exp[j]
                animal_a_reshape = np.reshape(animal_a, (-1))
                animal_b_reshape = np.reshape(animal_b, (-1))

                x = np.linspace(0, 1, animal_a.shape[0], endpoint=True)
                y = np.linspace(0, 1, animal_b.shape[1], endpoint=True)
                xx, yy = np.meshgrid(x,y)

                x_s = np.array([np.reshape(xx, (-1)), np.reshape(yy, (-1))]).T
                x_t = x_s

                # Constructing a cost matrix. It denotes how much it should cost to move in space vs in time
                M = ot.dist(x_s, x_t)
                M /= M.max()

                # Calculating how much you have to move the sinks/sources of animal a to match the sinks/sources
                # of animal b
                G0 = ot.emd(animal_a_reshape, animal_b_reshape, M)

                distance_matrix[i,j] = (M * G0).sum()
                distance_matrix[j,i] = (M * G0).sum()

                #G0_all[i,j] = G0
                #G0_all[j,i] = G0
                
                #G0_temp.append(G0)
            #G0_all.append(G0_temp)

        return distance_matrix#, M, G0_all, x_s, x_t
    elif mode == 'exp_to_reco':
        n_files_exp = len(file_list_exp)
        n_files_sim = len(file_list_sim)
        distance_matrix = np.zeros((n_files_exp, n_files_sim))
        G0_all = list()
        for i in tqdm(range(n_files_exp)):
            G0_temp = list()
            for j in range(n_files_sim):
                animal = file_list_exp[i]
                sim = file_list_sim[j]
                animal_reshape = np.reshape(animal, (-1))
                sim_reshape = np.reshape(sim, (-1))

                x = np.linspace(0, 1, animal.shape[0], endpoint=True)
                y = np.linspace(0, time_distance_scale_factor, animal.shape[1], endpoint=True)
                xx, yy = np.meshgrid(x,y)

                x_s = np.array([np.reshape(xx, (-1)), np.reshape(yy, (-1))]).T
                x_t = x_s

                # Constructing a cost matrix. It denotes how much it should cost to move in space vs in time
                M = ot.dist(x_s, x_t)
                M /= M.max()

                # Calculating how much you have to move the sinks/sources of animal a to match the sinks/sources
                # of animal b
                G0 = ot.emd(animal_reshape, sim_reshape, M)

                distance_matrix[i,j] = (M * G0).sum()
                #distance_matrix[j,i] = (M * G0).sum()

                G0_temp.append(G0)
            G0_all.append(G0_temp)
        
        return distance_matrix, M, G0_all, x_s, x_t


def plot_wasserstein_result(a, b, G, xs, xt, name_a = 'Animal A', name_b = 'Animal B', mode = 'source', M = 0, thr=1.e-3, dist = -1):
    
    mx = G.max()
    if mode=='sink':
        c1 = 'Blues'
        c2 = 'Greens'
    if mode=='source':
        c1 = 'Reds'
        c2 = 'Purples'
    #dist = np.round((M*G).sum(), decimals=4)
    
    fig, ax = plt.subplots(figsize=(10, 4), ncols=3, nrows=1, sharey=True, sharex=True)
    
    colormap_range = np.max(np.abs(a))
    # Note: origin = 'lowerleft here'
    im = ax[0].imshow(a, extent=(0,1,0,1), cmap=c1, vmin=0, vmax=colormap_range)
    ax[0].grid()
    #ax[0].set_xlabel('Time from flash onset (ms)')
    ax[0].set_ylabel('Depth ($\mu$m)')
    ax[0].set_title(mode.capitalize()+'s animal '+name_a)
    #ax[0].set_xticks(np.arange(0, a.shape[1], 20))
    #ax[0].set_xticklabels(np.arange(0, a.shape[1]+20, 20))
    #ax[0].set_yticks(np.arange(0, a.shape[0], 6))
    #ax[0].set_yticklabels(-np.arange(0,1000,200))
    
    ax[0].set_xticks(np.arange(0, 1+0.2, 0.2))
    ax[0].set_xticklabels(np.arange(0, a.shape[1]+60, 20))
    ax[0].set_yticks(np.arange(0, 1+0.25, 0.25))
    ax[0].set_yticklabels(np.array([-800, -600, -400, -200, 0]))
    
    colormap_range = np.max(np.abs(b))
    # Note: origin = 'lowerleft here'
    im = ax[1].imshow(b, extent=(0,1,0,1), cmap=c2, vmin=0, vmax=colormap_range)
    ax[1].grid()
    ax[1].set_xlabel('Time from flash onset (ms)')
    ax[1].set_title(mode.capitalize()+'s animal '+name_b)

    colormap_range = np.max(np.abs(a))
    # Note: origin = 'lowerleft here'
    im = ax[2].imshow(a, extent=(0,1,0,1), cmap=c1, vmin=0, vmax=colormap_range, alpha=0.7)
    colormap_range = np.max(np.abs(b))
    im = ax[2].imshow(b, extent=(0,1,0,1), cmap=c2, vmin=0, vmax=colormap_range, alpha=0.5)

    toggle = True
    for i in tqdm(range(xs.shape[0])):
        for j in range(xt.shape[0]):
            if G[i, j] / mx > thr:
                #if toggle:
                #    label = 'Movement of \n'+mode
                #    toggle = False
                #else:
                #    label = None
                ax[2].plot([xs[i, 0], xt[j, 0]], [xs[i, 1], xt[j, 1]], alpha=G[i, j]*0.1 / mx, color='black')#, label = label)

    ax[2].grid()
    #ax[2].set_xlabel('Time from flash onset (ms)')
    ax[2].set_title('Norm. WD between \n'+mode+'s = '+str(np.round(dist, 3)))

    return fig

def subtract_lfp_baseline_all_sims(lfp_orig, tstim_onset = 1000, contributions = False, contributions_summed = False):
        
    #TODO: Fix the stupid data organization that forces all these if-statements
    
    lfp_out = dict()

    if contributions:
        for sim_name in lfp_orig.keys():
            print(sim_name)
            lfp_dict_pops = dict()
            for pop_name in lfp_orig[sim_name].keys():
                lfp_dict = dict()
                
                lfp_temp = lfp_orig[sim_name][pop_name]['trial_avg'].T
                
                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
            
                lfp_dict['trial_avg'] = lfp_temp.T
                
                lfp_trials_temp = []
                for itrial in range(lfp_orig[sim_name][pop_name]['all_trials'].shape[1]):
                    
                    lfp_temp = lfp_orig[sim_name][pop_name]['all_trials'][:,itrial].T
                    
                    lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
                    
                    lfp_trials_temp.append(lfp_temp.T)
                
                lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
                lfp_dict_pops[pop_name] = lfp_dict
                                    
            lfp_out[sim_name] = lfp_dict_pops
            
    elif contributions_summed:
        for sim_name in lfp_orig.keys():
            print(sim_name)
            lfp_dict_pops = dict()
            for pop_name in lfp_orig[sim_name]['all_trials'].keys():
                lfp_dict = dict()
                
                lfp_temp = lfp_orig[sim_name]['trial_avg'][pop_name].T
                
                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
            
                lfp_dict['trial_avg'] = lfp_temp.T
                
                lfp_trials_temp = []
                for itrial in range(lfp_orig[sim_name]['all_trials'][pop_name].shape[1]):
                    
                    lfp_temp = lfp_orig[sim_name]['all_trials'][pop_name][:,itrial].T
                    
                    lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)
                    
                    lfp_trials_temp.append(lfp_temp.T)
                
                lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
                lfp_dict_pops[pop_name] = lfp_dict
                                    
            lfp_out[sim_name] = lfp_dict_pops
    else:
        for sim_name in lfp_orig.keys():
            lfp_dict = dict()
            print(sim_name)
            lfp_temp = lfp_orig[sim_name]['trial_avg'].T

            lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)

            lfp_dict['trial_avg'] = lfp_temp.T

            lfp_trials_temp = []
            for itrial in range(lfp_orig[sim_name]['all_trials'].shape[1]):
                lfp_temp = lfp_orig[sim_name]['all_trials'][:,itrial].T

                lfp_temp -= np.mean(lfp_temp[:tstim_onset], axis = 0)

                lfp_trials_temp.append(lfp_temp.T)
                
            lfp_dict['all_trials'] = np.array(lfp_trials_temp)
                
            lfp_out[sim_name] = lfp_dict
            
    return lfp_out
