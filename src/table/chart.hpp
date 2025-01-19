#ifndef CHART_HPP
#define CHART_HPP

#include <iostream>
#include <string>
#include "shoe.hpp"

#define TABLE_SIZE 21

class Chart {
	class ChartRow {
		public:
			std::string key;
			std::string value[MAXIMUM_CARD_VALUE + 1];
	};

	public:
		Chart(const std::string& name);

	private:
   		ChartRow rows[TABLE_SIZE];
		std::string name;
		int nextRow = 0;

	public:
		int getRowCount();
		void insert(const std::string& key, int up, const std::string& value);
		const std::string getValue(std::string& key, int up);
		const std::string getValue(int total, int up);
		void print();

	private:
		int getRow(const std::string& key);
};

#endif // CHART_HPP
