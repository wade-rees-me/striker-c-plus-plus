#ifndef RANK_HPP
#define RANK_HPP

#include <string>
#include <cstdint>

//
class Rank {
  public:
	Rank(const std::string& rank, int value, int offset)
		: rank(rank), value(value), offset(offset) {}

  public:
	std::string getRank() const {
		return rank;
	}
	int getValue() const {
		return value;
	}
	int getOffset() const {
		return offset;
	}

  private:
	std::string rank;
	int value;
	int offset;
};

#endif // RANK_HPP
