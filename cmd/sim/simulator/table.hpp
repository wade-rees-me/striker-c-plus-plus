#ifndef TABLE_HPP
#define TABLE_HPP

#include <cstdint>
#include "parameters.hpp"
#include "rules.hpp"
#include "player.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "report.hpp"

class Table {
public:
	Table(Parameters* params, Rules* rules);
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
	void status(int64_t round, int64_t hand);
};

#endif // TABLE_HPP
