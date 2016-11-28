#include <iostream>

// c++ read and write library
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

void CreateFile(char fileName[]){
    
    char data[100];
    
    ofstream outfile;
    outfile.open(fileName);
    
    cout << "Writing to the file" << endl;
    cout << "Enter your name: ";
    cin.getline(data, 100);
    
    // write inputted data into the file.
    outfile << data << endl;
    
    cout << "Enter your age: ";
    cin >> data;
    
    //ignore() enter a basmadan programın ilerlemini durdurur.
    cin.ignore();
    
    // again write inputted data into the file.
    outfile << data << endl;
    
    // close the opened file.
    outfile.close();
    
}

void ReadFile(){
    
    char data[100];
    // open a file in read mode.
    ifstream infile;
    infile.open("input.txt");
    
    cout << "Reading from the file" << endl;
    infile >> data;
    
    // write the data at the screen.
    cout << data << endl;
    
    // again read the data from the file and display it.
    infile >> data;
    cout << data << endl;
    
    // close the opened file.
    infile.close();
}

int** SetAlignmentArrayLimits(int **alignmentArray, int s2Length, int s1Length){
    alignmentArray = new int*[s2Length + 2];
    for (int i = 0; i < s2Length + 2; i++) {
        alignmentArray[i] = new int[s1Length + 2];
    }
    return alignmentArray;
}

void SetZeros(int **alignmentArray, int i, int j){
    alignmentArray[i][j] = 0;
    cout << alignmentArray[i][j];
}

void SetAlignmentValues(int **alignmentArray, int i, int j, int match, int missMatch, int gap, string s1, string s2){
    int score = 0;
    if (s1.at(j - 1) == s2.at(i - 1))
        score = alignmentArray[i - 1][ j - 1] + (match);
    else
        score = alignmentArray[i - 1][ j - 1] + (missMatch);
    
    int scroeLeft = alignmentArray[i][ j - 1] + (gap);
    int scoreUp = alignmentArray[i - 1][ j] + (gap);
    
    int maxScore = max(max(score, scroeLeft), scoreUp);
    
    alignmentArray[i][j] = maxScore;
    cout << alignmentArray[i][j];
}

void SetAlignmentArrayValues(int s1Length, int s2Length, int** alignmentArray, string s1, string s2, int match, int missMatch, int gap){
    for (int i = 0; i < (s2Length + 2); i++) {
        for (int j = 0; j < (s1Length + 2); j++) {
            if (i == 0) {
                SetZeros(alignmentArray, i, j);
            }
            else if (i != 0 && j == 0){
                SetZeros(alignmentArray, i, j);
            }
            else if(i >= 1 && j >= 1){
                SetAlignmentValues(alignmentArray, i, j, match, missMatch, gap, s1, s2);
            }
        }
        cout << endl;
    }
}

void TracebackAlgorithm(int** alignmentArray, string s1, string s2, int s1Length, int s2Length){
    char s2Array[1024];
    char s1Array[1024];
    strcpy(s2Array, s2.c_str());
    strcpy(s1Array, s1.c_str());
    
    string AlignmentA = "";
    string AlignmentB = "";
    int m = s2Length + 1;
    int n = s1Length + 1;
    while (m > 0 && n > 0)
    {
        int scroeDiag = 0;
        
        //Remembering that the scoring scheme is +2 for a match, -1 for a mismatch and -2 for a gap
        if (s2Array[m - 1] == s1Array[n - 1])
            scroeDiag = 2;
        else
            scroeDiag = -1;
        
        if (m > 0 && n > 0 && alignmentArray[m][ n] == alignmentArray[m - 1][ n - 1] + scroeDiag)
        {
            AlignmentA = s1Array[n - 1] + AlignmentA;
            AlignmentB = s2Array[m - 1] + AlignmentB;
            m = m - 1;
            n = n - 1;
        }
        else if (n > 0 && alignmentArray[m][ n] == alignmentArray[m][ n - 1] - 2)
        {
            AlignmentA = s1Array[n - 1] + AlignmentA;
            AlignmentB = "-" + AlignmentB;
            n = n - 1;
        }
        else //if (m > 0 && scoringMatrix[m, n] == scoringMatrix[m - 1, n] + -2)
        {
            AlignmentA = "-" + AlignmentA;
            AlignmentB = s2Array[m - 1] + AlignmentB;
            m = m - 1;
        }
    }
    cout << AlignmentA << endl << AlignmentB << endl;
}

int main(int argc, const char * argv[]) {
    
    
    string s1, s2;
    
    cout << "S1 => ";
    getline(cin, s1);
    
    cout << "S2 => ";
    getline(cin, s2);
    int **alignmentArray;
    int s1Length = (int)s1.length() - 1;
    int s2Length = (int)s2.length() - 1;
    
    int match, missMatch, gap;
    
    match = 2;
    missMatch = -1;
    gap = -2;
    
    
    //AlignmentArray Limit Settings
    alignmentArray = SetAlignmentArrayLimits(alignmentArray, s1Length, s2Length);
    
    //AlignmentArray Value Settings
    SetAlignmentArrayValues(s1Length, s2Length, alignmentArray, s1, s2, match, missMatch, gap);
    
    //Traceback Step
    TracebackAlgorithm(alignmentArray, s1, s2, s1Length, s2Length);
    
    
    return 0;
    
    
}
