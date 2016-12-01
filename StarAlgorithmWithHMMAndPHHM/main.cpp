#include <iostream>

// c++ read and write library
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

void CreateFile(char fileName[], string* data, int length){
    
    ofstream outfile;
    outfile.open(fileName);
    
    cout << "Writing to the file" << endl;
    
    for (int i  = 0; i < length; i++) {
        // write inputted data into the file.
        outfile << data[i] << endl;
        
    }
    
    // close the opened file.
    outfile.close();
    
}

string* SetStarArrayLimits(string* starArray, int length){
    starArray = new string[length];
    return starArray;
}

string* ReadFile(string* lineArray, int matchArray[3]){
    
    // open a file in read mode.
    ifstream infile;
    infile.open("input.txt");
    int lineCount = 0;
    
    string str;
    string file_contents;
    while (std::getline(infile, str))
    {
        file_contents += str;
        file_contents.push_back('*');
        lineCount ++;
    }
    
    lineArray = SetStarArrayLimits(lineArray, lineCount);
    
    string delimiter = "*";
    size_t pos = 0;
    int i = 0;
    while ((pos = file_contents.find(delimiter)) != std::string::npos) {
        
        lineArray[i] = file_contents.substr(0, pos);
        file_contents.erase(0, pos + delimiter.length());
        i++;
    }
    string del = ",";
    while ((pos = lineArray[0].find(del)) != std::string::npos) {
        if (matchArray[0] == 0) {
            matchArray[0] = stoi(lineArray[0].substr(0, pos).c_str());
            lineArray[0].erase(0, pos + del.length());
        }
        else if(matchArray[1] == 0){
            matchArray[1] = stoi(lineArray[0].substr(0, pos));
            lineArray[0].erase(0, pos + del.length());
        }
        
    }
    matchArray[2] = stoi(lineArray[0].substr(0, pos));
    lineArray[0].erase(0, pos + del.length());
    
    for (int k = 1; k < lineCount; k ++) {
        file_contents = lineArray[k];
        lineArray[k] = "";
        while ((pos = file_contents.find(del)) != std::string::npos) {
            
            lineArray[k] += file_contents.substr(0, pos);
            file_contents.erase(0, pos + del.length());
        }
        lineArray[k] += file_contents.substr(0, pos);
        lineArray[k-1] = lineArray[k];
    }
    infile.close();
    return lineArray;
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

void TracebackAlgorithm(int** alignmentArray, string s1, string s2, int s1Length, int s2Length, string array[5], int x, int y, bool isChanged, int** resultArray, bool isSet, int stringCount){
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
        for (int t = 0; t < stringCount; t++) {
            for (int z = 0; z < stringCount; z++) {
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
    
}

void StarSolution(int **alignmentArray, int s1Length, int s2Length, int match, int missMatch, int gap, string s1, string s2, string array[5], int x, int y, bool isChanged, int** resultArray, bool isSet, int stringCount){
    
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
    TracebackAlgorithm(alignmentArray, s1, s2, s1Length, s2Length, array, x, y, isChanged, resultArray, isSet, stringCount);
}

int MTotals(int** resultArray, int l, int stringCount){
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
    int** resultArray;
    bool isSet = false;
    int stringCount = 0;
    
    match = 0;
    missMatch = 0;
    gap = 0;
    
    string* starArray;
    stringCount = 5;
    
    starArray = SetStarArrayLimits(starArray, stringCount);
    
    int matchArray[3] = {match, missMatch, gap};
    
    starArray = ReadFile(starArray, matchArray);
    for (int p = 0; p < 3 ; p++) {
        if (p == 0) {
            match = matchArray[p];
        }
        else if(p == 1){
            missMatch = matchArray[p];
        }
        else if(p == 2){
            gap = matchArray[p];
        }
    }
    resultArray = SetAlignmentArrayLimits(resultArray, stringCount, stringCount);
    
    
    int s1Length = 0;
    int s2Length = 0;
    
    
    
    for (int i = 0; i < stringCount; i++) {
        for (int j = i+1; j < stringCount; j++) {
            
            s1Length = (int)starArray[i].length() -1;
            s2Length = (int)starArray[j].length() -1;
            StarSolution(alignmentArray, s1Length, s2Length, match, missMatch, gap, starArray[i], starArray[j], starArray, i, j, isChanged, resultArray, isSet, stringCount);
        }
    }
    
    for (int k = 0; k < stringCount; k++) {
        for (int l = 0; l < stringCount ; l++) {
            resultArray[l][k] = resultArray[k][l];
        }
    }
    
    int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0;
    
    m1 = MTotals(resultArray, 0, stringCount);
    m2 = MTotals(resultArray, 1, stringCount);
    m3 = MTotals(resultArray, 2, stringCount);
    m4 = MTotals(resultArray, 3, stringCount);
    m5 = MTotals(resultArray, 4, stringCount);
    
    int less = min(m1, min(m2, min(m3, min(m4, m5))));
    
    CreateFile("msa.txt", starArray, stringCount);
    
    return 0;
    
    
}
