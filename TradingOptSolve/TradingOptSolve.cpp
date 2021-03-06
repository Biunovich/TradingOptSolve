// TradingOptSolve.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#define AMOUNT 1000

enum Action {
	BUY,
	SELL,
	WAIT
};

double solveTask(std::vector<double>* vecPrice, std::vector<Action>* vecActions) {
	double amount_dollars = AMOUNT;
	double amount_crypto = 0;
	double prev_amount_dollars = AMOUNT;
	Action last_action = Action::SELL;
	for (int i = 0; i < vecPrice->size(); i++) {
		switch (vecActions->at(i))
		{
		case WAIT:
			break;
		case BUY:
			if (last_action == Action::BUY)
				break;
			amount_crypto = amount_dollars / vecPrice->at(i);
			prev_amount_dollars = amount_dollars;
			amount_dollars = 0;
			last_action = Action::BUY;
			break;
		case SELL:
			if (last_action == Action::SELL)
				break;
			amount_dollars = amount_crypto * vecPrice->at(i);
			amount_crypto = 0;
			last_action = Action::SELL;
			break;
		default:
			std::cout << "Wrong action" << std::endl;
			break;
		} 
	}
	return amount_dollars ? amount_dollars : prev_amount_dollars;
}

void permutRecursive(std::vector<Action>* vecActions, int size) {
	if (size == -1)
		return;
	vecActions->at(size) = (Action)((vecActions->at(size) + 1) % 3);
	if (vecActions->at(size) == Action::WAIT) {
		permutRecursive(vecActions, size - 1);
	}
}

int main()
{
	std::ifstream file("USDT_ETH.txt");
	std::string line;
	std::vector<double> vecPrice;
	double volume, price, maxValue = 0;
	int number = 0;
	while (std::getline(file, line)) {
		std::string date, time;
		std::istringstream istream(line);
		istream >> date >> time >> volume >> price;
		vecPrice.push_back(price);
		number++;
		if (number > 10)
			break;
	}

	std::vector<Action> vecActions(vecPrice.size(), Action::WAIT);
	std::vector<Action> vecMaxActions;
	for (int i = 0; i < pow(3, vecPrice.size()); i++) {
		double temp = solveTask(&vecPrice, &vecActions);
		permutRecursive(&vecActions, vecActions.size() - 1);
		if (maxValue < temp) {
			maxValue = temp;
			vecMaxActions = vecActions;
		}
	}
	for (auto it = vecMaxActions.begin(); it != vecMaxActions.end(); it++)
		std::cout << *it;
	std::cout << " - Best solution" << std::endl;
	std::cout << "Max profit: " << maxValue << std::endl;
	std::getchar();
    return 0;
}

