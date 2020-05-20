#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
//#define FILENAME "input.txt"
#define FILENAME "testCase.txt"
//#define FILENAME "testCase_orig.txt"
#define INT_MIN -9999999
#define MAX_ARR_SIZE 100

class MatData
{
	public:
		std::string peakName;
        int row, col;
        int **data;

        int dx[4] = {0, 1, -1, 0};
        int dy[4] = {1, 0, 0, -1};

        //Within bounds
        bool isValid(int x, int y){return ((x >= 0) && (x < row)) && ((y >= 0)  && (y < col));}
		//Default Constructor 
		MatData()
		{
			row = 0;
			col = 0;
		}
		//Parametric Contructor
		MatData(uint16_t r, uint16_t c): row(r), col(c)
	{}

        int longestDecSequence();
        int getDecreasingSequence(int, int);
};

int MatData::longestDecSequence()
{
    int max = 1;
    int val = 1;

    //Go through all the values
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
            val = getDecreasingSequence(i, j);
            if(val > max)
                max = val;
        }
    }
    return max;
}

int MatData::getDecreasingSequence(int x, int y)
{
    int max = 1;
    int retVal = 1;
    for(int i = 0; i < 4; i++)
    {
        if(isValid(x + dx[i], y + dy[i]) && data[x][y] > data[x + dx[i]][y + dy[i]])
        {
            retVal = getDecreasingSequence(x + dx[i], y + dy[i]) + 1;
        }
        if(retVal > max)
            max = retVal;
    }
    return max;
}

class ReadDataFile
{
	public:
		int numOfSamples;
        static std::vector<MatData> m_data;
		ReadDataFile(std::string fileName)
		{
			std::ifstream file(fileName);
			if(file.is_open())
			{
				fromFileToObject(file);
			}
		}
	private:
		void fromFileToObject(std::ifstream&);
        void readPeakDimensions(std::string &line, MatData &matData);
        void readAllPeaks(std::string &line, MatData &matData, int row);

};

void ReadDataFile::fromFileToObject(std::ifstream& file)
{

    std::string line;
    std::getline(file, line);
    numOfSamples = std::stoi(line);
    //std::cout << " Num of Samples : " << numOfSamples << std::endl;

    for(int sample = 0; sample < numOfSamples; ++sample)
    {
        std::getline(file, line);
        //Read new Sample
        MatData matdata;
        matdata.row = 0;
        matdata.col = 0;
        matdata.data = nullptr;
        readPeakDimensions(line, matdata);

        //Allocate Memory here
        matdata.data = new int*[matdata.row];
        for(int rowNum = 0; rowNum < matdata.row; ++rowNum)
            matdata.data[rowNum] = new int[matdata.col];

        //Fill the data
        for(int row = 0; row < matdata.row; ++row)
        {
            std::getline(file, line);
            //Now store data
            readAllPeaks(line, matdata, row);
        }
        m_data.push_back(matdata);
    }
}

void ReadDataFile::readPeakDimensions(std::string &aLine, MatData &matData)
{
    std::string line = aLine;
    std::string delimiter = " ";
    std::vector<std::string> token;
    size_t start;
    size_t end = 0;

    while ((start = line.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = line.find(delimiter, start);
        token.push_back(line.substr(start, end - start));
    }

    //Store peak dimensions and name
    matData.peakName =  token[0];
    matData.row = stoi(token[1]);
    matData.col = stoi(token[2]);
}

void ReadDataFile::readAllPeaks(std::string &aLine, MatData &matData, int curRow)
{
    std::string line = aLine;
    std::string delimiter = " ";
    std::vector<std::string> token;
    size_t start;
    size_t end = 0;
    while ((start = line.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = line.find(delimiter, start);
        token.push_back((line.substr(start, end - start)));
    }
    for(int i = 0; i <matData.col; ++i)
        matData.data[curRow][i] = std::stoi(token[i]);
}


std::vector<MatData> ReadDataFile::m_data;

int main()
{
	ReadDataFile readFile(FILENAME);

    for(auto &elem : ReadDataFile::m_data)
    {
        std::cout << "" << elem.peakName
                  << ": "
                  << elem.longestDecSequence()
                  << std::endl;
    }
}
