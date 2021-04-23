import os 
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import numpy as np
import pandas as pd

# Read in the data
clean_test = pd.read_csv("test.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)
noisy_test = pd.read_csv("noisytest.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)

# Convert the data to numpy arrays (realy matrices, but whatever)
clean_test = np.array(clean_test)
noisy_test = np.array(noisy_test)

#print(clean_test)
#print(noisy_test)

#print(noisy_test - clean_test)
#print(np.square(noisy_test - clean_test))

mse = (np.square(noisy_test - clean_test)).mean()

print(mse)
