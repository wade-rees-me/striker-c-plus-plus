#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <string>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "card.hpp"
#include "json.hpp"
#include "constants.hpp"

const static std::string BET = "bet";
const static std::string INSURANCE = "insurance";
const static std::string SURRENDER = "surrender";
const static std::string DOUBLE = "double";
const static std::string SPLIT = "split";
const static std::string STAND = "stand";
const static std::string PLAY = "play";

//
class Strategy {
  public:
	Strategy(std::string p, int n);
	~Strategy();

  public:
	int getBet(const int *seenCards) {
		json_object = nullptr;
		return parseAuxInt(httpGet(urlBet, buildParams(seenCards, nullptr, nullptr, nullptr)), BET, 2);
	}
	bool getInsurance(const int *seenCards) {
		return parseAuxBool(httpGet(urlInsurance, buildParams(seenCards, nullptr, nullptr, nullptr)), INSURANCE, false);
	}
	bool getSurrender(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, SURRENDER, false);
		}
		return parseAuxBool(httpGet(urlSurrender, buildParams(seenCards, haveCards, nullptr, up)), SURRENDER, false);
	}
	bool getDouble(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, DOUBLE, false);
		}
		return parseAuxBool(httpGet(urlDouble, buildParams(seenCards, haveCards, nullptr, up)), DOUBLE, false);
	}
	bool getSplit(const int *seenCards, Card* pair, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, SPLIT, false);
		}
		return parseAuxBool(httpGet(urlSplit, buildParams(seenCards, nullptr, pair, up)), SPLIT, false);
	}
	bool getStand(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, STAND, false);
		}
		return parseAuxBool(httpGet(urlStand, buildParams(seenCards, haveCards, nullptr, up)), STAND, true);
	}
	void doPlay(const int *seenCards, const int *haveCards, Card* pair, Card* up);
	void clear() {
		json_object = nullptr;
	}

  private:
	CURL *curl_handle;
	cJSON* json_object;
	std::string urlBet = "http://" + getStrategyUrl() + "/bet";
	std::string urlInsurance = "http://" + getStrategyUrl() + "/" + INSURANCE;
	std::string urlSurrender = "http://" + getStrategyUrl() + "/" + SURRENDER;
	std::string urlDouble = "http://" + getStrategyUrl() + "/" + DOUBLE;
	std::string urlSplit = "http://" + getStrategyUrl() + "/" + SPLIT;
	std::string urlStand = "http://" + getStrategyUrl() + "/" + STAND;
	std::string urlPlay = "http://" + getStrategyUrl() + "/" + PLAY;
	std::string playbook;
	int numberOfCards;

  private:
	std::string buildParams(const int* seenData, const int* haveData, Card* pair, Card* up);
	cJSON* httpGet(std::string& url, std::string params);
};

#endif // STRATEGY_HPP
