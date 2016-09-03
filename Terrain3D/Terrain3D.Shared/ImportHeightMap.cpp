#include "pch.h"
#include "Game.h"
#include <fstream>

void CGame::Import()
{
	int fixedsize = 512 * 512;
	std::vector<unsigned char> in(fixedsize);
	
	std::string File = "mountain.raw";
	//std::string File = "coastal.raw";
	std::ifstream InFile(File, std::ios_base::binary);
	if (InFile.is_open())
	{
		dbgRAW = "SUCCESS";
		InFile.read((char*)&in[0], (std::streamsize)in.size());
		InFile.close();
	}

	float heightfactor = 200;
	for (int i = 0; i < 512 * 512; i++)
	{
		tempHM[i] = (float)in[i] * heightfactor / 256 ;
	}
		


}