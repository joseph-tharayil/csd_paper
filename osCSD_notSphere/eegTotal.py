import numpy as np
import numpy as np
import pandas as pd
import os
from scipy.stats import linregress

from sklearn.decomposition import PCA

from scipy import signal

sos = signal.butter(3, [1,500], 'bp', fs=10000, output='sos')


for i in range(10):

    total = 0

    for file in os.listdir(str(i)+'/pkls'):
        t = file.split('.')[0].split('g')[1]
        if i == 0:
            total = pd.read_pickle(str(i)+'/pkls/'+file)
        else:
            
            try:
                total += pd.read_pickle(str(i)+'/pkls/'+file)
            except:
                pass
    
    
    np.save(str(i)+'/total.npy',total)
