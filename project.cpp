
#include <string.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
using namespace std;

int *SP;  const int q = 33554393; const int d = 26;
void initSP(string p, int num);
void kmpsearch(int shortread_num, string a);
void kmp_alpha_search(int shortread_num, string a);
void make_input(int n);
void make_reference(int n);
void make_shortread(int n, int shortread_num);
void cal_accuracy(string changed_reference, int algo);

class alpha {
public:
	string pattern = "none";
	int state[10];
	alpha() {
		for (int i = 0; i < 10; i++) state[i] = -1;
	}
}*alphas;


void main() {

	string str;//test string�� ���� �������� �޾ƿ� �� �ְ� �ϱ� ���Ͽ� string ���
	char pattern[50];

	int n, shortread_num, shortread_length;

	//pattern�� n�� �� �Է�
	cout << "N = ";
	cin >> n;
	make_input(n);
	make_reference(n);
	cout << "shortread_num = ";
	cin >> shortread_num;
	cout << endl;
	make_shortread(n, shortread_num);

	//make_input���� ������� input.txt�� �о�� str�� ����
	ifstream readfile;
	readfile.open("myreference.txt");
	if (readfile.is_open() == true) {
		while (!readfile.eof())
		{
			getline(readfile, str, ' ');
		}
	}
	readfile.close();

	//��Ʈ�� �˻� �Լ����� ����
	kmpsearch(shortread_num, str);
	kmp_alpha_search(shortread_num, str);

}



//KMP
void kmpsearch(int shortread_num, string a) {

	int i, j;
	int m;
	int n = a.size();
	int count = 0;
	int find_count = 0;


	int mismatch = 0;
	string str;

	string changed_reference;

	for (int i = 0; i < n; i++)
		changed_reference += 'B';

	clock_t start, finish;
	double duration;

	int count_pattern_start = 0;

	int check_real_first_check = 0;//ù ���� �ٸ� ���� ���ؼ��� if���� ������ ���ؼ� check�ϴ� ����
	int find_check = 0;//ã�� �ٷ� ���� for�� ������ �����ϴ� ����

	int *mismatch_check;
	int *mismatch_check_temp = new int[30];

	ifstream readshortread;
	readshortread.open("shortread.txt");

	int make_arr_count = 0;
	int pattern_count = 0;

	start = clock();

	for (int k = 0; k < shortread_num - 1; k++) {

		mismatch = 0;
		getline(readshortread, str, ' ');

		m = str.length();

		//�������ʹ� ã�� ����� reference�� �����ϸ鼭 ���� �߰��� �κп� ã�� pattern�� ���� ����ְ� �ȴ�.


		SP = new int[m];

		mismatch_check = new int[m];

		alphas = new alpha[m];



		//patern�� ���� prefix table ����
		initSP(str, count);



		int alphas_search_start = m - 3;
		int alphas_d = 1;
		int pattern_find = 0;
		int SP_check = 0;
		string temp;
		//table�� ���Ͽ� 
		for (i = 0, j = -1; i <= n - 1; i++) { //mismatch�ѵڷ�, i�� 0������ 1�� ���ƿ;� �Ѵ�.
			count++;

			check_real_first_check = 0;

			if ((j >= 0) && (str[j + 1] != a[i])) { // �ٸ� �� --> ó������ Ʋ������ �������� �Ѵ�. -->������ ����ؼ� j�� -1�ɋ����� j�� �ű��. -->
				mismatch++;
				if (mismatch == 3) {//�̹� 2���� mismatch�� ������ٸ�
					while ((j >= 0) && (str[j + 1] != a[i])) {
						j = SP[j];
						SP_check = 1;
					}
					int mismatch_temp = mismatch;
					mismatch = 0;
					if (j == -1) {
						for (int k = 0; k < mismatch_temp; k++) {
							mismatch_check[k] = -1;
							mismatch_check_temp[k] = -1;
						}
					}
					else {
						for (int k = 0; k < j + 1; k++) {
							mismatch_check[k] = mismatch_check_temp[k];
							if (mismatch_check_temp[k] == 1) mismatch++;
						}
					}
				}
				else {//case *
					mismatch_check[j + 1] = 1; //mismatch�� ���� 1�� ����
					mismatch_check_temp[j + 1] = 1;
					j++;
					check_real_first_check = 1;
				}
				count++;
			}

			//case * ��쿡, j++�� �ǰ��� str[j+1] == a[i]�ϼ��ִ�.
			if (str[j + 1] == a[i] && check_real_first_check == 0) {//������
				mismatch_check[j + 1] = 0;//mismatch�� �ƴ� ��쿡�� mismatch_check�� 0�� ����
				mismatch_check_temp[j + 1] = 0;
				j++;
			}
			else if (j == -1 && str[j + 1] != a[i]) {//ù ���� �ٸ� ��,
				mismatch++;
				mismatch_check[0] = 1;
				mismatch_check_temp[0] = 1;
				j++;
			}
			if (j == m - 1) {//pattern�� ���� ��ġ�� ã������, result�� ��������� �Ѵ�. 
				j = SP[j];
				//      cout << i - m + 1 << " ";
				int count_pattern_location = 0;
				for (int q = i - m + 1; q < i + 1; q++) {
					changed_reference[q] = str[count_pattern_location];
					count_pattern_location++;
				}
				find_count++;
				find_check = 1;
				mismatch = 0;
			}
		}


		if (k == 1 && i == 10000)
			cout << ".";
		else
			cout << ".";
	}

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;

	ofstream kmp_result;
	kmp_result.open("kmp_result.txt");
	for (int i = 0; i < n; i++)
		kmp_result << changed_reference[i];
	kmp_result.close();



	cout << endl;
	cout << "KMP �� Ƚ�� = " << count << endl;
	cout << "KMP ���� �ð� = " << duration << endl << endl;


	cal_accuracy(changed_reference, 0);

}


void kmp_alpha_search(int shortread_num, string a) {

	int i, j;
	int m;
	int n = a.size();
	int count = 0;
	int find_count = 0;


	int mismatch = 0;
	string str;

	string changed_reference;

	for (int i = 0; i < n; i++)
		changed_reference += 'B';

	clock_t start, finish;
	double duration;

	int count_pattern_start = 0;

	int check_real_first_check = 0;//ù ���� �ٸ� ���� ���ؼ��� if���� ������ ���ؼ� check�ϴ� ����
	int find_check = 0;//ã�� �ٷ� ���� for�� ������ �����ϴ� ����

	int *mismatch_check;
	int *mismatch_check_temp = new int[30];

	ifstream readshortread;
	readshortread.open("shortread.txt");

	int make_arr_count = 0;
	int pattern_count = 0;

	start = clock();

	for (int k = 0; k < shortread_num - 1; k++) {

		mismatch = 0;
		getline(readshortread, str, ' ');

		m = str.length();


		//�������ʹ� ã�� ����� reference�� �����ϸ鼭 ���� �߰��� �κп� ã�� pattern�� ���� ����ְ� �ȴ�.


		SP = new int[m];

		mismatch_check = new int[m];

		alphas = new alpha[m];


		int *pattern_state_count = new int[m];
		for (int i = 0; i < m; i++) {
			pattern_state_count[i] = 0;
		}

		pattern_count = 0;


		//������ ������ �迭�� �����ؼ� ��ġ�� �����Ѵ�.
		for (int i = 0; i < m - 2; i++) {
			string temp;
			temp = str[i];
			temp += str[i + 1];
			temp += str[i + 2];
			if (make_arr_count == 0) {
				alphas[0].pattern = temp;
				make_arr_count = 1;
				alphas[0].state[0] = 0;
				pattern_count++;
				pattern_state_count[0]++;
			}
			else {
				int find_check = 0;
				for (int j = 0; j < pattern_count; j++) {
					if (alphas[j].pattern == temp) {
						alphas[j].state[pattern_state_count[j]] = i;
						pattern_state_count[j]++;
						find_check = 1;
					}
				}
				if (find_check != 1) {
					alphas[pattern_count].pattern = temp;
					alphas[pattern_count].state[0] = i;
					pattern_state_count[pattern_count]++;
					pattern_count++;
				}

			}
		}
		//alphas skip pattern table�� ������.




		//patern�� ���� prefix table ����
		initSP(str, count);



		int alphas_search_start = m - 3;
		int alphas_d = 1;
		int pattern_find = 0;
		int SP_check = 0;
		string temp;
		//table�� ���Ͽ� 
		for (i = 0, j = -1; i <= n - 1; i++) {
			count++;

			check_real_first_check = 0;


			if (i == (m + 6 * alphas_d - 9) && SP_check == 1) {
				if (i + m >= n)break;
				alphas_d++;
				int alpha_mismatch = 0;
				int pattern_location_count = 0;
				temp = "";
				temp += a[i];
				temp += a[i + 1];
				temp += a[i + 2];
				for (int g = 0; g < pattern_count; g++) {
					if (pattern_find == 1)
						break;
					if (temp == alphas[g].pattern) {//alphas�� �̿��Ͽ� pattern�� ã��.
					//ã�� pattern�� state���� ã�Ƽ� Ž����.
						pattern_find = 1;
						for (int b = 0; b < pattern_state_count[g]; b++) {
							alpha_mismatch = 0;
							pattern_location_count = 0;
							for (int e = i; e < i + m; ) {
								if (a[e] != str[pattern_location_count]) {
									alpha_mismatch++;
								}

								e++;
								pattern_location_count++;
								if (e == i + alphas[g].state[b])
									e += 3;


								if (alpha_mismatch == 3) break;
								if (e == i + m) {//match�Ǵ� ���� ã����.
									int count_pattern_location = 0;
									for (int q = i; q < i + m; q++) {
										changed_reference[q] = str[count_pattern_location];
										count_pattern_location++;
									}

								}

							}
						}
					}
				}
				pattern_find = 0;
			}

			if ((j >= 0) && (str[j + 1] != a[i])) { // �ٸ� �� --> ó������ Ʋ������ �������� �Ѵ�. -->������ ����ؼ� j�� -1�ɋ����� j�� �ű��. -->
				mismatch++;
				if (mismatch == 3) {//�̹� 2���� mismatch�� ������ٸ�
					while ((j >= 0) && (str[j + 1] != a[i])) {
						j = SP[j];
						SP_check = 1;
					}
					int mismatch_temp = mismatch;
					mismatch = 0;
					if (j == -1) {
						for (int k = 0; k < mismatch_temp; k++) {
							mismatch_check[k] = -1;
							mismatch_check_temp[k] = -1;
						}
					}
					else {
						for (int k = 0; k < j + 1; k++) {
							mismatch_check[k] = mismatch_check_temp[k];
							if (mismatch_check_temp[k] == 1) mismatch++;
						}
					}
				}
				else {//case *
					mismatch_check[j + 1] = 1; //mismatch�� ���� 1�� ����
					mismatch_check_temp[j + 1] = 1;
					j++;
					check_real_first_check = 1;
				}
				count++;
			}

			//case * ��쿡, j++�� �ǰ��� str[j+1] == a[i]�ϼ��ִ�.
			if (str[j + 1] == a[i] && check_real_first_check == 0) {//������
				mismatch_check[j + 1] = 0;//mismatch�� �ƴ� ��쿡�� mismatch_check�� 0�� ����
				mismatch_check_temp[j + 1] = 0;
				j++;
			}
			else if (j == -1 && str[j + 1] != a[i]) {//ù ���� �ٸ� ��,
				mismatch++;
				mismatch_check[0] = 1;
				mismatch_check_temp[0] = 1;
				j++;
			}
			if (j == m - 1) {//pattern�� ���� ��ġ�� ã������, result�� ��������� �Ѵ�. 
				j = SP[j];
				//		cout << i - m + 1 << " ";
				int count_pattern_location = 0;
				for (int q = i - m + 1; q < i + 1; q++) {
					changed_reference[q] = str[count_pattern_location];
					count_pattern_location++;
				}

				find_count++;
				find_check = 1;
				mismatch = 0;
			}
		}


		cout << ".";
	}

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;

	ofstream kmp_result;
	kmp_result.open("kmp_alpha_result.txt");
	for (int i = 0; i < n; i++)
		kmp_result << changed_reference[i];
	kmp_result.close();



	cout << endl;
	cout << "KMP_alhpa �� Ƚ�� = " << count << endl;
	cout << "KMP_alhpa ���� �ð� = " << duration << endl << endl;


	cal_accuracy(changed_reference, 1);

}


//��Ȯ���� ����ϴ� �Լ�
void cal_accuracy(string chaged_reference, int algo) {

	string original;
	string result;

	ifstream mygenome;
	mygenome.open("mygenome.txt");
	getline(mygenome, original, ' ');
	mygenome.close();

	ifstream myreference_result;
	myreference_result.open("result.txt");
	getline(myreference_result, result, ' ');
	myreference_result.close();

	int n = original.size();
	int count_not_equal = 0;

	for (int i = 0; i < n; i++) {
		if (original[i] != chaged_reference[i])count_not_equal++;
	}

	double accuracy = 1 - ((double)count_not_equal / (double)n);

	ofstream result_txt;
	if (algo == 0) {
		result_txt.open("kmp_accuracy.txt");
		result_txt << "��Ȯ����, �� " << n << "���� ���� �߿���, " << count_not_equal << "���� ��ġ���� �ʾ����Ƿ�,";
		result_txt << accuracy * 100 << "%�� ��Ȯ���� �����ϴ�." << endl;
	}
	else if (algo == 1) {
		result_txt.open("kmp_alpha_accuracy.txt");
		result_txt << "��Ȯ����, �� " << n << "���� ���� �߿���, " << count_not_equal << "���� ��ġ���� �ʾ����Ƿ�,";
		result_txt << accuracy * 100 << "%�� ��Ȯ���� �����ϴ�." << endl;
	}


	cout << endl << "��Ȯ����, �� " << n << "���� ���� �߿���, " << count_not_equal << "���� ��ġ���� �ʾ����Ƿ�,";
	cout << accuracy * 100 << "%�� ��Ȯ���� �����ϴ�." << endl;
}


void initSP(string p, int num) {

	int i, j;
	int m = p.length();

	SP[0] = -1;

	for (i = 1, j = -1; i <= m - 1; i++) {
		num++;
		while ((j >= 0) && (p[j + 1] != p[i])) { j = SP[j]; num++; }
		if (p[j + 1] == p[i]) j++;
		SP[i] = j;
	}
}



//n�� ũ���� ������ A/C/G/T�� ���Ͽ� ����ϴ� �Լ�
void make_input(int n) {

	string Path = "mygenome.txt";

	srand(time(NULL));
	int num;

	int count = 0;
	ofstream writeFile(Path);

	while (count != n) {
		num = rand() % 4;
		if (writeFile.is_open()) {
			if (num == 0) writeFile << "A";
			else if (num == 1) writeFile << "C";
			else if (num == 2) writeFile << "G";
			else if (num == 3) writeFile << "T";
		}
		count++;
	}
	writeFile.close();
}


void make_reference(int n) {

	ifstream mygenome;
	ofstream myreference;
	mygenome.open("mygenome.txt");
	myreference.open("myreference.txt");
	int count;
	if (mygenome.is_open() == true) {
		while (!mygenome.eof()) {
			char temp;
			mygenome.get(temp);
			count = rand() % 1000 + 1;

			if (count <= 10) {
				int random = rand() % 4;

				if (random < 4) {
					if (random == 0) myreference << "A";
					else if (random == 1) myreference << "C";
					else if (random == 2) myreference << "G";
					else if (random == 3) myreference << "T";
				}
			}
			else myreference << temp;
		}

	}
	mygenome.close();
	myreference.close();

}


//�ߺ��Ǵ� �κ��� �����ϵ��� ������ �Ѵ�.
void make_shortread(int n, int shortread_num) {//n�� mygenome�� ������, shortread_num�� shortread�� ����
	ifstream mygenome;
	ofstream shortread;
	mygenome.open("mygenome.txt");
	shortread.open("shortread.txt");

	string str;
	int random_location = 0;
	int random_location_plus = 0;
	int random_length; // ���̰� 20���� 30�� short read ����
	int check = 0;

	if (mygenome.is_open() == true) {
		while (!mygenome.eof()) {
			getline(mygenome, str, ' ');
		}
	}//str�� mygenome ���� ������.
	mygenome.close();

	//���� �� ���� ��ġ�� ���� ������ ���̸��� �����Ͽ� shortread.txt�� ���پ� �����Ѵ�.

	while (shortread_num != 0) {
		random_length = rand() % 11 + 20;
		random_location_plus = rand() % random_length;
		random_location += random_location_plus; // �ߺ��ɼ��� �ְ� �ߺ����� ���� ���� ����.
		if (random_location + random_length - 1 > n) { //����ó�� - random ��ġ���� ������ pattern�� ���̰� reference�� �� �� �Ѿ ��쿡.
			int random = rand() % 2;
			if (random == 1) random_location = n - random_length;//������ pattern ���� ��ŭ�� string�� ��ġ�� ������ location�� �����Ѵ�. 
			else random_location = rand() % 5;
		}
		while (random_length != 0) {
			shortread << str[random_location];
			random_length--;
			random_location++;
		}
		shortread_num--;
		shortread << ' ';
	}
	shortread.close();
}

