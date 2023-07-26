#include <map>
#include <string>
#include <iostream>

int main()
{
	std::map<int, std::string>	myMap;

	for (int i = 0; i < 10; i++)
		myMap[i] = "ms cuteness" + std::to_string(i);
	std::cout << myMap[4] << std::endl;
	return 0;

}