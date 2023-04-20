#include "LvlGrabber.h"
#include <filesystem>
#include <direct.h>
#include <fstream>
#include <string>
#include "fw.h"

LvlGrabber::LvlGrabber() {
	this->mp = std::map<dpp::snowflake, std::map<dpp::snowflake, UserStat>>();
}

void LvlGrabber::read() {
	for (const auto& f : std::filesystem::directory_iterator()) {
		std::string fname = f.path().filename().string();

		fname = fname.substr(0, fname.length()-4);

		this->mp[(dpp::snowflake)std::stoi(fname)] = std::map<dpp::snowflake, UserStat>();
		std::map<std::string, std::string> set = fw::read(fname.c_str());

		for (std::pair<std::string, std::string> pr : set) {
			dpp::snowflake id = (dpp::snowflake)std::stoi(pr.first);
			this->mp[(dpp::snowflake)std::stoi(fname)][id] = leveling::proc(id, pr.second);
		}
	}
}

void LvlGrabber::upt() {
	for (const auto& upr : this->mp) {
		std::string fname = std::to_string(upr.first) + DIR_LVL_GUILD_EXTENSION;
		std::map<std::string, std::string> set = std::map<std::string, std::string>();

		for (const auto& pr : this->mp[upr.first]) {
			set[std::to_string(pr.first)] = leveling::comp(pr.second);
		}

		fw::upt(fname.c_str(), set);
	}
}