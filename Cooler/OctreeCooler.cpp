#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "lodepng.h"

#include "OctreeCooler.h"

using namespace std;

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a){}
bool Color::operator==(Color & c)
{
	return (this->R == c.R && this->G == c.G && this->B == c.B && this->A == c.A);
}
ostream& operator << (ostream& os, Color& c)
{
	os << "(" << (int)c.R << "|" << (int)c.G << "|" << (int)c.B << "|" << (int)c.A << ")";
	return os;

}

Cooler::Cooler(string path)
{

	ifstream file(path, ios::in | ios::binary); // Am Ende beginnen, um Länge der Datei zu ermitteln

	if (file.good()) {
		switch (getType(file)) {

		case FileType::BMP:
			cout << "Loading from BMP" << endl;
			loadBMP(file);
			break;

		case FileType::PNG:
			cout << "Loading from PNG" << endl;
			loadPNG(file);
			
			break;

		case FileType::COOLER:
			cout << "Loading from Coolerfile" << endl;
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
	if (file.good()) {
		switch (getType(file)) {

		case FileType::BMP:
			cout << "Loading from BMP" << endl;
			loadBMP(file);
			break;

		case FileType::PNG:
			cout << "Loading from PNG" << endl;
			loadPNG(file);

			break;

		case FileType::COOLER:
			cout << "Loading from Coolerfile" << endl;
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
}

void Cooler::loadPNG(string path)
{

	ifstream file(path, ios::in | ios::binary);

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

		if (file.read((char*)rawfile.data(), filesize)) {

			vector<uint8_t> byteimage; //the raw pixels
			vector<Color> image;

			unsigned width, height;

			//decode with lodepng | Thanks!!!
			unsigned error = lodepng::decode(byteimage, width, height, rawfile);
			if (error) std::cout << "lodepngerror " << error << ": " << lodepng_error_text(error) << std::endl;

			cout << width << " x " << height << " Pixel" << endl;

			if (byteimage.size() % sizeof(Color) != 0)
				cout << "Invalid byte count for image data: " << byteimage.size() << endl;

			for(int i = 0; i < byteimage.size() / sizeof(Color); i++){
			
				uint8_t r = byteimage.at(sizeof(Color) * i + 0);
				uint8_t g = byteimage.at(sizeof(Color) * i + 1);
				uint8_t b = byteimage.at(sizeof(Color) * i + 2);
				uint8_t a = byteimage.at(sizeof(Color) * i + 3);

				Color pixel(r, g, b, a);

				if (!(pixel == Color(0, 0, 0, 255) || pixel == Color(255, 0, 0, 255) || pixel == Color(0, 0, 255, 255)))
					cout << "Error: Wrong color in cooler png: " << i << " : " << pixel << endl;

				image.push_back(pixel);

			}

			load(image, width, height);

		}
		else {

			cout << "Error: File could not be read!!!" <<endl;

		}
	}
	else {

		cout << "Error: File.good() returned false!!!" << endl;

	}

}

void Cooler::loadCooler(ifstream & file)
{
}

Material Cooler::decode(Color pixel)
{
	if (pixel == Color(0, 0, 0, 0))
		return Material::COOLER;

	else if (pixel == Color(255, 0, 0, 255))
		return Material::AMMONIA;

	else if (pixel == Color(0, 0, 255, 255))
		return Material::ALMIGHTY_CHAN_THE_COOLEST;
	else											//Shouldn't happen
		return Material::ALMIGHTY_CHAN_THE_COOLEST;
}

void Cooler::load(vector<Color>& image, int width, int height)
{
	data = vector<vector<Material>>(height);

	for (int i = 0; i < height; i++) {

		vector<Material> row(width);

		for (int j = 0; j < width; j++)
			row.push_back(decode(image.at(width * i + j)));

		data.push_back(row);

	}

	cout << "Successfully loaded cooler..." << endl;

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

//void Cooler::loadBMP(ifstream & file)
//{
//
//	if (file.good()) {
//
//		file.seekg(0, ios::end);
//		int filesize = file.tellg();
//		file.seekg(0, ios::beg);
//
//		cout << "Filesize: " << filesize << endl;
//
//		vector<uint8_t> rawfile(filesize);
//
//		if (file.read((char*)rawfile.data(), filesize)) {	//Vielleicht irgendwann mal ne struct BMPHeader schreiben?
//
//			unsigned width, height, dataoffset;
//
//			width = *(reinterpret_cast<unsigned*>(&(rawfile.data()[18])));	// Vier bytes ab Offset 18
//			height = *(reinterpret_cast<unsigned*>(&(rawfile.data()[22])));	// Vier Bytes ab Offset 22
//			dataoffset = *(reinterpret_cast<unsigned*>(&(rawfile.data()[10]))); // Vier Bytes ab Offset 10
//
//			cout << width << " x " << height << " Pixel" << endl; //RGB, kein A
//
//			vector<uint8_t> byteimage(filesize - dataoffset); //the raw pixels, without header
//			vector<Color> image;
//
//			copy(rawfile.begin() + dataoffset, rawfile.end(), byteimage.begin());
//
//			if (byteimage.size() != width * height * sizeof(Color))
//				cout << "Invalid byte count for image data: " << byteimage.size() << endl;
//
//			for (int i = 0; i < byteimage.size() / sizeof(Color); i++) {
//
//				uint8_t r = byteimage.at(3 * i + 0);
//				uint8_t g = byteimage.at(3 * i + 1);
//				uint8_t b = byteimage.at(3 * i + 2);
//				uint8_t a = byteimage.at(3 * i + 3); //Sie haben schrecklicherweise doch ein alphabyte
//
//				Color pixel(r, g, b, a);
//
//				if (!(pixel == Color(0, 0, 0, 255) || pixel == Color(255, 0, 0, 255) || pixel == Color(0, 0, 255, 255)))
//					cout << "Error: Wrong color in cooler png: " << i << " : " << pixel << endl;
//
//				image.push_back(pixel);
//
//			}
//
//			load(image, width, height);
//
//		}
//		else {
//
//			cout << "Error: File could not be read!!!" << endl;
//
//		}
//	}
//	else {
//
//		cout << "Error: File.good() returned false!!!" << endl;
//
//	}
//}
//bool Cooler::isBMP(ifstream & file)
//{
//	if (!file.is_open())
//		return false;
//
//	file.seekg(0);
//	char signature_raw[3] = { '\0', '\0', '\0' };
//
//	if (!file.read(signature_raw, 2)) //Die Bytes 1, 2 und 3 sollten "BM" enthalten
//		return false;
//
//	string signature(signature_raw);
//
//	return signature == "BM";
//}
//bool Cooler::isBMP(string path)
//{
//	ifstream file(path);
//
//	bool ispng = isBMP(file);
//
//	file.close();
//
//	return ispng;
//}

bool Cooler::isCooler(string path)
{
	return false;
}
bool Cooler::isCooler(ifstream & file)
{
	return false;
}


