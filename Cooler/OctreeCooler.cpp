#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "lodepng.h"

#include "OctreeCooler.h"

using namespace std;

Cooler::Cooler(string path)
{
	
	ifstream file(path);

	if (file) {

		switch (getType(file)) {

		case FileType::BMP:
			loadBMP(file);
			cout << "Loading from BMP" << endl;
			break;

		case FileType::PNG:
			loadPNG(file);
			cout << "Loading from PNG" << endl;
			break;

		case FileType::COOLER:
			loadCooler(file);
			break;

		case FileType::UNSUPPORTED:
			cout << "Error: Unsupported file format!!!" << endl;
			break;

		default:
			break;

		}

	}
	else {

		cout << "Error in file!!!" << endl;

	}

	file.close();
}
Cooler::Cooler(ifstream& file)
{

	if (file) {



	}
}


void Cooler::loadBMP(ifstream & file)
{
}

void Cooler::loadPNG(string path)
{

	ifstream file; // Am Ende beginnen, um Länge der Datei zu ermitteln

	file.exceptions(ifstream::failbit | ifstream::badbit);

	try { // Versuche, Datei zu öffnen, bei Fehler Informationen ausgeben
		file.open(path, ios::in | ios::binary);
	}
	catch (system_error& e) {
		cerr << e.code().message() << endl;
	}

	loadPNG(file);

	file.close();

}
void Cooler::loadPNG(ifstream & file)
{

	if (file.good()) {

		file.seekg(0, ios::end);
		int filesize = file.tellg();
		file.seekg(0, ios::beg);

		cout << "Filesize: " << filesize << endl;

		vector<uint8_t> rawfile(filesize);

		cout << file.read((char*)rawfile.data(), filesize).fail() << endl;

		//if (file.read((char*)rawfile.data(), filesize)) {

		//	vector<uint8_t> image; //the raw pixels

		//	unsigned width, height;

		//	//decode with lodepng | Thanks!!!
		//	unsigned error = lodepng::decode(image, width, height, rawfile);
		//	if (error) std::cout << "lodepngerror " << error << ": " << lodepng_error_text(error) << std::endl;

		//	cout << width << " x " << height;

		//}
		//else {

		//	cout << "Error: File could not be read!!!" <<endl;

		//}
	}
	else {

		cout << "Error: Something wrong with ifstream!!!" << endl;

	}

}

void Cooler::loadCooler(ifstream & file)
{
}

Cooler::FileType Cooler::getType(ifstream & file)
{

	if (isPNG(file))
		return FileType::PNG;

	else if (isBMP(file))
		return FileType::BMP;

	else if (isCooler(file))
		return FileType::COOLER;

	else
		return FileType::UNSUPPORTED;
	
}

bool Cooler::isPNG(ifstream & file)
{

	if (!file.is_open())
		return false;

	file.seekg(1);
	char signature_raw[4] = { '\0', '\0', '\0', '\0' };

	if (!file.read(signature_raw, 3)) //Die Bytes 1, 2 und 3 sollten "PNG" enthalten
		cout << (int)file.is_open();

	string signature(signature_raw);

	return signature == "PNG";
	
}
bool Cooler::isPNG(string path)
{
	ifstream file(path);

	bool ispng = isPNG(file);

	file.close();

	return ispng;
}

bool Cooler::isBMP(ifstream & file)
{
	if (!file.is_open())
		return false;

	file.seekg(0);
	char signature_raw[3] = { '\0', '\0', '\0' };

	if (!file.read(signature_raw, 2)) //Die Bytes 1, 2 und 3 sollten "BM" enthalten
		return false;

	string signature(signature_raw);

	return signature == "BM";
}
bool Cooler::isBMP(string path)
{
	ifstream file(path);

	bool ispng = isBMP(file);

	file.close();

	return ispng;
}

bool Cooler::isCooler(string path)
{
	return false;
}
bool Cooler::isCooler(ifstream & file)
{
	return false;
}
