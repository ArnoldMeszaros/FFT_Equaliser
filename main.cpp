// https://codereview.stackexchange.com/questions/226323/radix2-fast-fourier-transform-implemented-in-c
// https://riptutorial.com/algorithm/example/27088/radix-2-fft

//  OVO KORISTIM
//  https://cplusplus.com/forum/general/265589/


#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <cstdint>
#include <stdio.h> 
#include <vector>
#include <iomanip>
#include <complex>

using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::getline;

const double pi = 3.14159265358979323846;
using Complex = complex<double>;

typedef struct  WAV_HEADER
{
	/* RIFF Chunk Descriptor */
	uint8_t         RIFF[4];        // RIFF Header Magic header
	uint32_t        ChunkSize;      // RIFF Chunk Size
	uint8_t         WAVE[4];        // WAVE Header
									/* "fmt" sub-chunk */
	uint8_t         fmt[4];         // FMT header
	uint32_t        Subchunk1Size;  // Size of the fmt chunk
	uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
	uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
	uint32_t        bytesPerSec;    // bytes per second
	uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
	uint16_t        bitsPerSample;  // Number of bits per sample
									/* "data" sub-chunk */
	uint8_t         Subchunk2ID[4]; // "data"  string
	uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

// Function prototypes
int getFileSize(FILE* inFile);
void print(const char * prompt, Complex A[], int log2N);
void FFT(Complex f[], Complex ftilde[], int log2N);
void iFFT(Complex ftilde[], Complex f[], int log2N);
int dataPoints();
//void add_to_array(int *&array, int size, int value)

int main(int argc, char* argv[])
{
	// Command line parameters  - sa testiranjem
	// g++ mainreturn.cpp -o main 
	// ./main test.wav arg2 arg3
	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	wav_hdr wavHeader;
	int headerSize = sizeof(wav_hdr), filelength = 0;

	const char* filePath;
	string input;
	if (argc <= 1)
	{
		cout << "Input wave file name: ";
		cin >> input;
		cin.get();
		//filePath = input.c_str();
		filePath = "test.wav";
	}
	else
	{
		filePath = argv[1];
		cout << "Input wave file name: " << filePath << endl;
	}

	FILE* wavFile = fopen(filePath, "r");
	if (wavFile == nullptr)
	{
		fprintf(stderr, "Unable to open wave file: %s\n", filePath);
		return 1;
	}

	//Read the header
	size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
	cout << '\n' << "Header Read " << bytesRead << " bytes." << endl;

	if (bytesRead > 0)
	{
		//Read the data
		uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;				//Number     of bytes per sample
		cout << "bytesPerSample " << bytesPerSample << " bytes." << endl;	//dodato

		//uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample;		//How many samples are in the wav file?   - bytesPerS = 0 ZASTO?
		static const uint16_t BUFFER_SIZE = 4096;
		int8_t* buffer = new int8_t[BUFFER_SIZE];
		while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
		{
			/*   ** DO SOMETHING WITH THE WAVE DATA HERE **    */
			cout << "Read " << bytesRead << " bytes." << endl;
		}

		delete[] buffer;
		buffer = nullptr;
		filelength = getFileSize(wavFile);

		cout << "File is                    :" << filelength << " bytes." << endl;
		cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
		cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
		cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
		cout << "Data size                  :" << wavHeader.ChunkSize << endl;

		// Display the sampling Rate from the header
		cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
		cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
		cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
		cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
		cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
		cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
		// Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

		cout << "Block align                :" << wavHeader.blockAlign << endl;
		cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;

		cout << '\n';
	}

	//------------------------------------  get proper data from WAV !!!
	//------------------------------------  read data from txt and make array[] !!!
	int countLines = dataPoints();

	// Zero stuffing za sekvence koji imaju razlicit broj clanova od 2^n :    1, 2  ->  1, 2, 0, 0
	//add_to_array(data, D, 0)  - stavlja jednu nulu na mesto D+1. Resiti for petljom da dodaje onoliko 0 koliko treba

	cout << "Number of rows in data.txt: " << countLines << " -- ovo je za debug funkcije fft";
	cout << '\n';

	//int fileLines = 1500000;				//---OVO JE const int N -> iz ovoga sledi log2N -- mozda
	//Ideja je bila, da sve vrednosti wav file iypisem u txt file red po red, prebrojim redove i tako da dobijem broj data tacaka


	//------------------------------------  LINK data.txt to FFT V. link wav file to FFT
	const int N = 16, log2N = 4;			// Hardwired for testing, Values for testing the function - zameniti na dataPoints()
																	
	Complex f[N] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 4.0, 3.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1.0, 2.0 };
	Complex ft[N];		//array for FFTdata
	Complex n[N];		//array for IFFTdata
	
	print("Original:", f, log2N);
	cout << '\n';

	// Forward transform
	FFT(f, ft, log2N);   print("\nTransform:", ft, log2N);

	//freq = np.fft.rfftfreq(len(data), d = 1. / rate)
	

	//------------------------------------  Filter FFT_data
	
	// Generate an array,
	// which contains the frequencies where the gaussian function will be generated
	// it contains 10 elements
	int gauss_freq[10] = { 31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
	
	// Set preset defined in argument
	//amplitude = presets.set_presets(preset_arg)

	// Calculate the gaussian function
	int g = 0;
	double gaussian_function = 0;
	while (g < 10)
	{
		//gaussian_function += amplitude[i] * np.exp(-np.square(freq - gauss_freq[i]) / (2 * (gauss_freq[i] / 3)**2));
		g = g + 1;
	}

	//apply gaussian filter
	//FFT_data += FFT_data  * gaussian_function;

	//------------------------------------  Check FFT[].cpp == FFT_data.txt
	// Ovo samo debug


	//------------------------------------  ifft
	//iFFT(ft, f, log2N);   print("\nInvert:", f, log2N);
	iFFT(ft, n, log2N);   print("\nInvert:", n, log2N);
	//------------------------------------  Check ifft[].cpp == newdata.txt
	// Ovo isto ne treba ako su spojeni wav vrednosti sa IFFT
	// Isipis IFFT u wav


	cout << '\n';
	fclose(wavFile);
	system("pause");

	return 0;
}


// Find the file size
int getFileSize(FILE* inFile)
{
	int fileSize = 0;
	fseek(inFile, 0, SEEK_END);

	fileSize = ftell(inFile);

	fseek(inFile, 0, SEEK_SET);
	return fileSize;
}

// Ispis
void print(const char * prompt, Complex A[], int log2N)
{
	int N = 1 << log2N;
	cout << prompt << '\n' << fixed;
	for (int i = 0; i < N; i++) cout << A[i] << '\n';
}

// Fast Fourier Transform
void FFT(Complex f[], Complex ftilde[], int log2N)                 
{
	int N = 1 << log2N;

	// Reorder
	for (int i = 0; i < N; i++)
	{
		int ii = 0, x = i;
		for (int j = 0; j < log2N; j++)
		{
			ii <<= 1;
			ii |= (x & 1);
			x >>= 1;
		}
		ftilde[ii] = f[i];
	}

	for (int s = 1; s <= log2N; s++)
	{
		int m = 1 << s;
		int m2 = m / 2;
		Complex w = 1.0;
		Complex wm = polar(1.0, -pi / m2);
		for (int j = 0; j < m2; j++)
		{
			for (int k = j; k < N; k += m)
			{
				Complex t = w * ftilde[k + m2];
				Complex u = ftilde[k];
				ftilde[k] = u + t;
				ftilde[k + m2] = u - t;
			}
			w *= wm;
		}
	}
}

// Inverse Fast Fourier Transform
void iFFT(Complex ftilde[], Complex f[], int log2N)                
{
	int N = 1 << log2N;

	for (int m = 0; m < N; m++) ftilde[m] = conj(ftilde[m]);      // Apply conjugate (reversed below)

	FFT(ftilde, f, log2N);

	double factor = 1.0 / N;
	for (int m = 0; m < N; m++) f[m] = conj(f[m]) * factor;

	for (int m = 0; m < N; m++) ftilde[m] = conj(ftilde[m]);      // Only necessary to reinstate ftilde
}

//	Count the lines in a file ( counting time points)
int dataPoints()
{
	FILE* fp;
	int countLines = 1;
	int i;
	int columns = 0;
	fp = fopen("data.txt", "r");
	if (fp == NULL)
	{
		cout << "Cant open file";
		fclose(fp);
		return 1;
	}
	else
	{
		while ((i = fgetc(fp)) != EOF)
		{
			if (i == '\n')
			{
				countLines++;
			}
		}

	}
	return countLines;
}

// Zero stuffing  - treba da prima vrednost N, da racuna razliku 2^n - N i to da doda na kraj novog reda
void add_to_array(int *&array, int size, int value) // add & to make "array" a reference
{
	int *newArr = new int[size + 1];
	memcpy(newArr, array, size * sizeof(int));
	delete[] array;
	array = newArr;
	array[size] = value;
} // Popraviti ovu funkciju
/*
// WAV open, read, get data
double wavRead()
{

}
*/