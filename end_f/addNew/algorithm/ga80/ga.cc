#include <algorithm>
#include<iomanip>
#include <fstream>
#include <cfloat>
#include <climits>
#include <cstdio>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "bin.h"
using namespace std;

#define POPULATION_SIZE 100
int testCount = 0;
int validPopulationSize = POPULATION_SIZE;
int gcd(int m, int n) {
        if (m < n)
                swap(m, n);
        while (n > 0) {
                int r = m % n;
                m = n;
                n = r;
        }
        return m;
}

int MCM(int m, int n) {
        int r = gcd(m, n);
        long long mm = (long long)m;
        long long nn = (long long)n;
        long long rr = (long long)r;
        long long ret = (mm * nn) / rr;
        return (int) ret;
}

void shuffle(vector<TsHandled> &v) {
	for (int i = v.size() - 1; i > 0; i --) {
		int index = rand() % (i + 1);
		swap(v[i], v[index]);
	}
}
vector<Bin> population(validPopulationSize);
vector<double> e(validPopulationSize, DBL_MAX);
vector<vector<OutTs> > o(validPopulationSize, vector<OutTs>());
bool hasSolution = false;
void changeOts(vector<OutTs> &o, Bin &bin) {
	for (int i = 0; i < o.size(); i ++) {
		for (int j = 0; j < bin.table.size(); j ++)
			for (int k = 0; k < bin.table[j].size(); k ++)
				if (bin.table[j][k] == o[i].id) {
					o[i].slot = k;
					o[i].offset = j;
					goto NE;
				}
		NE:
			;
	}
}
bool initPopulation(vector<TsHandled> &v, Bin b)
{
	int repeat = 0;
	int count = 0;
	int k = 0;
	count = 0;
	AGAIN:
	int selectInit = 0;
	for (int i = 0; i < o.size(); i ++)
		o[i].resize(v.size());
	for (int i = 0; i < validPopulationSize; i ++) {
		if (k == validPopulationSize) {
//			cout << "k = " << k << "validPopula = " << validPopulationSize << endl;
			break;
		}
		//shuffle(v);
		next_permutation(v.begin(), v.end());	
		b.clear();
		if (b.findSolution(v, 0, o[k])) {
			if (selectInit == 1) {
				b.adjuest();
				changeOts(o[k], b);
				selectInit = 0;
			}
			else {
				selectInit = 1;
			}
			hasSolution = true;
			population[k] = b;
			e[k] = b.evaluate();
			k ++;
			//cout << "select = " << !selectInit << " e[" << k - 1 << "]" << " = " << e[k - 1] << endl;
		}
		else {
			count ++;
		}
	}
	int validNum = validPopulationSize - count;
//	cout << "count = " << count << endl;
//	cout << "validPopulationSize = " << validPopulationSize << endl;
	if (k != validPopulationSize)  {
		repeat ++;
		//cout << "repeat = " << repeat << endl;
		if (repeat < 50) {
			;
			goto AGAIN;
		}
		if (k != 0)
			goto BEGIN;
		return false;
	}
BEGIN:
	validNum = k;
	for (int i = validNum; i < validPopulationSize; i ++) {
		population[i] = population[validNum - 1];
		e[i] = e[validNum - 1];
		o[i] = o[validNum - 1];
	}
	population.resize(validPopulationSize);
	o.resize(validPopulationSize);
	e.resize(validPopulationSize);
	for (int i = 1; i < e.size(); i ++) {
	Bin tp = population[i];
		double te = e[i];
		vector<OutTs> to = o[i];
		int j = i - 1;
		for (j = i - 1; j >= 0; j --) {
			if (te < e[j]) {
				e[j + 1] = e[j];
				o[j + 1] = o[j];
				population[j + 1] = population[j];
			}
			else 
				break;
		}
		e[j + 1] = te;
		o[j + 1] = to;
		population[j + 1] = tp;
	}
//	for (int i = 0; i < e.size(); i ++) 
//		cout << "i = " << i << "    " << e[i] << endl;
//	cout << "-----------------------------------------" << endl;
		return true;
//		for (int i = 0; i < e.size(); i ++) {
//			cout << e[i] << " ";
//		}
//		cout << endl;
}

bool addNewPopulation(vector<TsHandled> &v, Bin b, vector<Bin> &p, vector<double> &ee, vector<vector<OutTs> > &oo, int validPopulationSize) {

	int repeat = 0;
	int count = 0;
	int k = 0;
	count = 0;
	AGAIN1:
	int selectInit = 0;
	for (int i = 0; i < oo.size(); i ++)
		oo[i].resize(v.size());
	for (int i = 0; i < validPopulationSize; i ++) {
		if (k == validPopulationSize) {
			//cout << "k = " << k << "validPopula = " << validPopulationSize << endl;
			break;
		}
		//shuffle(v);
		next_permutation(v.begin(), v.end());
		b.clear();
		if (b.findSolution(v, 0, oo[k])) {
			if (selectInit == 1) {
				b.adjuest();
				changeOts(oo[k], b);
				selectInit = 0;
			}
			else {
				selectInit = 1;
			}
			hasSolution = true;
			p[k] = b;
			ee[k] = b.evaluate();
			k ++;
			//cout << "select = " << !selectInit << " ee[" << k - 1 << "]" << " = " << ee[k - 1] << endl;
		}
		else {
			count ++;
		}
	}
	int validNum = validPopulationSize - count;
//	cout << "count = " << count << endl;
//	cout << "validPopulationSize = " << validPopulationSize << endl;
	if (k != validPopulationSize)  {
		repeat ++;
		if (repeat < 50) {
			;
			goto AGAIN1;
		}
		if (validNum != 0)
			goto BEGIN1;
		return false;
	}
BEGIN1:
	validNum = k;
	for (int i = validNum; i < validPopulationSize; i ++) {
		population[i] = population[validNum - 1];
		e[i] = e[validNum - 1];
		o[i] = o[validNum - 1];
	}
}

Bin cross(Bin b1, vector<OutTs> ots1, Bin b2, vector<OutTs> ots2, vector<OutTs> &ots3) {
	Bin b(b1.table.size(), b1.table[0].size());
	int k = 0;
	for (int i = 0; i < ots1.size(); rand() % 2 == 0 ? i += 1 : i += 2) {
		ots3[k++] = ots1[i];
		TsHandled ts;
		ts.id = ots1[i].id;
		ts.epochPeroid = ots1[i].epochPeroid;
		ts.occupy = ots1[i].occupy;
		b.place(ts, ots1[i].offset, ots1[i].slot);
		vector<OutTs>::iterator it;
		for (it = ots2.begin(); it < ots2.end(); it ++) {
			if (it->id == ots1[i].id) {
				ots2.erase(it);
				break;
			}
		}
	}
	vector<TsHandled> v;
	for (int i = 0; i < ots2.size(); i ++) {   // 简单 有待改进
		TsHandled ts;
		ts.id = ots2[i].id;
		ts.epochPeroid = ots2[i].epochPeroid;
		ts.occupy = ots2[i].occupy;
		v.push_back(ts);
	}
	vector<OutTs> ot(v.size());
	if (false == b.findSolution2(v, 0, ot)) {
		ots3 = ots1;
		return b1;
		ots3 = o[validPopulationSize - 1];
		return population[validPopulationSize - 1];
	}
	for (int i = 0; i < ot.size(); i ++)
		ots3[k++] = ot[i];
	for (int i = 0; i < ots3.size(); i ++)
		if (ots3[i].id == 0) {
			ots3 = ots1;
			return b1;
			ots3 = o[validPopulationSize - 1];
			return population[validPopulationSize - 1];
		}
	return b;

// 效果不好
/*
	Bin b(b1.table.size(), b1.table[0].size());
	vector<TsHandled> v;
	int len = ots1.size();
	int flag = 1;
	int k = 0;
	sort(ots1.begin(), ots1.end());
	sort(ots2.begin(), ots2.end());
	for (int i = 0; i < len; i ++) {
		int id = 0;
		int flag = 1;
		if (flag) {
			if (b.posCanPlace(ots1.back().offset, ots1.back().slot, ots1.back().epochPeroid, ots1.back().occupy)) {
				TsHandled ts;
				ts.id = ots1.back().id;
				ts.epochPeroid = ots1.back().epochPeroid;
				ts.occupy = ots1.back().occupy;
				b.place(ts, ots1.back().offset, ots1.back().slot);
				id = ts.id;
				ots3[k ++] = ots1.back();
			}
			else if (b.posCanPlace(ots2.back().offset, ots2.back().slot, ots2.back().epochPeroid, ots2.back().occupy)) {
					TsHandled ts;
					ts.id = ots2.back().id;
					ts.epochPeroid = ots2.back().epochPeroid;
					ts.occupy = ots2.back().occupy;
					b.place(ts, ots2.back().offset, ots2.back().slot);
					id = ts.id;
				ots3[k ++] = ots2.back();
			}
		}
		else {
			if (b.posCanPlace(ots2.back().offset, ots2.back().slot, ots2.back().epochPeroid, ots2.back().occupy)) {
				TsHandled ts;
				ts.id = ots2.back().id;
				ts.epochPeroid = ots2.back().epochPeroid;
				ts.occupy = ots2.back().occupy;
				b.place(ts, ots2.back().offset, ots2.back().slot);
				id = ts.id;
				ots3[k ++] = ots2.back();
			}
			else if (b.posCanPlace(ots1.back().offset, ots1.back().slot, ots1.back().epochPeroid, ots1.back().occupy)) {
					TsHandled ts;
					ts.id = ots1.back().id;
					ts.epochPeroid = ots1.back().epochPeroid;
					ts.occupy = ots1.back().occupy;
					b.place(ts, ots1.back().offset, ots1.back().slot);
					id = ts.id;
					ots3[k ++] = ots1.back();
			}
		
		}
		flag != flag;
		if (id == 0) {
			TsHandled ts;
			ts.id = ots2[i].id;
			ts.epochPeroid = ots2[i].epochPeroid;
			ts.occupy = ots2[i].occupy;
			v.push_back(ts);
		}
		ots1.pop_back();
		ots2.pop_back();	

	}
	vector<OutTs> ot(v.size());
	b.findSolution(v, 0, ot);
	for (int i = 0; i < ot.size(); i ++)
		ots3[k++] = ot[i];
	return b;
*/
}


void mutate(Bin &b, vector<OutTs> &ots) {
        int r = rand() % ots.size();
/*      for (int i = 0; i < ots.size(); i ++) {
                if (r == i)
                        continue;
                for (int k = ots[i].offset; k < b.table.size(); k ++) {
                        for (int s = ots[i].slot; s < ots[i].slot + ots[i].occupy; s ++)
                                b.table[k][s] = ots[i].id;
                }
        }
*/      vector<pair<int, int> > rc;
        TsHandled ts;
        ts.id = ots[r].id;
        ts.occupy = ots[r].occupy;
        ts.epochPeroid = ots[r].epochPeroid;
	
//	b.outPutTable();
        b.disPlace(ts, ots[r].offset, ots[r].slot);
//	cout << " $$ " << ots[r].offset << " $$ " << ots[r].slot <<  " id " << ots[r].id << endl;
//	b.outPutTable();
        if (b.canPlace(ts, rc)) {
		int select = /*rc.size() - 1;*/rand() % (rc.size());
/*		int select = 0;
		int minTemp = b.countRow(rc[0].first);
		for (int i = 1; i < rc.size(); i ++) {
			int temp = b.countRow(rc[i].first);
			if (temp < minTemp) {
				select = i;
				minTemp = temp;
			}
		}
  */              int row = rc[select].first;
                int col = rc[select].second;
                b.place(ts, row, col);
                ots[r].offset = row;
                ots[r].slot = col;
//		cout << "rc.size = " << rc.size() << endl;
        }
//	b.outPutTable();
//      b.outPutTable();
}

	int C = 0;
	int lastC = 1;
Bin produceNewSolution(Bin b, vector<OutTs> &ots) {
        int r = rand() % ots.size();
/*      for (int i = 0; i < ots.size(); i ++) {
                if (r == i)
                        continue;
                for (int k = ots[i].offset; k < b.table.size(); k ++) {
                        for (int s = ots[i].slot; s < ots[i].slot + ots[i].occupy; s ++)
                                b.table[k][s] = ots[i].id;
                }
        }
	
*/      b.outPutTable();
	vector<pair<int, int> > rc;
        TsHandled ts;
        ts.id = ots[r].id;
        ts.occupy = ots[r].occupy;
        ts.epochPeroid = ots[r].epochPeroid;
        b.disPlace(ts, ots[r].offset, ots[r].slot);
	//cout << " $$ " << ots[r].offset << " $$ " << ots[r].slot <<  " id " << ots[r].id << endl;
	b.outPutTable();
        if (b.canPlace(ts, rc)) {
                int row = rc[rc.size() - 1].first;
                int col = rc[rc.size() - 1].second;
                b.place(ts, row, col);
                ots[r].offset = row;
                ots[r].slot = col;
        }
     b.outPutTable();

        return b;
}

	vector<Bin> crp(validPopulationSize/*, population[0]*/);
	vector<double> cre(validPopulationSize);
	vector<vector<OutTs> > cro(validPopulationSize);
int tempNew = 40;
int schedule(vector<TsChannel> &ts, int slotTime, int slotNum) {
        vector<TsHandled> v(ts.size());
        for (int i = 0; i < ts.size(); i ++) {
                v[i].id = ts[i].id;
                v[i].epochPeroid = ts[i].peroid / (slotTime * slotNum);
                v[i].occupy = (int)ceil((double)ts[i].sendTime / (double)slotTime);
        }
        int epochNum = 1;
        for (int i = 0; i < v.size(); i ++) {
                epochNum = MCM(epochNum, v[i].epochPeroid);
        }
	srand((int)time(NULL));
	Bin bin(epochNum, slotNum);
	Bin res(epochNum, slotNum);
	vector<OutTs> ots(ts.size());
        //sort(v.begin(), v.end());
	double minEval = DBL_MAX;
	bool ret = initPopulation(v, bin);
	int cnt = 1000;
	C = cnt;
	int unChanged = 0;
	int addNew = 0;
	int exitTime = 0;
	int mutateTime = 1;
	if (ret == false) {
		; ret = false;
		goto FINISH;
	}
	
	while (cnt --) {
		//cout << "%%%%%%%%%%%%%%%%%/ cnt = " <<  cnt << endl;
	//	for (int i = 0; i < e.size(); i ++)
	//		cout << "e[" << i << "] = " << e[i] << " " ;
	//	cout << endl;
		// 选择交叉 轮盘赌算法
		vector<double> sum(e.size());
		vector<double> tempE(e);
		double maxD = tempE[0];
		for (int i = 0; i < tempE.size(); i ++)
			maxD = max(tempE[i], maxD);
		for (int i = 0; i < tempE.size(); i ++)
			tempE[i] = maxD + 800  - tempE[i];
		
		sum[0] = tempE[0];
		for (int i = 0; i < sum.size() ; i ++)
			if (i > 0)
				sum[i] = sum[i - 1] + tempE[i];
		for (int i = 0; i < sum.size(); i ++)
			sum[i] /= sum[sum.size() - 1];
		// 交叉次数 初始种群大小的一半 每次交叉 差生两个新的染色体
//		cout << testCount ++ << endl;
	//	vector<Bin> crp(validPopulationSize, population[0]);
		int m = 0;  // 交叉产生的染色
		//cout << "validPopulationSize = " << validPopulationSize << endl; 
		for (int i = 0; i < (validPopulationSize) / 2; i ++) {
			double r1 = (double)(rand() % 1000) / 1000.0;
			double r2 = (double)(rand() % 1000) / 1000.0;
			int ri = 0;
			int rj = 0;
			for (int k = 0; k < validPopulationSize; k ++) {
				if (r1 < sum[k]) {
					ri = k;
					break;
				}
			}
			for (int k = 0; k < validPopulationSize; k ++) {
				if (r2 < sum[k]) {
					rj = k;
					break;
				}
			}
			// 选择的两个染色体相同
		//	ri = validPopulationSize - ri - 1;
		//	rj = validPopulationSize - rj - 1;
			if (ri == rj) {
				crp[m] = population[ri];
				cre[m] = e[ri];
				cro[m] = o[ri];
				m ++;
				if (m >= validPopulationSize)
					break;
				crp[m] = population[rj];
				cre[m] = e[rj];
				cro[m] = o[rj];
				m ++;
				if (m >= validPopulationSize)
					break;
			}
			else {    // 选择的两个染色体不同
				vector<OutTs> tempO1(o[ri].size());
				//cout << "###### " << o[ri].size() << " " << o[rj].size() << endl;
//				cout << "1ri = " << ri << "rj = " << rj << endl;
				//cout << population[ri].table.size() << "  " << population[ri].table.size() << endl;
				crp[m] = cross(population[ri], o[ri], population[rj], o[rj], tempO1);
				cro[m] = tempO1;
				cre[m] = crp[m].evaluate();
				//cout << "1%  " << crp[m].table.size() << endl;
				m ++;
				vector<OutTs> tempO2(o[ri].size());
				if (m >= validPopulationSize)
					break;
				//cout << "%%%%%% " << o[rj].size() << " " << o[ri].size() << endl;
//				cout << "2ri = " << ri << "rj = " << rj << endl;
				//cout << population[ri].table.size() << "  " << population[rj].table.size() << endl;
				crp[m] = cross(population[rj], o[rj], population[ri], o[ri], tempO2);
				cro[m] = tempO2;
				cre[m] = crp[m].evaluate();
				//cout << "2%  " << crp[m].table.size() << endl;
				m ++;
				if (m >= validPopulationSize)
					break;
			
			}
			
		}
//		for (int g = 0; g < cro.size(); g ++) {
//			for (int f = 0; f < cro[g].size(); f ++)
//				if (crp[g].table[cro[g][f].offset][cro[g][f].slot] != cro[g][f].id)
///					cout << " err " << endl;
//		}
		// 交叉完毕 开始选择 优秀种群
		// 选择一个变异
	//	cout << "------------------------" << endl;
/*		for (int i = 0; i < cro.size(); i ++)
			cout << cro[i].size() << " ";
		cout << endl;
*/		for (int mu = 0; mu < mutateTime; mu ++) {
			int r = rand() % validPopulationSize;
			mutate(crp[r], cro[r]);
			cre[r] = crp[r].evaluate();
		}
	//	crp[r] = produceNewSolution(crp[r], cro[r]);
	//	cout << " mutate = " << cre[r] << endl;
/*		cout << "Before sort crp " << endl;
		for (int i = 0; i < e.size(); i ++) {
			cout << cre[i] << " " << crp[i].table.size() << " " << endl;
		}
		cout << endl;
*/
//		for (int i = 0; i < e.size(); i ++) {
//			cout << e[i] << " " << population[i].table.size() << " " << endl;
//		}
//		cout << endl;
		
			// 插入排序
		for (int t = 0; t < cre.size(); t ++) {
			int k = 0;
			for (k = e.size() - 1; k >= 0; k --) {
				if (cre[t] >= e[k])
					break;
			}
			int n = 0;
			for (n = e.size() - 2; n > k; n --) {
				e[n + 1] = e[n];
				o[n + 1] = o[n];
				population[n + 1] = population[n];
			}
			if (k + 1 >= 0 && k + 1 < e.size()) {
				population[k + 1] = crp[t];
				e[k + 1] = cre[t];
				o[k + 1] = cro[t];
			}
		}
//		cout << "After sort " << endl;
//		for (int i = 0; i < e.size(); i ++) {
//			cout << e[i] << " " << population[i].table.size() << " " << endl;
//		}
//		cout << endl;
		if ( minEval - e[0] > 0.001) {
			minEval = e[0];
			res = population[0];
			//cout << "cnt = " << cnt << "minEval = " << minEval << endl;
			addNew = 0;
			lastC = C - cnt + 1;
			exitTime = 0;
		}
		addNew ++;
		//cout << " add New = " << addNew << "tempNew  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& = " << tempNew << endl;
		if (addNew == tempNew) {
			tempNew = 1200 - cnt;
			tempNew = min(50, max(40, tempNew));
			addNew = 0;
			exitTime ++;
			//cout << "add New ********** " << endl;
			goto addN;
		}
		if (exitTime == 10) {
			break;
		}
		if (e[0] == e[validPopulationSize - 1]) {
			unChanged ++;
			if (unChanged == 50) {
		addN:		//cout << "add New population " << endl;
				int factorSize = 80; //validPopulationSize * 199 / 1; 
				vector<Bin> p(factorSize );
				vector<double> ee(factorSize , DBL_MAX);
				vector<vector<OutTs> > oo(factorSize , vector<OutTs>());
				if (true == addNewPopulation(v, bin, p, ee, oo, factorSize )) {
					int kk = validPopulationSize - 1;
					for (int i = p.size() - 1; i >= 0; i --, kk --) {
						population[kk] = p[i];
						e[kk] = ee[i];
						o[kk] = oo[i];
					}
					for (int i = 1; i < e.size(); i ++) {
						Bin tp = population[i];
						double te = e[i];
						vector<OutTs> to = o[i];
						int j = i - 1;
						for (j = i - 1; j >= 0; j --) {
							if (te < e[j]) {
								e[j + 1] = e[j];
								o[j + 1] = o[j];
								population[j + 1] = population[j];
							}
							else 
								break;
						}
						e[j + 1] = te;
						o[j + 1] = to;
						population[j + 1] = tp;
					}
					unChanged = 0;

					
				}
		}
		}
		else {
			unChanged = 0;
		}

	}

 FINISH:   if (hasSolution) {
		cout.setf(ios::fixed); 
                cout << setprecision(10) << "minEval value = " << minEval;
                //res.outPutTable();
        }
        else
                cout << "minEval value = " << -200;
//	res.outPutTable();
        return 0;
}


int main(int argc, char ** argv)
{
        vector<TsChannel> vts;
/*        TsChannel t1;
        t1.id = 1;
        t1.peroid = 10;
        t1.sendTime = 1;
        TsChannel t2;
        t2.id = 2;
        t2.peroid = 15;
        t2.sendTime = 2;
        TsChannel t3;
        t3.id = 3;
        t3.peroid = 20;
        t3.sendTime = 3;
*/
        ifstream f(argv[1]);
        int slotTime = 1;
        int slotNum = 5;
        f >> slotTime >> slotNum;
        while (!f.eof()) {
                TsChannel ts;
                f >> ts.id >> ts.peroid >> ts.sendTime;
		if (f.fail())
			break;
                vts.push_back(ts);
        }

        f.close();

//        vts.push_back(t1);
//        vts.push_back(t2);
//        vts.push_back(t3);
        clock_t start = clock();
        schedule(vts, slotTime, slotNum);
        clock_t finish = clock();
        cout << " time = " << (double)(finish - start) / CLOCKS_PER_SEC ;
	cout << " last = " << lastC << endl; 
        return 0;
}

