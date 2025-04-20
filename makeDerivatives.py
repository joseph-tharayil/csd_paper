import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import h5py

fname = 'electrodes/coeffs_highRes.h5'
f = h5py.File(fname)
offsets = f['S1nonbarrel_neurons']['offsets'][:]
derivatives = np.diff(np.diff(f['electrodes']['S1nonbarrel_neurons']['scaling_factors'][:,:101],axis=1),axis=1)
derivatives = -derivatives
f_dest = h5py.File('electrodes/csdCoeffs.h5','w')
f.copy(f['S1nonbarrel_neurons'],f_dest,'S1nonbarrel_neurons')
f_dest.create_dataset('electrodes/S1nonbarrel_neurons/scaling_factors',data=derivatives)
f.close()
f_dest.close()