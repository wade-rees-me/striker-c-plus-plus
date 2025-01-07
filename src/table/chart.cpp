#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <cctype>
#include "chart.hpp"
#include "constants.hpp"

//
Chart::Chart(const std::string& name)
		: name(name) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		ChartRow *row = &rows[i];
		row->key = "--";
		for (int j = MINIMUM_CARD_VALUE; j <= MAXIMUM_CARD_VALUE; j++) {
			row->value[j] = "---";
		}
	}
}

//
int Chart::getRowCount() {
	return nextRow;
}

//
void Chart::insert(const std::string& key, int up, const std::string& value) {
	int index = getRow(key);
	if (index < 0) {
		index = nextRow++;
		rows[index].key.assign(key);
		toUpperCase(rows[index].key);
	}
	rows[index].value[up].assign(value);
	toUpperCase(rows[index].value[up]);
}

//
const std::string Chart::getValue(std::string& key, int up) {
	int index = getRow(key);
	if (index < 0) {
		printf("Cannot find value in %s for %s vs %d\n", name.c_str(), key.c_str(), up);
		exit(-1);
	}
	return rows[index].value[up];
}

//
const std::string Chart::getValue(int total, int up) {
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	std::string key = buffer;
	return getValue(key, up);
}

void Chart::print() {
	printf("%s\n", name.c_str());
	printf("--------------------2-----3-----4-----5-----6-----7-----8-----9-----X-----A---\n");
	for (int i = 0; i < nextRow; i++) {
		ChartRow *row = &rows[i];
		printf("%2s : ", row->key.c_str());
		for (int j = 0; j <= MAXIMUM_CARD_VALUE; j++) {
			printf("%4s, ", row->value[j].c_str());
		}
		printf("\n");
	}
	printf("------------------------------------------------------------------------------\n\n");
}

//
int Chart::getRow(const std::string& key) {
	for (int i = 0; i < nextRow; i++) {
		ChartRow *row = &rows[i];
		std::string keyLower(key);
		toUpperCase(keyLower);
		if(strcmp(row->key.c_str(), keyLower.c_str()) == 0) {
			return i;
		}
	}
	return -1;
}

