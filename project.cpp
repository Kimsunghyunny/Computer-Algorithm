
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

	string str;//test string의 수가 많아져도 받아올 수 있게 하기 위하여 string 사용
	char pattern[50];

	int n, shortread_num, shortread_length;

	//pattern과 n의 수 입력
	cout << "N = ";
	cin >> n;
	make_input(n);
	make_reference(n);
	cout << "shortread_num = ";
	cin >> shortread_num;
	cout << endl;
	make_shortread(n, shortread_num);

	//make_input으로 만들어진 input.txt를 읽어와 str에 저장
	ifstream readfile;
	readfile.open("myreference.txt");
	if (readfile.is_open() == true) {
		while (!readfile.eof())
		{
			getline(readfile, str, ' ');
		}
	}
	readfile.close();

	//스트링 검색 함수들을 실행
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

	int check_real_first_check = 0;//첫 값이 다를 때에 대해서만 if문을 돌리기 위해서 check하는 변수
	int find_check = 0;//찾은 바로 후의 for문 인지를 구분하는 변수

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

		//이제부터는 찾은 결과를 reference를 복사하면서 새로 발견한 부분에 찾은 pattern의 값을 집어넣게 된다.


		SP = new int[m];

		mismatch_check = new int[m];

		alphas = new alpha[m];



		//patern에 대한 prefix table 만듦
		initSP(str, count);



		int alphas_search_start = m - 3;
		int alphas_d = 1;
		int pattern_find = 0;
		int SP_check = 0;
		string temp;
		//table과 비교하여 
		for (i = 0, j = -1; i <= n - 1; i++) { //mismatch한뒤로, i가 0다음에 1로 돌아와야 한다.
			count++;

			check_real_first_check = 0;

			if ((j >= 0) && (str[j + 1] != a[i])) { // 다를 때 --> 처음부터 틀릴때도 고려해줘야 한다. -->조건은 계속해서 j가 -1될떄까지 j를 옮긴다. -->
				mismatch++;
				if (mismatch == 3) {//이미 2개의 mismatch를 봐줬었다면
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
					mismatch_check[j + 1] = 1; //mismatch인 경우면 1을 저장
					mismatch_check_temp[j + 1] = 1;
					j++;
					check_real_first_check = 1;
				}
				count++;
			}

			//case * 경우에, j++가 되고나서 str[j+1] == a[i]일수있다.
			if (str[j + 1] == a[i] && check_real_first_check == 0) {//같으면
				mismatch_check[j + 1] = 0;//mismatch가 아닌 경우에는 mismatch_check에 0값 저장
				mismatch_check_temp[j + 1] = 0;
				j++;
			}
			else if (j == -1 && str[j + 1] != a[i]) {//첫 값이 다를 때,
				mismatch++;
				mismatch_check[0] = 1;
				mismatch_check_temp[0] = 1;
				j++;
			}
			if (j == m - 1) {//pattern이 같은 위치를 찾았을때, result에 삽입해줘야 한다. 
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
	cout << "KMP 비교 횟수 = " << count << endl;
	cout << "KMP 수행 시간 = " << duration << endl << endl;


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

	int check_real_first_check = 0;//첫 값이 다를 때에 대해서만 if문을 돌리기 위해서 check하는 변수
	int find_check = 0;//찾은 바로 후의 for문 인지를 구분하는 변수

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


		//이제부터는 찾은 결과를 reference를 복사하면서 새로 발견한 부분에 찾은 pattern의 값을 집어넣게 된다.


		SP = new int[m];

		mismatch_check = new int[m];

		alphas = new alpha[m];


		int *pattern_state_count = new int[m];
		for (int i = 0; i < m; i++) {
			pattern_state_count[i] = 0;
		}

		pattern_count = 0;


		//패턴을 이차원 배열에 저장해서 위치를 저장한다.
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
		//alphas skip pattern table을 만들음.




		//patern에 대한 prefix table 만듦
		initSP(str, count);



		int alphas_search_start = m - 3;
		int alphas_d = 1;
		int pattern_find = 0;
		int SP_check = 0;
		string temp;
		//table과 비교하여 
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
					if (temp == alphas[g].pattern) {//alphas를 이용하여 pattern을 찾음.
					//찾은 pattern의 state들을 찾아서 탐색함.
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
								if (e == i + m) {//match되는 것을 찾았음.
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

			if ((j >= 0) && (str[j + 1] != a[i])) { // 다를 때 --> 처음부터 틀릴때도 고려해줘야 한다. -->조건은 계속해서 j가 -1될떄까지 j를 옮긴다. -->
				mismatch++;
				if (mismatch == 3) {//이미 2개의 mismatch를 봐줬었다면
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
					mismatch_check[j + 1] = 1; //mismatch인 경우면 1을 저장
					mismatch_check_temp[j + 1] = 1;
					j++;
					check_real_first_check = 1;
				}
				count++;
			}

			//case * 경우에, j++가 되고나서 str[j+1] == a[i]일수있다.
			if (str[j + 1] == a[i] && check_real_first_check == 0) {//같으면
				mismatch_check[j + 1] = 0;//mismatch가 아닌 경우에는 mismatch_check에 0값 저장
				mismatch_check_temp[j + 1] = 0;
				j++;
			}
			else if (j == -1 && str[j + 1] != a[i]) {//첫 값이 다를 때,
				mismatch++;
				mismatch_check[0] = 1;
				mismatch_check_temp[0] = 1;
				j++;
			}
			if (j == m - 1) {//pattern이 같은 위치를 찾았을때, result에 삽입해줘야 한다. 
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
	cout << "KMP_alhpa 비교 횟수 = " << count << endl;
	cout << "KMP_alhpa 수행 시간 = " << duration << endl << endl;


	cal_accuracy(changed_reference, 1);

}


//정확도를 계산하는 함수
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
		result_txt << "정확도는, 총 " << n << "개의 문자 중에서, " << count_not_equal << "개가 일치하지 않았으므로,";
		result_txt << accuracy * 100 << "%의 정확도를 가집니다." << endl;
	}
	else if (algo == 1) {
		result_txt.open("kmp_alpha_accuracy.txt");
		result_txt << "정확도는, 총 " << n << "개의 문자 중에서, " << count_not_equal << "개가 일치하지 않았으므로,";
		result_txt << accuracy * 100 << "%의 정확도를 가집니다." << endl;
	}


	cout << endl << "정확도는, 총 " << n << "개의 문자 중에서, " << count_not_equal << "개가 일치하지 않았으므로,";
	cout << accuracy * 100 << "%의 정확도를 가집니다." << endl;
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



//n의 크기대로 임의의 A/C/G/T를 파일에 출력하는 함수
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


//중복되는 부분이 존재하도록 만들어야 한다.
void make_shortread(int n, int shortread_num) {//n은 mygenome의 사이즈, shortread_num은 shortread의 갯수
	ifstream mygenome;
	ofstream shortread;
	mygenome.open("mygenome.txt");
	shortread.open("shortread.txt");

	string str;
	int random_location = 0;
	int random_location_plus = 0;
	int random_length; // 길이가 20에서 30인 short read 길이
	int check = 0;

	if (mygenome.is_open() == true) {
		while (!mygenome.eof()) {
			getline(mygenome, str, ' ');
		}
	}//str에 mygenome 값을 복사함.
	mygenome.close();

	//이제 각 랜덤 위치의 값과 랜덤의 길이마다 복사하여 shortread.txt에 한줄씩 저장한다.

	while (shortread_num != 0) {
		random_length = rand() % 11 + 20;
		random_location_plus = rand() % random_length;
		random_location += random_location_plus; // 중복될수도 있고 중복되지 않을 수도 있음.
		if (random_location + random_length - 1 > n) { //예외처리 - random 위치에서 시작한 pattern의 길이가 reference의 끝 을 넘어선 경우에.
			int random = rand() % 2;
			if (random == 1) random_location = n - random_length;//끝나는 pattern 길이 만큼의 string의 위치를 가져와 location을 지정한다. 
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

