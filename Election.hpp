#ifndef ELECTION_H
#define ELECTION_H
#include "ElectoralMap.hpp"
#include <vector>

struct Candidate{
    Party partyType;
    int id;
    std::string name;
    int numVotes;

    bool operator==(Candidate const &c) const {
        return this->id==c.id && this->name==c.name;
    }
};

class TextUI{
    public:
        TextUI();                                           // constructor, adds all menu statements to main_menu_
        int routeChoice();                                  // gets the users choice of direct, representative, or quit
        int registerCandidate(Party p, std::string &name);  // asks user to register candidates for the inputted party
    private:
        std::vector<std::string> main_menu_;
};

class Election{
    public:
        Election(std::vector<Candidate> c);                     // Constructor, creates an election with a vector of candidates
        void campaign();                                        // function that allows Candidates to campaign in a district
        void votes();                                           // updates the numVotes for each Candidate
        void getWinner();                                       // Finds the candidate with the most votes
        std::vector<Candidate> candidates_;
        ElectoralMap &em_ = ElectoralMap::GetInstance();        
};

// RepresentativeElection inherits public Election because it is an Election object, however,
// the votes are counted differently
class RepresentativeElection:public Election{
    public:
        RepresentativeElection(std::vector<Candidate> c):Election(c){}  //constructor, same as Election class
        void votes();                                               // updates the numVotes for each Candidate
};

#endif