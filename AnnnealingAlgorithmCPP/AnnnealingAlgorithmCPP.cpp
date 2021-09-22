#include <iostream>
#include <ctime>

using namespace std;

//КОНСТАНТЫ
const int N = 10; //размер доски и число ферзей
const double Tn = 30.0; //начальная температура
const double Tk = 0.5; //конечная температура
const double Alfa = 0.98; //скорость охлаждения
int ST = 100; //число итераций при смене T

struct TMember
{
	int Plan[N]; //кодировка
	int Energy;
};

void Swap(TMember& M);
void New(TMember& M);
void CalcEnergy(TMember& M);
void Copy(TMember& MD, TMember& MS);
void Show(const TMember& M);

TMember Current; //текущее решение
TMember Working; //рабочее решение
TMember Best; //лучшее решение
double T; //температура
double Delta; //разница энергий
double P; //вероятность допуска
bool fNew; //флаг нового решения
bool fBest; //флаг лучшего решения
long int Time; //этап поиска
long int Step; //шаг на этапе поиска
long int Accepted; //число новых решений

int main() {
	srand(time(0));
	T = Tn;
	fBest = false;
	Time = 0;
	Best.Energy = 100;
	New(Current);
	CalcEnergy(Current);
	Copy(Working, Current);
	while (T > Tk)
	{
		Accepted = 0;
		for (Step = 0; Step <= ST; Step++) {
			fNew = false;
			Swap(Working);
			CalcEnergy(Working);
			if (Working.Energy <= Current.Energy) {
				fNew = true;
			}
			else {
				Delta = Working.Energy - Current.Energy;
				P = exp(-Delta / T);
				auto r = rand();
				if (P > r % 2) {
					Accepted += 1;
					fNew = true;
				}
			}
			if (fNew) {
				fNew = false;
				Copy(Current, Working);
				if (Current.Energy < Best.Energy) {
					Copy(Best, Current);
					fBest = true;
				}
			}
			else {
				Copy(Working, Current);
			}
			cout << "Best: "<< Best.Energy << endl;
		}
		cout << "Temp= " << T << " Energy= " << Best.Energy << endl;
		T = T * Alfa;
		Time++;
	}
	if (fBest) {
		Show(Best);
	}
}

void Swap(TMember& M) {
	int x, y, v;
	x = rand() % N;
	do
		y = rand() % N;
	while (x == y);
	v = M.Plan[x];
	M.Plan[x] = M.Plan[y];
	M.Plan[y] = v;
}

void New(TMember& M) {
	for (int i = 0; i < N; i++) {
		M.Plan[i] = i;
	}
	for (int i = 0; i < N; i++) {
		Swap(M);
	}
}

void CalcEnergy(TMember& M) {
	const int dx[] = { -1,1,-1,1 };
	const int dy[] = { -1,1,1,-1 };
	int j, x, tx = 0, ty = 0, error = 0;
	error = 0;
	for (x = 0; x < N; x++) {
		for (j = 0; j < 4; j++) {
			tx = x + dx[j];
			ty = M.Plan[x] + dy[j];
			while ((tx >= 0) && (tx < N) && (ty >= 0) && (ty < N)) {
				if (M.Plan[tx] == ty) {
					error++;
				}
				tx = tx + dx[j];
				ty = ty + dy[j];
			}
		}
	}
	M.Energy = error;
}

void Copy(TMember& MD, TMember& MS) {
	for (int i = 0; i < N; i++) {
		MD.Plan[i] = MS.Plan[i];
	}
	MD.Energy = MS.Energy;
}

void Show(const TMember& M) {
	cout << "Solve " << endl;
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			if (M.Plan[x] == y) {
				cout << "Q";
			}
			else {
				cout << "*";
			}
		}
		cout << endl;
	}
	cout << endl;
}