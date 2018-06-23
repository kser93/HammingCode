#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

void print_description(unsigned r)
{
	auto m{ 1 << (r - 1) };
	std::vector<unsigned> word_bits(m);
	std::vector<unsigned> control_bits(r);
	std::vector<unsigned> encoded_bits(m + r);

	std::cout << "m = " << m << std::endl;
	std::cout << "r = log2(m)+1 = " << r << std::endl;
	std::cout << "n = m + r = " << m + r << std::endl;

	std::iota(std::begin(encoded_bits), std::end(encoded_bits), 1);
	for (auto i{ 0 }; i < r; ++i)
	{
		control_bits[i] = 1 << i;
	}
	std::set_difference(
		std::begin(encoded_bits), std::end(encoded_bits),
		std::begin(control_bits), std::end(control_bits),
		std::begin(word_bits)
	);
	std::cout << "Data bits:";
	for (const auto &bit : word_bits)
		std::cout << ' ' << bit << ',';
	std::cout << "\b; ";
	std::cout << "Control bits:";
	for (const auto &bit : control_bits)
		std::cout << ' ' << bit << ',';
	std::cout << "\b " << std::endl;

	for (const auto &bit_c : control_bits)
	{
		std::cout << "Bit " << bit_c << " controls bits:";
		for (const auto &bit_w : word_bits)
		{
			if (bit_c & bit_w)
			{
				std::cout << ' ' << bit_w << ',';
			}
		}
		std::cout << "\b " << std::endl;
	}
}

int main()
{
	std::cout << "Hamming code illustration" << std::endl;
	std::random_device rd;
	std::mt19937 gen(rd());

	for (auto r{ 3 }; r < 10; ++r)
	{
		for (auto k = 0; k < 100; ++k)
		{
			auto m{ 1 << (r - 1) };
			//print_description(r);
			std::vector<bool> word(m + r);
			for (auto &bit : word)
			{
				bit = std::bernoulli_distribution(0.5)(gen);
			}
			for (auto index{ 1 }; index < word.size(); index <<= 1)
			{
				word[index - 1] = false;
				for (auto i{ 0 }; i < word.size(); ++i)
				{
					if ((index & (i + 1)) && (index != (i + 1)))
					{
						word[index - 1] = word[index - 1] ^ word[i];
					}
				}
			}
			std::vector<bool> original_word{ word };
			auto errbit{ std::uniform_int_distribution<>(1, m + r)(gen) };
			word[errbit - 1] = !word[errbit - 1];

			std::vector<bool> control_word{ word };

			errbit = 0; // 0 is no result
			for (auto index{ 1 }; index < control_word.size(); index <<= 1)
			{
				control_word[index - 1] = false;
				for (auto i{ 0 }; i < control_word.size(); ++i)
				{
					if ((index & (i + 1)) && (index != (i + 1)))
					{
						control_word[index - 1] = control_word[index - 1] ^ control_word[i];
					}
				}
				if (control_word[index - 1] != word[index - 1])
				{
					errbit += index;
				}
			}
			word[errbit - 1] = !word[errbit - 1];
			if (
				std::equal(
					std::begin(original_word), std::end(original_word),
					std::begin(word), std::end(word))
				)
				std::cout << "Pass" << std::endl;
			else
				std::cout << "Fail" << std::endl;

		}
	}
}