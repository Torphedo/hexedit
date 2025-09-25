#include "ArgParser/ArgParser.h"
#include "ArgParser/ArgParseException.h"

#include <string>
#include <vector>
#include <algorithm>
#include <utility>      //for std::pair

ArgParser::ArgParser() : finalized(false) {
}

ArgParser::ArgParser(const int argc, char *argv[],
    const std::string switches, const std::string dataOptions)
    : allSwitches(switches), allDataOpts(dataOptions) {

    for(int i = 0; i < argc; ++i)
        args.push_back(argv[i]);

    commonIter = args.begin();
    finalized = true;
}

void ArgParser::operator()(const int argc, char *argv[],
    const std::string switches, const std::string dataOptions) {
        if(!finalized) {
            for(int i = 0; i < argc; ++i)
                args.push_back(argv[i]);

            allSwitches = switches;
            allDataOpts = dataOptions;

            commonIter = args.begin();
            finalized = true;
        }
    }

void ArgParser::parse() {
    ++commonIter;       //first arg is the program name, skip it
    std::string arg;

    while(commonIter != args.end()) {
        arg = *commonIter;
        if(arg[0] != '-')               //matches 'op', 'f.txt' etc.
            processOperand();
        else if(arg.length() == 1)      //matches (only) '-'
            processOperand();
        else if(arg[1] != '-')          //mathes '-c', '-dRe' etc.
            processSingleDash();
        else if(arg.length() == 2)      //matches (only) '--'
            processOperand();
        else                            //matches '--reedirect', '--file=name' etc
            processDoubleDash();
    }
}

bool ArgParser::isSwitchSet(const std::string svitch) const {
    return std::find(switchesSet.begin(), switchesSet.end(), svitch) != switchesSet.end();
}

bool ArgParser::isDataOptSet(const std::string dataOption) const {
    for(std::pair<std::string, std::string> p : dataOptsSet)
        if(p.first == dataOption)
            return true;
        
    return false;
}

bool ArgParser::isSet(const std::string option) const {
    return isSwitchSet(option) || isDataOptSet(option);
}

std::string ArgParser::getDataForOpt(const std::string option) const {
    for(std::pair<std::string, std::string> p : dataOptsSet)
        if(p.first == option)
            return p.second;

    throw ArgParseException("No such data option set: " + option);
}

std::vector<std::string> ArgParser::getSwitches() const {
    return switchesSet;
}

std::vector<std::string> ArgParser::getDataOptions() const {
    std::vector<std::string> dataOpts;

    for(std::pair<std::string, std::string> p : dataOptsSet)
        dataOpts.push_back(p.first);

    return dataOpts;
}

std::vector<std::string> ArgParser::getAllOptions() const {
    std::vector<std::string> options;

    //copy all switches
    std::copy(switchesSet.begin(), switchesSet.end(), std::back_inserter(options));

    //copy all data options(only options, not their data)
    for(std::pair<std::string, std::string> p : dataOptsSet)
        options.push_back(p.first);

    return options;
}

std::vector<std::string> ArgParser::getOperands() const {
    return operands;
}
