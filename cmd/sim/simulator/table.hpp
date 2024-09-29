#ifndef TABLE_HPP
#define TABLE_HPP

#include <cstdint>
#include "player.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "report.hpp"

class Table {
public:
	Table(Parameters* params);
	~Table();

	void session(bool mimic);
	Card* dealCards(Hand* hand);
	void show(Card* card);

//private:
	Parameters* parameters;
	Shoe* shoe;
	Dealer* dealer;
	Player* player;
	Report report;

private:
	void status(int64_t round);
};

#endif // TABLE_HPP
