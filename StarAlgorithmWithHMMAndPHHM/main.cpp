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
    }
}

int Difference(string alignmentA, string alignmentB){
    int score = 0;
    for (int i = 0; i < alignmentA.length(); i++) {
        if (alignmentA.at(i) != alignmentB.at(i)) {
            score++;
        }
    }
    
    return score;
}

void TracebackAlgorithm(int** alignmentArray, string s1, string s2, int s1Length, int s2Length, string array[5], int x, int y, bool isChanged, int resultArray[5][5], bool isSet){
    char s2Array[1024];
    char s1Array[1024];
    strcpy(s2Array, s2.c_str());
    strcpy(s1Array, s1.c_str());
    
    string AlignmentA = "";
    string AlignmentB = "";
    int m = s2Length + 1;
    int n = s1Length + 1;
    int newS1L = 0;
    int newS2L = 0;
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
    
    int result = Difference(AlignmentA, AlignmentB);
    
    newS1L = (int)AlignmentA.length() - 1;
    newS2L = (int)AlignmentB.length() - 1;
    
    if(newS1L > s1Length){
        
        for (int i = x + 1; i < y; i++) {
            int sIndex = (int)AlignmentA.find('-');
            string x = "-";
            array[i].insert(sIndex, x);
//            cout << array[i] << endl;
        }
        
    }
    
    if (!isChanged) {
        array[x] = AlignmentA;
        array[y] = AlignmentB;
    }
    else {
        array[x] = AlignmentB;
        array[y] = AlignmentA;
    }
    if (resultArray[0][0] == 0 ) {
        isSet = true;
    }
    if (!isSet){
        for (int t = 0; t < 5; t++) {
            for (int z = 0; z < 5; z++) {
                if (t == x && z == y) {
                    resultArray[t][z] = result;
                }
                else {
                    resultArray[t][z] = 0;
                }
            }
        }
    }
    else {
        resultArray[x][y] = result;
    }
    
//    cout << AlignmentA << endl << AlignmentB << endl << result << endl << resultArray[x][y];
}



void StarSolution(int **alignmentArray, int s1Length, int s2Length, int match, int missMatch, int gap, string s1, string s2, string array[5], int x, int y, bool isChanged, int resultArray[5][5], bool isSet){
    
    if(s2Length > s1Length){
        string stringTemp = s1;
        int intTemp = s1Length;
        s1 = s2;
        s2 = stringTemp;
        s1Length = s2Length;
        s2Length = intTemp;
        isChanged = true;
    }
    else{
        isChanged = false;
    }
    
    //AlignmentArray Limit Settings
    alignmentArray = SetAlignmentArrayLimits(alignmentArray, s1Length, s2Length);
    
    //AlignmentArray Value Settings
    SetAlignmentArrayValues(s1Length, s2Length, alignmentArray, s1, s2, match, missMatch, gap);
    
    //Traceback Step
    TracebackAlgorithm(alignmentArray, s1, s2, s1Length, s2Length, array, x, y, isChanged, resultArray, isSet);
}

int MTotals(int resultArray[5][5], int l){
    int m = 0;
    for (int k = 0; k < 5; k++) {
        m = m + resultArray[l][k];
    }
    return m;
}

int main(int argc, const char * argv[]) {
    
    int **alignmentArray;
    int match, missMatch, gap;
    bool isChanged = false;
    int resultArray[5][5];
    bool isSet = false;
    
    match = 2;
    missMatch = -1;
    gap = -2;

    string starArray[5];
    
    starArray[0] = "CCTGCTGCAG";
    starArray[1] = "GATGTGCCG";
    starArray[2] = "GATGTGCAG";
    starArray[3] = "CCGCTAGCAG";
    starArray[4] = "CCTGTAGG";
    
//    cout << "S1 => ";
//    getline(cin, starArray[0]);
//    
//    cout << "S2 => ";
//    getline(cin, starArray[1]);
//    
//    cout << "S3 => ";
//    getline(cin, starArray[2]);
//    
//    cout << "S4 => ";
//    getline(cin, starArray[3]);
//    
//    cout << "S5 => ";
//    getline(cin, starArray[4]);
    
    int s1Length = 0;
    int s2Length = 0;
    
//    s1Length = (int)starArray[0].length() -1;
//    s2Length = (int)starArray[1].length() -1;
//    
//    StarSolution(alignmentArray, s1Length, s2Length, match, missMatch, gap, starArray[0], starArray[1], starArray, 0, 1, isChanged);
    
    
    for (int i = 0; i < 5; i++) {
        for (int j = i+1; j < 5; j++) {
            
            s1Length = (int)starArray[i].length() -1;
            s2Length = (int)starArray[j].length() -1;
            StarSolution(alignmentArray, s1Length, s2Length, match, missMatch, gap, starArray[i], starArray[j], starArray, i, j, isChanged, resultArray, isSet);
//            cout << endl;
        }
    }
    
    for (int k = 0; k < 5; k++) {
        for (int l = 0; l < 5 ; l++) {
            resultArray[l][k] = resultArray[k][l];
        }
    }
    
    int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0;
    
    m1 = MTotals(resultArray, 0);
    m2 = MTotals(resultArray, 1);
    m3 = MTotals(resultArray, 2);
    m4 = MTotals(resultArray, 3);
    m5 = MTotals(resultArray, 4);
    
    int less = min(m1, min(m2, min(m3, min(m4, m5))));

    for (int k = 0; k < 5; k++){
        cout << starArray[k] << endl;
    }
    
    //DATA SET
    // CCTGCTGCAG
    // GATGTGCCG
    // GATGTGCAG
    // CCGCTAGCAG
    // CCTGTAGG
    
    //CC-GCTAGCAG
    //GATG-TGCCG
    return 0;
    
    
}
