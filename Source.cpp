#include <Windows.h>
#include <iostream>
#include <fstream> //for file io
#include <string>
#include <vector>

using namespace std;


std::string current_working_directory()
{
	char working_directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(working_directory), working_directory); // **** win32 specific ****
	return working_directory;
}

//re-encode bytes list as ANSI string and write to file
void writeFile(const vector<int>& newByteList, int numBytesToChange, int iterations, string fileExtension)
{
	ofstream fout;
	//string path = "frame" + to_string(fileNum) + "_bytesChanged_=" + to_string(numBytesToChange) + "_iterations_=" + to_string(iterations) + "." + fileExtension;
	string path = "_bytesChanged_=" + to_string(numBytesToChange) + "_iterations_=" + to_string(iterations) + "." + fileExtension;
	fout.open(path, ios::out | ios::binary);

	std::basic_string<uint8_t> bytes;

	// Iterate over every byte value in the vector
	for (size_t i = 0; i < newByteList.size(); i++)
	{
		// Add the current byte value to 8-bit string byte array
		bytes.push_back(static_cast<uint8_t>(newByteList.at(i)));
	}

	// we are now generating a string obj from our bytes array
	// this string object contains the binary byte values of new image in ANSI
	std::string result(begin(bytes), end(bytes));

	// output string object containing the binary byte values directly into a binary file
	fout << result;
	fout.close();
}

//insert a new chunk of data at a random position in the byte list
vector<int> insertBytes(vector<int>& newByteList, const int bytesToChange)
{
	int pos = (rand() % newByteList.size());
	vector<int> chunk;
	for (int i = 0; i < bytesToChange; ++i)
	{
		chunk.push_back(rand() % 255);
	}
	//replace chunks at position pos with new chunk of random data
	newByteList.erase(newByteList.begin() + pos, newByteList.begin() + pos + bytesToChange);
	newByteList.insert(newByteList.begin() + pos, chunk.begin(), chunk.end());
	return newByteList;
}

void alterByteCode(const vector<int>& originalByteList, const int iterations, const int numBytesToChange, string fileExtension)
{
		vector<int>newByteList = originalByteList;
		for (int j = 0; j < iterations; ++j)
		{
			newByteList = insertBytes(newByteList, numBytesToChange);
		}
		writeFile(newByteList, numBytesToChange, iterations, fileExtension);
}

void bytesizeFile()
{
	//ifstream fin("/...", ios::in); = read
	//ofstream fout ("/...", ios::out); = write

	string fileName;
	int iterations = 1;
	int bytesToChange = 3;

	cout << "\n(image must be located in image folder)\n\ninput file name (inc extension) .... :";
	cin >> fileName;

	//images/2statue.png

	//vector<int> byteList;

	std::ifstream fin;
	//fin.open(fileName, ios::in | ios::binary); // input file
	fin.open("images/" + fileName, ios::in | ios::binary); // input file
	if (!fin.fail())
	{
		//extract file type
		size_t i = fileName.rfind('.', fileName.length());
		string fileExtension;
		if (i != string::npos) {
			fileExtension = fileName.substr(i + 1, fileName.length() - i);
		}

		cout << "\n(recommended between 5-100)\n\nnumber of random iterations .... : ";
		cin >> iterations;
		cout << "\n(recommended between 2-50)\n\nnumber of bytes to alter per iteration .... : ";
		cin >> bytesToChange;

		vector<int> byteList;

		unsigned int bit;
		while ((bit = fin.get()) != EOF)
		{
			byteList.push_back(bit);
		}

		alterByteCode(byteList, iterations, bytesToChange, fileExtension);
		
		//use for outputting multiple images at a time
		/*int no_of_images = 20;
		for (int i(0); i < no_of_images; i++)
		{
			iterations = rand() % 10 + 1;
			bytesToChange = rand() % 10 + 1;
			alterByteCode(byteList, iterations, bytesToChange, fileExtension);
		}*/

		fin.close();
	}
	else
	{
		char Path[MAX_PATH];
		cout << "\n\nCANT FIND FILE \"" + fileName + "\" AT CURRENT DIRECTORY ";
		cout << current_working_directory();
	}
}

int main()
{
	// Use current time as seed for random generator
	srand(time(0));

	bytesizeFile();

	cout << "\n\n\n\n\n";
	system("pause");
	return 0;
}