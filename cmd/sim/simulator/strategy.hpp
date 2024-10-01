#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <string>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "card.hpp"
#include "json.hpp"
#include "constants.hpp"

//
class Strategy {
  public:
	Strategy(std::string p, int n);
	~Strategy();

  public:
	int getBet(const int *seenCards) {
		json_object = nullptr;
		return parseAuxInt(httpGet(urlBet, buildParams(seenCards, nullptr, nullptr, nullptr)), "bet", 2);
	}
	bool getInsurance(const int *seenCards) {
		return parseAuxBool(httpGet(urlInsurance, buildParams(seenCards, nullptr, nullptr, nullptr)), "insurance", false);
	}
	bool getSurrender(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, "surrender", false);
		}
		return parseAuxBool(httpGet(urlSurrender, buildParams(seenCards, haveCards, nullptr, up)), "surrender", false);
	}
	bool getDouble(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, "double", false);
		}
		return parseAuxBool(httpGet(urlDouble, buildParams(seenCards, haveCards, nullptr, up)), "double", false);
	}
	bool getSplit(const int *seenCards, Card* pair, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, "split", false);
		}
		return parseAuxBool(httpGet(urlSplit, buildParams(seenCards, nullptr, pair, up)), "split", false);
	}
	bool getStand(const int *seenCards, const int *haveCards, Card* up) {
		if(json_object != nullptr) {
			return parseAuxBool(json_object, "stand", false);
		}
		return parseAuxBool(httpGet(urlStand, buildParams(seenCards, haveCards, nullptr, up)), "stand", true);
	}
	void doPlay(const int *seenCards, const int *haveCards, Card* pair, Card* up);
	void clear() {
		json_object = nullptr;
	}

  private:
	CURL *curl_handle;
	cJSON* json_object;
	std::string urlBet = "http://" + getStrategyUrl() + "/bet";
	std::string urlInsurance = "http://" + getStrategyUrl() + "/insurance";
	std::string urlSurrender = "http://" + getStrategyUrl() + "/surrender";
	std::string urlDouble = "http://" + getStrategyUrl() + "/double";
	std::string urlSplit = "http://" + getStrategyUrl() + "/split";
	std::string urlStand = "http://" + getStrategyUrl() + "/stand";
	std::string urlPlay = "http://" + getStrategyUrl() + "/play";
	std::string playbook;
	int numberOfCards;

  private:
	std::string buildParams(const int* seenData, const int* haveData, Card* pair, Card* up);
	cJSON* httpGet(std::string& url, std::string params);
};

#endif // STRATEGY_HPP
