#include <iostream>

int main() {
	unsigned int seed;

	std::cout << "Type the seed: ";
	std::cin >> seed;

	srand(seed);

	std::cout << "This generator will now generate 10 random numbers" << std::endl;

	for (int i = 0; i < 10; i++) {
		std::cout << rand() << std::endl;
	}

	return EXIT_SUCCESS;
}
