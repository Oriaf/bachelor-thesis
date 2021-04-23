import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
from tensorflow.keras import layers
import numpy as np
import pandas as pd
import math
import scipy.optimize as opt
from tensorflow.keras import backend as K
from tensorflow import keras

# Some basic formating configurations for printing
np.set_printoptions(precision=3, suppress=True)

# Read in the data
clean_train = pd.read_csv("clean.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)
noisy_train = pd.read_csv("noisy.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)
clean_test = pd.read_csv("test.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)
noisy_test = pd.read_csv("noisytest.dat",
		names=["b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9"]
)
#print(clean_train.head())
#print(noisy_train.head())
#print(clean_test.head())
#print(noisy_test.head())

# Convert the data to numpy arrays (realy matrices, but whatever)
clean_train = np.array(clean_train)
noisy_train = np.array(noisy_train)
clean_test = np.array(clean_test)
noisy_test = np.array(noisy_test)
#print(clean_train)
#print(noisy_train)
#print(clean_test)
#print(noisy_test)

# Constants
n_epochs = 8
learning_rate = 0.001

# Science!
middle_layer_size = 5
while(middle_layer_size < 200):
	num = 0
	losses = []

	print("{} # of Nodes".format(middle_layer_size))

	# Do the 10 repeats
	while(num<10):
		num = num + 1
		MODEL_NAME = 'newData-DAE-{}-epoch-{}LR-{}{}{}{}.model'.format(n_epochs,learning_rate,'10-',middle_layer_size,'-10-layer-test-',num)
		
		# Create our model given our middle layer size
		#print(num)
		model = tf.keras.Sequential([layers.InputLayer(input_shape=(10, )),
					     layers.Dense(10, activation='relu'),
					     layers.Dense(middle_layer_size, activation='relu'),
					     layers.Dense(10)
					    ])
		model.compile(optimizer='adam', loss='mean_squared_error')
		K.set_value(model.optimizer.learning_rate, learning_rate)
	
		# Train the model
		model.fit(noisy_train, clean_train, epochs=n_epochs, batch_size=10,shuffle=True, verbose=0)
	
		# Evaluate how good the network did
		loss = model.evaluate(noisy_test, clean_test, verbose=0)
		losses.append(loss)
		
	
	print('Median: {}'.format(np.median(losses)))
	print('Min: {}'.format(np.amin(losses)))
	print('Max: {}'.format(np.amax(losses)))
	print('Avg: {}'.format(np.mean(losses)))
	print()

	middle_layer_size = middle_layer_size + 5
