#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

enum class Material { AMMONIA, COOLER, ALMIGHTY_CHAN_THE_COOLEST };
struct Color
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0);
	bool operator== (Color& c);

};

class Cooler
{

	enum class FileType{BMP, PNG, COOLER, UNSUPPORTED};

	string path;
	vector<vector<Material>> data;

public:

	Cooler(string path);
	Cooler(ifstream & filestream); //lodepng unterstützt keine ifstreams...

	vector<vector<Material>>& getData();

	//void loadBMP(string path);
	void loadPNG(string path);

	//void loadBMP(ifstream & file);
	void loadPNG(ifstream & file);
	void loadCooler(ifstream & file);

	Material decode(Color pixel);
	void load(vector<Color>& image, int width, int height);

	FileType getType(ifstream & file);

	static bool isPNG(string path);
	static bool isPNG(ifstream & file);

	/*bool isBMP(string path);
	bool isBMP(ifstream & file);*/

	bool isCooler(string path);
	bool isCooler(ifstream & file);

};