#include<stdio.h>
#include<math.h>
#include<random>
#include<time.h>
#include<chrono>

/*
	Generates training and evaluation data for the DAE consisting of two pairs
	of two files each. Each file containing N distance vectors, and each of the
	two pairs (the training data pair, and the evaluation data pair) consisted
	of the following two files:
		* Clean Data File - Each element in the distance vector coresponds to
		a true distance between the UE and one of the base stations
		* Noisy Data File - The distance vector on line i is derived from the
		distance vector on line i in the clean training data, with each
		element having stochastic noise added to it

	Usage: datagen [-flag | --flag, value]
	Flags:
		-N, --numdata		the number of vector pairs in each set
		-b, --numbasestations	the number of basestations
		-w, --width		the width of the area
		-h, --height		the height of the area
		-c, --clean		name of the file for the clean training data
		-n, --noisy		name of the file for the noisy training data
		-vc, --validate_clean	name of the file for the clean validation data
		-vn, --validate_noisy	name of the file for the noisy validation data
*/

struct point{
	int x;
	int y;
};

// Default data settings
#define DEFAULT_DATASET_SIZE 100
#define DEFAULT_NUM_STATIONS 10
#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 100
// Default noise parameters
#define DEFAULT_EST_RANGE 0
#define DEFAULT_STDDEV_RANGE 10 //5, 10, 20
#define DEFAULT_LOW_TIME 0
#define DEFAULT_HIGH_TIME 20 //10, 20, 40
#define DEFAULT_PROB_NLOS 0.2 //0.1, 0.2, 0.4
#define DEFAULT_DIST_BIAS_NLOS 50
// Default file names
#define DEFAULT_CLEAN_FILE_NAME "clean.dat"
#define DEFAULT_NOISY_FILE_NAME "noisy.dat"
#define DEFAULT_VCLEAN_FILE_NAME "test.dat"
#define DEFAULT_VNOISY_FILE_NAME "noisytest.dat"
#define DEFAULT_UE_POS_FILE_NAME "pos.dat"
#define DEFAULT_NOISY_UE_POS_FILE_NAME "noisy_pos.dat"
#define DEFAULT_BASESTATIONS_FILE_NAME "basestations.dat"

using namespace std;

int N = DEFAULT_DATASET_SIZE;
int n = DEFAULT_NUM_STATIONS;
int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
string cleanName = DEFAULT_CLEAN_FILE_NAME;
string noisyName = DEFAULT_NOISY_FILE_NAME;	
string vCleanName = DEFAULT_VCLEAN_FILE_NAME;
string vNoisyName = DEFAULT_VNOISY_FILE_NAME;	
string posName = DEFAULT_UE_POS_FILE_NAME;
string nPosName = DEFAULT_NOISY_UE_POS_FILE_NAME;
string baseName = DEFAULT_BASESTATIONS_FILE_NAME;

// Helper function that parses command line arguments
void parseArgs(int argc, char **argv){
	for(int i = 1; i < argc; i++){
		string argument(argv[i]);

		if((argument == "--numdata" || argument == "-N") && i+1 < argc){
			N = atoi(argv[++i]);
		}
		else if((argument == "--numbasestations" || argument == "-b")
		  && i+1 < argc){
			n = atoi(argv[++i]);
		}
		else if((argument == "--width" || argument == "-w") && i+1 < argc){
			width = atoi(argv[++i]);
		}
		else if((argument == "--height" || argument == "-h") && i+1 < argc){
			height = atoi(argv[++i]);
		}
		else if((argument == "--clean" || argument == "-c") && i+1 < argc){
			cleanName = argv[++i];
		}
		else if((argument == "--noisy" || argument == "-n") && i+1 < argc){
			noisyName = argv[++i];
		}
		else if((argument == "--validate_clean" || argument == "-vc") && i+1 < argc){
			vCleanName = argv[++i];
		}
		else if((argument == "--validate_noisy" || argument == "-vn") && i+1 < argc){
			vNoisyName = argv[++i];
		}
		else {
			printf(""
			 "Usage: datagen [-flag | --flag, value]\n"
			 "Flags:\n"
			 "-N, --numdata		the number of vector pairs in each set\n"
			 "-b, --numbasestations	the number of basestations\n"
			 "-w, --width		the width of the area\n"
			 "-h, --height		the height of the area\n"
			 "-c, --clean		name of the file for the clean data\n"
			 "-n, --noisy		name of the file for the noisy data\n"
			 "-vc, --validate_clean	name of the file for the clean validation data\n"
			 "-vn, --validate_noisy	name of the file for the noisy validation data\n"
			);
			
			exit(EXIT_SUCCESS);
		}
	}
}

int main(int argc, char **argv){
	unsigned seed[2];
	parseArgs(argc, argv);

	//printf("%d\n%d\n", N, n);

	srand(time(NULL));

	point basestation[n]; //We will let the base stations be fixed points for all data

	// Generate data
	FILE *cleanFile[2];
	FILE *noisyFile[2];
	cleanFile[0] = fopen(cleanName.c_str(), "w");
	noisyFile[0] = fopen(noisyName.c_str(), "w");
	cleanFile[1] = fopen(vCleanName.c_str(), "w");
	noisyFile[1] = fopen(vNoisyName.c_str(), "w");
	for(int k = 0; k < 2; k++){
		//Initialize the random for this generation
		seed[k] = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine randomEngine(seed[k]);
		std::normal_distribution<double> normal(DEFAULT_EST_RANGE,
							DEFAULT_STDDEV_RANGE
						       );
		std::uniform_real_distribution<double> uniform(DEFAULT_LOW_TIME,
								DEFAULT_HIGH_TIME
							      );
		std::bernoulli_distribution bernoulli(DEFAULT_PROB_NLOS);

		for(int i = 0; i < N; i++){
			// Clean
			for(int i = 0; i < n; i++){
				basestation[i].x = rand() % width;
				basestation[i].y = rand() % height;

			}
	
			int targetX = rand() % width;
			int targetY = rand() % height;

			double distance[n];
	
			string distanceRow = ""; 
			for(int j = 0; j < n; j++){
				int x = targetX - basestation[j].x;
				int y = targetY - basestation[j].y;
	
				distance[j] = sqrt(x*x + y*y);
				//printf("{%f, %d, %d}", distance[j], basestation[j].x, basestation[j].y);
				if(j != 0) distanceRow += ", ";
				distanceRow += to_string(distance[j]);
	
				//distanceRow = to_string(distance[j]) + "\n";
	
				//fputs(distanceRow.c_str(), cleanFile);
			}
			distanceRow += "\n";
			fputs(distanceRow.c_str(), cleanFile[k]);
	
			// Noisy
			//Add some noise
			distanceRow = ""; 
			for(int j = 0; j < n; j++){
				double rangeNoise = normal(randomEngine);
				double clockNoise = uniform(randomEngine);
				double nlosNoise = bernoulli(randomEngine);
	
				double noise = rangeNoise + clockNoise +
						nlosNoise*DEFAULT_DIST_BIAS_NLOS;
				distance[j] += noise;
				if(j != 0) distanceRow += ", ";
				distanceRow += to_string(distance[j]);
			}
			distanceRow += "\n";
			fputs(distanceRow.c_str(), noisyFile[k]);

		}
		fclose(cleanFile[k]);
		fclose(noisyFile[k]);
	}

	return 0;
}
