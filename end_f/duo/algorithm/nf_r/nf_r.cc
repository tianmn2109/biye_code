#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int nf(vector<int> v, int capacity, vector<vector<int> > &bins) {
	int num = 0;
	vector<int> c;
	for (int i = 0; i < v.size(); i ++) {
		if (c.size() == 0 || c[c.size() - 1] < v[i]) {
			c.push_back(capacity - v[i]);
			bins.push_back(vector<int>());
			bins[bins.size() - 1].push_back(v[i]);
		}
		else {
			c[c.size() - 1] -= v[i];
			bins[bins.size() - 1].push_back(v[i]);
		}
	}
	return bins.size();
}

double fitness(vector<vector<int> > &bins, int capacity) {
        double fit = 0.0;
        for (int i = 0; i < bins.size(); i ++) {
                double tempCap = 0;
                for (int j = 0; j < bins[i].size(); j ++) 
                        tempCap += bins[i][j];
                fit += tempCap / capacity * tempCap / capacity;
        }
        return fit / bins.size();
}

void shuffle(vector<int> &v) {
	for (int i = v.size() - 1; i > 0; i --) {
		int index = rand() % (i + 1);
		swap(v[i], v[index]);
	}
}

int main(int argc, char **argv)
{
	int num;
	int capacity;
	ifstream f(argv[1]);	
	f >> num;
	f >> capacity;
	vector<int> v(num);
	for (int i = 0; i < v.size(); i ++)
		f >> v[i];
	f.close();
	vector<vector<int> > bins;
	clock_t start = clock();
	int minBins = nf(v, capacity, bins);
	double maxFit = fitness(bins, capacity);
	for (int i = 0; i < 1000; i ++) {
		bins.resize(0);
		shuffle(v);
		int tempBins = nf(v, capacity, bins);
		if (tempBins <= minBins) {
			minBins = tempBins;
			double tempFit = fitness(bins, capacity);
			maxFit = tempFit;
		}
		
	}
	cout << "bins num = " << minBins;
	cout << " fitness = " << maxFit;
/*	for (int i = 0 ; i < bins.size(); i ++) {
		cout << "bin number: " << i + 1 << ": ";
		for (int j = 0; j < bins[i].size(); j ++)
			cout << bins[i][j] << " ";
		cout << endl;
	}
*/	clock_t finish = clock();
	cout << " time = " << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	return 0;

}
