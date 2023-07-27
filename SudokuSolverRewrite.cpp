#include <iostream>
#include "miles.cpp"
//example sudoku here: 123456789123456789123456789123456789123456789123456789123456789123456789123456789

//famous easy computer sudoku
//                     530070000600195000098000060800060003400803001700020006060000280000419005000080079
//                     534678912672195348198342567859761423426853791713924856961537284287419635345286179

//mimi's book #32
//                     009004001052000004000083027091050060000308000030010780120430000500000190900100300
//                     389274651752691834416583927891752463675348219234916785128439576543867192967125348

//mimi's book #64       
//                     060005100200009007000041006150000090049000720020000061400530000700800005005900040
//                     964375182213689457578241936156723894849156723327498561481532679792864315635917248

//world's hardest
//                     800000000003600000070090200050007000000045700000100030001000068008500010090000400
//                     862751349943628157571493286159387624386245791724169835217934568438576912695812473
#define SUDOKUVOLUME 81 //these are not guaranteed to successfully alter the program
#define SUDOKUSIZE 9
#define SECTIONSIZE 3 //must be even factor into sudokucount
#define SECTIONCOUNT (SUDOKUSIZE/SECTIONSIZE)
#define SECTIONTOTALCOUNT (SECTIONCOUNT*SECTIONCOUNT)
#define UNSOLVEDMARKER 0 
#define inrange(x,a,b) ( (x) >= (a) && (x) <= (b) )
//inrange is inclusive
void eliminatepossibilities(milesArray& remaining, milesArray& sudoku, int location);
bool isLegal(milesArray&);
int algorithm1(milesArray&);
int algorithm2(milesArray&);
void emptyGarbage(milesArray&);
void countup(milesArray&);
int findnot(milesArray&, int);
int sudokudimension(int, int);
int sudokusectiondimension(int, int, int);
int sudokusectiondimension(int, int);
int row(int);
int col(int);
int extra(int);
int section(int);

int main()
{	
	std::cout << "Enter Sudoku Here, number by number, with unknowns marked as 0\n";
	char input[SUDOKUVOLUME + 1];
	std::cin >> input;
	int inputtoints[SUDOKUVOLUME];
	for (int i = 0; i < SUDOKUVOLUME; i++) {
		inputtoints[i] = input[i] - '0';
	}
	milesArray sudoku(inputtoints, SUDOKUVOLUME);
	emptyGarbage(sudoku);
	if (!isLegal(sudoku)) {
		std::cout << "The given Sudoku has been identified illegal. Press a key then ENTER to continue...";
		char x;
		std::cin >> x;
		return 0;
	}
	milesArray oldsudoku = SUDOKUVOLUME;
	oldsudoku.fill(UNSOLVEDMARKER);
	milesInt debugcount;
	while (!(sudoku == oldsudoku)) {
		oldsudoku = sudoku;
		try {
			std::cout << "Algorithm 1, Try #" << (debugcount.read() + 1) << ": " << algorithm1(sudoku) << " Filled\n";
			std::cout << "Algorithm 2, Try #" << (debugcount.read() + 1) << ": " << algorithm2(sudoku) << " Filled\n";
		}
		catch (...) {
			std::cout << "The given Sudoku has been identified impossible. Press a key then ENTER to continue...";
			char x;
			std::cin >> x;
			return 0;
		}
		debugcount++;
	}
	std::cout << "Progress:\n";
	for (int i = 0; i < SUDOKUVOLUME; i++) {
		std::cout<<sudoku[i];
	}
}

bool isLegal(milesArray& sudoku) {
	for (int square = 0; square < SUDOKUVOLUME; square++) {
		if (sudoku[square] == UNSOLVEDMARKER) continue;
		for (int r = row(square) + 1; r < SUDOKUSIZE; r++) {
			int calc = sudokudimension(r, col(square));
			if (sudoku[square] == sudoku[calc]) return false;
		}
		for (int c = col(square) + 1; c < SUDOKUSIZE; c++) {
			int calc = sudokudimension(row(square), c);
			if (sudoku[square] == sudoku[calc]) return false;
		}
		for (int e = extra(square) + 1; e < SUDOKUSIZE; e++) {
			int calc = sudokusectiondimension(section(square), e);
			if (sudoku[square] == sudoku[calc]) return false;
		}
	}
	return true;
}

int algorithm1(milesArray& sudoku) { //for each row, col, and section: for each square: if only one number, use
	int ret = 0;
	for (int square = 0; square < SUDOKUVOLUME; square++) {
		if (sudoku[square] != UNSOLVEDMARKER) continue;
		milesArray remaining = SUDOKUSIZE;
		countup(remaining);
		eliminatepossibilities(remaining, sudoku, square);
		int remainingcount = remaining.count(UNSOLVEDMARKER);
		if (remainingcount == SUDOKUSIZE - 1) { //there is only 1 good value left in remaining; didn't use remove() a bunch of times on list because would be extremely inefficient
			sudoku[square] = findnot(remaining, UNSOLVEDMARKER) + 1; //finds only non-zero value in remaining
			ret++;
		}
		if (remainingcount == SUDOKUSIZE) {
			throw "Impossible Sudoku Detected";
		}
	}
	return ret;
}

int algorithm2(milesArray& sudoku) {
	int ret = 0;
	//rows:
	for (int row = 0; row < SUDOKUSIZE; row++) {
		for (int number = 1; number <= SUDOKUSIZE; number++) {
			milesArray candidateSquares = SUDOKUSIZE;
			candidateSquares.fill(false);
			bool broken = 0;
			for (int col = 0; col < SUDOKUSIZE; col++) {
				int calc = sudokudimension(row, col);
				if (sudoku[calc] == number) { broken = 1; break; }
				if (sudoku[calc] != UNSOLVEDMARKER) continue;
				milesArray remaining = SUDOKUSIZE;
				countup(remaining);
				eliminatepossibilities(remaining, sudoku, calc);
				if (remaining.find(number) != SUDOKUSIZE) {
					candidateSquares[col] = true;
				}
			}
			if (candidateSquares.count(true) == 1) {
				sudoku[sudokudimension(row, candidateSquares.find(true))] = number;
				ret++;
				continue;
			}
			if (candidateSquares.count(true) == 0 && !broken) {
				throw "Impossible Sudoku Detected";
			}
		}
	}
	//cols:
	for (int col = 0; col < SUDOKUSIZE; col++) {
		for (int number = 1; number <= SUDOKUSIZE; number++) {
			milesArray candidateSquares = SUDOKUSIZE;
			candidateSquares.fill(false);
			bool broken = 0;
			for (int row = 0; row < SUDOKUSIZE; row++) {
				int calc = sudokudimension(row, col);
				if (sudoku[calc] == number) { broken = 1; break; }
				if (sudoku[calc] != UNSOLVEDMARKER) continue;
				milesArray remaining = SUDOKUSIZE;
				countup(remaining);
				eliminatepossibilities(remaining, sudoku, calc);
				if (remaining.find(number) != SUDOKUSIZE) {
					candidateSquares[row] = true;
				}
			}
			if (candidateSquares.count(true) == 1) {
				sudoku[sudokudimension(candidateSquares.find(true), col)] = number;
				ret++;
				continue;
			}
			if (candidateSquares.count(true) == 0 && !broken) {
				throw "Impossible Sudoku Detected";
			}
		}
	}
	//sections:
	for (int section = 0; section < SECTIONTOTALCOUNT; section++) {
		for (int number = 1; number <= SUDOKUSIZE; number++) {
			milesArray candidateSquares = SUDOKUSIZE;
			candidateSquares.fill(false);
			bool broken = 0;
			for (int extra = 0; extra < SUDOKUSIZE; extra++) {
				int calc = sudokusectiondimension(section, extra);
				if (sudoku[calc] == number) { broken = 1; break; }
				if (sudoku[calc] != UNSOLVEDMARKER) continue;
				milesArray remaining = SUDOKUSIZE;
				countup(remaining);
				eliminatepossibilities(remaining, sudoku, calc);
				if (remaining.find(number) != SUDOKUSIZE) {
					candidateSquares[extra] = true;
				}
			}
			if (candidateSquares.count(true) == 1) {
				sudoku[sudokusectiondimension(section, candidateSquares.find(true))] = number;
				ret++;
				continue;
			}
			if (candidateSquares.count(true) == 0 && !broken) {
				throw "Impossible Sudoku Detected";
			}
		}
	}
	return ret;
}

void eliminatepossibilities(milesArray& remaining, milesArray& sudoku, int location) {
	//next, we are targeting squares in the sudoku-view and seeing if they have numbers, if so we take those numbers away from the options, if there is one option left then use that
	for (int row = 0; row < SUDOKUSIZE; row++) {
		int current = sudoku[sudokudimension(row, col(location))];
		if (current != UNSOLVEDMARKER) {
			remaining[current - 1] = UNSOLVEDMARKER;
		}
	}
	for (int col = 0; col < SUDOKUSIZE; col++) {
		int current = sudoku[sudokudimension(row(location), col)];
		if (current != UNSOLVEDMARKER) {
			remaining[current - 1] = UNSOLVEDMARKER;
		}
	}
	for (int ss = 0; ss < SUDOKUSIZE; ss++) { //ss means sectionsquare; rotates between individual squares in a section
		int current = sudoku[sudokusectiondimension(section(location), ss)];
		if (current != UNSOLVEDMARKER) {
			remaining[current - 1] = UNSOLVEDMARKER;
		}
	}
}

void emptyGarbage(milesArray& sudoku) {
	for (int i = 0; i < SUDOKUVOLUME; i++){
		if (!inrange(sudoku[i], 1, SUDOKUSIZE)) {
			sudoku[i] = UNSOLVEDMARKER;
		}
	}
}

void countup(milesArray& arr) {
	for (int i = 0; i < arr.length(); i++) {
		arr[i] = i + 1;
	}
}

int findnot(milesArray& arr, int element) {
	for (int i = 0; i < arr.length(); i++) {
		if (arr[i] != element) {
			return i;
		}
	}
	return arr.length();
}

int sudokudimension(int row, int col) {
	return (row * SUDOKUSIZE + col);
}

int sudokusectiondimension(int section, int extrarow, int extracol) {
	return sudokudimension((section / SECTIONCOUNT) * SECTIONSIZE + extrarow, (section % SECTIONCOUNT) * SECTIONSIZE + extracol);
}

int sudokusectiondimension(int section, int extra) {
	return sudokudimension((section / SECTIONCOUNT) * SECTIONSIZE + (extra/SECTIONSIZE), (section % SECTIONCOUNT) * SECTIONSIZE) + (extra % SECTIONSIZE);
}

int row(int location) {
	return location / SUDOKUSIZE;
}

int col(int location) {
	return location % SUDOKUSIZE;
}

int section(int location) {
	return ((row(location) / SECTIONSIZE) * SECTIONCOUNT) + (col(location) / SECTIONSIZE);
}

int extra(int location) {
	return ((row(location) % SECTIONSIZE) * SECTIONSIZE) + (col(location) % SECTIONSIZE);
}