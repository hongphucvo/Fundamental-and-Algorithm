// xepsach.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
vector <int> xep;

void split(vector <int> arr) {
	vector <int> nho;
	vector <int> lon;

	for (int i = 1; i < arr.size(); i++) {
		if (arr[i] < arr[0])
			nho.push_back(arr[i]);
		else lon.push_back(arr[i]);
	}

	if (nho.size() > 0) {
            xep.insert(xep.begin(),arr[0]);
			split(nho);
			}
    else {
        xep.push_back(arr[0]);
    }
	if (lon.size() == 0) {
			xep.push_back(xep[0]);
			xep.erase(xep.begin());		
			return;
			}
	else {
		split(lon);
	}
}

void input(vector <int > sach) {
	int a;
	do { 
		cin >> a;  
		sach.push_back(a);		
	} while (a != -1);
	sach.erase(sach.end()-1);	
}
void output() {
    xep.insert(xep.begin(),xep[xep.size()]);
	//chũaoaduoc   xep.erase(xep.end());
	for (int i = 0; i < xep.size()-1; i++) {
			cout << xep[i];
		}
 }


int main()
{
	vector <int> sach; 
	input(sach);
	split(sach);
	output();
	return 0;
}
