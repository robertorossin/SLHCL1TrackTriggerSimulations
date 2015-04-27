#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

static const unsigned MAX_NTOWERS = 6 * 8;

unsigned int decodeLayer( unsigned int moduleId) {
   return moduleId / 10000;
}

unsigned int decodeLadder( unsigned int moduleId) {
   return (moduleId / 100) % 100;
}

unsigned int decodeModule( unsigned int moduleId) {
   return moduleId % 100;
}

int getTriggerTowerMap(TString src, std::map<unsigned, std::vector<unsigned> >& ttmap) {
	if (!src.EndsWith(".csv")) {
		std::cout << "Input filename must be .csv" << std::endl;
		return 1;
	}
	std::vector<unsigned> values;
	unsigned j = 0;
	std::string line, issline;
	std::ifstream ifs(src.Data());
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);
		while (std::getline(iss, issline, ',')) { // split by commas
			if (j == 0) continue; // skip the first line
				values.push_back(std::stoi(issline));
		}
		++j;
	}
	if (values.empty()) {
		std::cout << "Failed to read any trigger tower" << std::endl;
		return 1;
	}
	ttmap.clear();
	unsigned ttId = 0;
	for (unsigned i=0; i<values.size(); ++i) {
		if (i == 0) continue; // skip the first index
			if (values.at(i-1) <= 6 && values.at(i) <= 8) { // eta_idx, phi_idx
				ttId = (values.at(i-1)-1) * 8 + (values.at(i)-1); // mapped to 0-47
				ttmap.insert(std::make_pair(ttId, std::vector<unsigned>()));
			} else if (values.at(i) > 10000) {
				ttmap.at(ttId).push_back(values.at(i));
			}
	}
	return 0;
}

int readTriggerTowerFile(TString src, std::map<unsigned, std::vector<unsigned> >& triggerTowerMap_, std::map<unsigned, std::vector<unsigned> >& triggerTowerReverseMap_) {
	if (getTriggerTowerMap(src, triggerTowerMap_)) {
		std::cout << "Failed to parse the trigger tower csv file." << std::endl;
		return 1;
	}
	assert(triggerTowerMap_.size() == MAX_NTOWERS);
	// Reverse trigger tower map
	triggerTowerReverseMap_.clear();
	for (unsigned i=0; i<MAX_NTOWERS; i++) { // loop over input trigger towers
		const std::vector<unsigned>& moduleIds = triggerTowerMap_[i];
		std::cout << "Tower " << i << " has " << moduleIds.size() << " modules." << std::endl;
		for (auto it2: moduleIds) { // loop over the moduleIds in the tower
			triggerTowerReverseMap_[it2].push_back(i);
		}
	}
	//for (auto it: triggerTowerReverseMap_) std::cout << "Module " << it.first << " is in " << it.second.size() << " towers." << std::endl;
	return 0;
}
