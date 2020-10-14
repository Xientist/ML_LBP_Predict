
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

struct park {

	int correct;
	int lbp[256];
};

double SAD(int* hist1, int* hist2) {

	double dist = 0;

	for (int index = 0; index < 256; index++) {

		dist += abs(hist1[index] - hist2[index]);
	}

	return dist;
}

double SSD(int* hist1, int* hist2) {

	double dist = 0;

	for (int index = 0; index < 256; index++) {

		dist += pow((hist1[index] - hist2[index]),2);
	}

	return dist;
}

double Correlation(int* hist1, int* hist2) {

	double dist = 0;
	double meanH1 = 0, meanH2 = 0;

	for (int index = 0; index < 256; index++) {

		meanH1 += hist1[index];
		meanH2 += hist2[index];
	}

	meanH1 = 1.0 * meanH1 / 256.0;
	meanH2 = 1.0 * meanH2 / 256.0;

	double sum1 = 0, sum2 = 0, sum3 = 0;

	for (int index = 0; index < 256; index++) {

		sum1 += (hist1[index] - meanH1) * (hist2[index] - meanH2);
		sum2 += (hist1[index] - meanH1) * (hist1[index] - meanH1);
		sum3 += (hist2[index] - meanH2) * (hist2[index] - meanH2);
	}

	dist = sum1 / (sqrt(sum2*sum3));

	return dist;
}

double ChiSquared(int* hist1, int* hist2) {

	double dist = 0;

	for (int index = 0; index < 256; index++) {

		dist += pow(hist1[index] - hist2[index], 2) / hist1[index];
	}

	return dist;
}

double Intersection(int* hist1, int* hist2) {

	double dist = 0;

	for (int index = 0; index < 256; index++) {

		dist += (hist1[index] < hist2[index])? hist1[index]: hist2[index];
	}

	return dist;
}

double Bhattacharyya(int* hist1, int* hist2) {

	double dist = 0;
	double meanH1 = 0, meanH2 = 0, sum = 0;

	for (int index = 0; index < 256; index++) {

		meanH1 += hist1[index];
		meanH2 += hist2[index];
		sum += sqrt(hist1[index] * hist2[index]);
	}

	meanH1 = 1.0 * meanH1 / 256.0;
	meanH2 = 1.0 * meanH2 / 256.0;

	dist = sqrt(1.0 - (1.0 / sqrt(meanH1*meanH2*pow(256, 2)) * sum));

	return dist;
}

int main()
{
	std::ifstream training("C:/Users/Mad Scientifique/Documents/m2_numerisation/results/training.txt");
	std::ifstream test("C:/Users/Mad Scientifique/Documents/m2_numerisation/results/test.txt");
	
	const int nbImages = 1000;

	std::vector<park> trainParks;
	std::vector<park> testParks;

	for (int line = 0; line < nbImages; line++) {

		park trainPark, testPark;

		for (int index = 0; index < 256; index++) {

			training >> trainPark.lbp[index];
			test >> testPark.lbp[index];
		}

		training >> trainPark.correct;
		test >> testPark.correct;

		trainParks.push_back(trainPark);
		testParks.push_back(testPark);
	}

	training.close();
	test.close();

	int nbTests = 1000;
	int nbCorrect = 0;
	double predict = 0;

	for (int te = 0; te < nbTests; te++) {

		std::cout << "prédiction pour l'image " << te << std::endl;

		double distMin = INT_MAX;
		int minIndex = 0;

		for (int tr = 0; tr < nbImages; tr++) {

			double dist = Correlation(testParks[te].lbp, trainParks[tr].lbp);

			if (dist < distMin) {
				distMin = dist;
				minIndex = tr;
			}
		}

		if (testParks[te].correct == trainParks[minIndex].correct) { nbCorrect++; }

		predict = (nbCorrect * 1.0) / ((te+1)*1.0);

		std::cout << "les prédictions ont été correctes à " << predict * 100 << "% pour l'instant!" << std::endl;
	}

	predict = (nbCorrect * 1.0) / nbTests;

	std::cout << "les prédictions ont été correctes à " << predict * 100 << "%!" << std::endl;

}

