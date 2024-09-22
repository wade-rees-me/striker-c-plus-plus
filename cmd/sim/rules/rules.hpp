#ifndef RULES_HPP
#define RULES_HPP

#include <string>

class Rules {
  public:
	char playbook[128];
	bool hit_soft_17 = true;
	bool surrender = false;
	bool double_any_two_cards = true;
	bool double_after_split = false;
	bool resplit_aces = false;
	bool hit_split_aces = false;
	int blackjack_pays = 3;
	int blackjack_bets = 2;
	float penetration = 0.75;

  public:
	void rulesLoadTable(const std::string& decks);
	void print(int indent);

  private:
	void rulesFetchTable(const std::string& url);
};

#endif // RULES_HPP
