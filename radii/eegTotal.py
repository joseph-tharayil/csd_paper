import numpy as np
import numpy as np
import pandas as pd
import os
from scipy.stats import linregress

from sklearn.decomposition import PCA

from scipy import signal

sos = signal.butter(3, [1,500], 'bp', fs=10000, output='sos')

EEGs = {}

for i in range(10):

    for file in os.listdir(str(i)+'/pkls'):
        t = file.split('.')[0].split('g')[1]
        if i == 0:
            EEGs[t] = pd.read_pickle(str(i)+'/pkls/'+file)
        else:
            
            try:
                EEGs[t] += pd.read_pickle(str(i)+'/pkls/'+file)
            except:
                pass

total = 0
for j, item in enumerate(EEGs.keys()):

    total += EEGs[item]
    
    
np.save('total.npy',total)
np.save('EEGs.npy',EEGs)

