#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;



int main()
{
	ifstream of("BOX2.ase");
	string ignore;

	if (!of.is_open())
	{
		return 0;
	}
	
	vector<istringstream> p;
	while (!of.eof())
	{
		getline(of, ignore);
		istringstream st(ignore);
		p.push_back(std::move(st));
	}

	of.close();
	
	for (size_t i = 0; i < p.size(); ++i)
	{
		string ign;
		getline(p[i], ign);
		int offset = (int)ign.find("*SCENE", 0);
		if (offset > 0)
		{
			int a = 5;
		}
	}

	return 0;
}