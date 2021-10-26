#include "Election.hpp"
#include <algorithm>
#include <math.h>

/**
 * constructor, adds all menu statements to main_menu_
 * @param none
 * @return none
*/
TextUI::TextUI(){
    main_menu_.push_back("What kind of election should we have (direct or representative, 0 to stop)?");
}

/**
 * gets the users choice of direct, representative, or quit
 * @param none
 * @return int, returns 1 for direct, 2 for representative, and 0 for quit
*/
int TextUI::routeChoice(){
    std::cout<<main_menu_[0]<<" ";
    std::string input;
    getline(std::cin,input);
    if(input=="direct"){
        return 1;
    } else if(input=="representative"){
        return 2;
    } else {
        return 0;
    }
}

/**
 * asks user to register candidates for the inputted party
 * @param Party p- the party where the candidate is being asked to be registered, string& name- return parameter that stores the value of the name the user chooses for the candidate
 * @return int, returns 1 for candidate registered, returns 0 if the user chooses not to register a candidate for the inputted party
*/
int TextUI::registerCandidate(Party p,std::string &name){
    std::cout<<"Do you want to register a candidate for party "<<PartyStringify(p)<<" (y or n)? ";
    std::string input;
    getline(std::cin,input);
    if(input=="y"){
        input="";
        std::cout<<"What is their name? ";
        getline(std::cin,input);
        name=input;
        return 1;
    } else {
        return 0;
    }

}

/**
 * Constructor, creates an election with a vector of candidates
 * @param vector<Candidate> c- sets the candidates, candidates_ ,of the election to this vector
 * @return none
*/
Election::Election(std::vector<Candidate> c){
    candidates_=c;
}

/**
 * This function asks the user to pick a candidate to campaign
 * If the user selects a candidate, the user is then asked which district to campaign the candidate
 * Two probabilities are calculated for the chosen district:
 * The probability of converting a constituent from Party::None and the probability of converting
 * a constituent from the majority Party that is not the candidate's party and is not Party::None
 * Depending on these probabilities, constituents are converted and the ElectoralMap is updated
 * This function loops until the user chooses to no longer campaign any candidates in any district
 * @param none
 * @return none
*/
void Election::campaign(){
    int x=-1;
    while(x!=0){                    // loops until the user does not want to campaign any more candidates
        x=-1;
        std::string input="";


        std::cout<<em_<<std::endl;                      // outputs each candidate and their respective id and Party
        std::cout<<"----------"<<std::endl;
        for(int i=0;i<candidates_.size();i++){
            std::cout<<candidates_[i].id<<": "<<candidates_[i].name<<" [Party: "<<PartyStringify(candidates_[i].partyType)<<"]"<<std::endl;
        }
        std::cout<<std::endl;

        std::cout<<"Which candidate is campaigning (id) (0 to stop) ? ";
        getline(std::cin,input);
        x=stoi(input);
        if(x!=0){
            Candidate can = candidates_[x-1];
            int y=-1;
            std::cout<<em_<<std::endl;
            while(y!=0){                                // loops until the user chooses to stop campaigning the chosen candidate
                y=-1;
                input="";
                std::cout<<"Where is this candidate campaigning (id) (0 to stop) ? ";
                getline(std::cin,input);
                y=stoi(input);
                if(y!=0){
                    std::cout<<can.name<<" is campaigning in district "<<y<<std::endl;

                    District d = em_.get_district(y);

                    int canConsts = d.parties_.find(can.partyType)->second;
                    int otherConsts=0;
                    int majorityConsts = 0;
                    Party majorityParty;

                    std::map<Party,int>::iterator it;                       // this finds the majority party as well as the number of constituents not
                    for(it=d.parties_.begin();it!=d.parties_.end();it++){   // in the chosen candidate's party and not in Party::None
                        if(it->first!=can.partyType&&it->first!=Party::None){
                            otherConsts+=it->second;
                        }
                        if(it->first!=can.partyType){
                            if(it->second >= majorityConsts){
                                majorityConsts = it->second;
                                majorityParty = it->first;
                            }
                        }
                    }

                    int size = d.getSize();

                    double prob;
                    double prob_success;
                    double prob_extra;
                    if(otherConsts!=0){                                     // calculates the probabilities                          
                        prob = (((canConsts+1.0)*2.0)/otherConsts)*(((canConsts+1.0)*2.0)/size);
                        prob_success = std::min(100.0,prob);
                        prob_extra = prob_success*0.1;
                    } else {
                        prob=0;
                        prob_success=0;
                        prob_extra=0;
                    }

                    std::cout<<"Chances to convert: "<<prob_success<<std::endl;
                    std::cout<<"Chances to convert from another party: "<<prob_extra<<std::endl;

                    // Scenarios chosen based on calculated probabilities
                    if(prob_success&&!prob_extra){              // Scenario 1
                        if(d.moveConstituents(Party::None,can.partyType)!=0){
                            std::cout<<"Congrats, you have converted someone from none to "<<PartyStringify(can.partyType)<<"!"<<std::endl;
                        }
                    } else if(prob_success&&prob_extra){ 
                        if(d.parties_.find(Party::None)->second==0){ // Scenario 3
                            std::cout<<"Sorry, no one in none to convert!"<<std::endl;
                            if(d.moveConstituents(majorityParty,can.partyType)!=0){
                                std::cout<<"Congrats, you have converted someone from "<<PartyStringify(majorityParty)<<" to "<<PartyStringify(can.partyType)<<"!"<<std::endl;
                            }
                        } else {                                // Scenario 2
                            if(d.moveConstituents(Party::None,can.partyType)!=0){
                                std::cout<<"Congrats, you have converted someone from none to "<<PartyStringify(can.partyType)<<"!"<<std::endl;
                            }
                            if(majorityParty!=Party::None){
                                if(d.moveConstituents(majorityParty,can.partyType)!=0){
                                    std::cout<<"Congrats, you have converted someone from "<<PartyStringify(majorityParty)<<" to "<<PartyStringify(can.partyType)<<"!"<<std::endl;
                                }
                            }
                        }
                    } else if(!prob_success){}             // Scenario 4

                    em_.electoralMap_.erase(y); 
                    em_.electoralMap_.insert(std::make_pair(y,d));      // updates the ElectoralMap

                    std::cout<<em_<<std::endl;
                }
            }
        }
    }
}

/**
 * This function goes through each Party and finds the Party that is not Party::None
 * and has the most constituents
 * @param map<Party,int> d- map of Parties inside a district
 * @return Party, returns the Party type of the majority party in the inputted District's map
*/
Party findMajority(std::map<Party,int> d){
    std::map<Party,int>::iterator it;
    int max=0;
    Party majority;
    for(it=d.begin();it!=d.end();it++){
        if(it->second>=max&&it->first!=Party::None){
            max = it->second;
            majority=it->first;
        }
    }
    return majority;
}

/**
 * This fuction calculates the number of votes each candidate gets in a direct election
 * First, each Party and a vector of candidates are put into a map to account for if there
 * are multiple candidates per Party
 * Each district is iterated through, and in each district, each Party is iterated through
 * Depending on the current Party in the loop, constituents vote for candidates
 * The function then outputs how many votes each candidate got in each district and then
 * it outputs how many total votes each candidate got
 * @param none
 * @return none
*/
void Election::votes(){
    std::map<Party,std::vector<Candidate>> sorted_candidates;
    std::vector<Candidate> alikeParty;
    for(int i=0;i<party_vector.size();i++){                     // sorts candidates based on their affiliated Party
        alikeParty.clear();
        for(int j=0;j<candidates_.size();j++){
            if(candidates_[j].partyType==party_vector[i]){
                alikeParty.push_back(candidates_[j]);
            }
        }
        sorted_candidates.insert(std::make_pair(party_vector[i],alikeParty));
    }

    std::map<int,District>::iterator district_iterator;
    std::map<int,std::map<std::string,int>> district_vote_tracker;
    if(candidates_.size()!=0){
        // for each district in the electoral map
        for(district_iterator=em_.electoralMap_.begin();district_iterator!=em_.electoralMap_.end();district_iterator++){
            std::map<Party,int>::iterator party_iterator=district_iterator->second.parties_.begin();
            Party majority = findMajority(district_iterator->second.parties_);
            std::map<std::string,int> current_district_candidate_votes;
            // for each party in the current district
            for(;party_iterator!=district_iterator->second.parties_.end();party_iterator++){
                std::map<Party,std::vector<Candidate>>::iterator sorted_iterator;
                // if the current party is Party::None
                if(party_iterator->first==Party::None){
                    sorted_iterator = sorted_candidates.find(majority);
                    std::vector<Candidate> current_party = sorted_iterator->second;
                    // if there are no candidates in the majority party
                    if(current_party.size()==0){
                        // for each constituent in None a random candidate is chosen
                        for(int i=0;i<party_iterator->second;i++){
                            int randomIndex = rand() % candidates_.size();
                            candidates_[randomIndex].numVotes++;                // increments the candidate's number of votes
                            std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[randomIndex].name);
                            if(it!=current_district_candidate_votes.end()){     // this tracks how many votes each candidate gets in each district
                                it->second+=1;
                            } else {
                                current_district_candidate_votes[candidates_[randomIndex].name] = 1;
                            }
                        }
                    } else {
                        // for each constituent in None a random candidate from the majority party is chosen
                        for(int i=0;i<party_iterator->second;i++){
                            int randomIndex = rand() % current_party.size();
                            // the random candidate is found in the candidates_ vector and its number of votes is increased
                            for(int j=0;j<candidates_.size();j++){
                                if(candidates_[j]==current_party[randomIndex]){
                                    candidates_[j].numVotes++;                      // increments the candidate's number of votes
                                    std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[j].name);
                                    if(it!=current_district_candidate_votes.end()){ // tracks how many votes each candidate gets in each district
                                        it->second+=1;  
                                    } else {
                                        current_district_candidate_votes[candidates_[j].name] = 1;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    sorted_iterator = sorted_candidates.find(party_iterator->first);
                    std::vector<Candidate> current_party = sorted_iterator->second;
                    // If there is no one aligned with a constituent's party and the constituent is not aligned with Party::None,
                    // they vote as if they were aligned with Party::None.
                    if(current_party.size()==0){      
                        // this section of code is the same as above for constituents affiliated with Party::None   
                        sorted_iterator = sorted_candidates.find(majority);
                        std::vector<Candidate> current_party = sorted_iterator->second;
                        if(current_party.size()==0){
                            // for each constituent in None a random candidate is chosen
                            for(int i=0;i<party_iterator->second;i++){
                                int randomIndex = rand() % candidates_.size();
                                candidates_[randomIndex].numVotes++;
                                std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[randomIndex].name);
                                if(it!=current_district_candidate_votes.end()){
                                    it->second+=1;
                                } else {
                                    current_district_candidate_votes[candidates_[randomIndex].name] = 1;
                                }
                            }
                        } else {
                            // for each constituent in None a random candidate from the majority party is chosen
                            for(int i=0;i<party_iterator->second;i++){
                                int randomIndex = rand() % current_party.size();
                                // the random candidate is found in the candidates_ vector and its number of votes is increased
                                for(int j=0;j<candidates_.size();j++){
                                    if(candidates_[j]==current_party[randomIndex]){
                                        candidates_[j].numVotes++;
                                        std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[j].name);
                                        if(it!=current_district_candidate_votes.end()){
                                            it->second+=1;
                                        } else {
                                            current_district_candidate_votes[candidates_[j].name] = 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        // for each constituent a random candidate from the current party is chosen
                        for(int i=0;i<party_iterator->second;i++){
                            int randomIndex = rand() % current_party.size();
                            // the random candidate is found in the candidates_ vector and its number of votes is increased
                            for(int j=0;j<candidates_.size();j++){
                                if(candidates_[j].partyType==current_party[randomIndex].partyType&&candidates_[j].id==current_party[randomIndex].id){
                                    candidates_[j].numVotes++;                      // increments the candidate's number of votes
                                    std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[j].name);
                                    if(it!=current_district_candidate_votes.end()){ // tracks how many votes the candidate got in the current district
                                        it->second+=1;
                                    } else {
                                        current_district_candidate_votes[candidates_[j].name] = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // this is the map of how many votes each candidate got in each district
            district_vote_tracker.insert(std::make_pair(district_iterator->first,current_district_candidate_votes));    
        }
    }

    std::cout<<std::endl;
    std::map<int,std::map<std::string,int>>::iterator details;
    std::map<std::string,int>::iterator inner_it;
    for(details=district_vote_tracker.begin();details!=district_vote_tracker.end();details++){  // outputs how many votes each candidate got in each district
        std::cout<<"District "<<details->first<<":"<<std::endl;
        for(inner_it=details->second.begin();inner_it!=details->second.end();inner_it++){
            std::cout<<inner_it->first<<": "<<inner_it->second<<std::endl;
        }
    }

    std::cout<<std::endl;

    for(int i=0;i<candidates_.size();i++){                          // outputs how many total votes each candidate got                                  
        std::cout<<candidates_[i].name<<": "<<candidates_[i].numVotes<<" votes"<<std::endl;
    }

    std::cout<<std::endl;

}

/**
 * This function loops through the vector of candidates and outputs a message
 * that indicates which candidate has the most votes and wins the election
 * @param none
 * @return none
*/
void Election::getWinner(){
    std::string winner="";
    int max=0;
    for(int i=0;i<candidates_.size();i++){
        if(candidates_[i].numVotes>=max){
            max=candidates_[i].numVotes;
            winner=candidates_[i].name;
        }
    }
    std::cout<<"Congratulations, "<<winner<<", you've won!"<<std::endl<<std::endl;
}

/**
 * This fuction calculates the number of votes each candidate gets in a representative election
 * First, each Party and a vector of candidates are put into a map to account for if there
 * are multiple candidates per Party
 * The number of total constituents out of all Districts in the ElectoralMap is calculated
 * For each district the majority party with at least one candidate is found
 * The number of votes that a candidate from this party will receive is calculated depending
 * on how many constituents the majority party has, how many total constituents the district has,
 * and how many total constituents are in the ElectoralMap
 * If there are multiple candidates from the majority Party then a random one is chosen to receive
 * all the votes from the current district
 * The number of votes that the candidate that won each district is outputted
 * The number of total votes each candidate got is outputted
 * @param none
 * @return none
*/
void RepresentativeElection::votes(){
    std::map<Party,std::vector<Candidate>> sorted_candidates;
    std::vector<Candidate> alikeParty;
    for(int i=0;i<party_vector.size();i++){                         // sorts candidates based on their affiliated Party 
        alikeParty.clear();
        for(int j=0;j<candidates_.size();j++){
            if(candidates_[j].partyType==party_vector[i]){
                alikeParty.push_back(candidates_[j]);
            }
        }
        sorted_candidates.insert(std::make_pair(party_vector[i],alikeParty));
    }

    std::map<int,District>::iterator district_iterator;
    int totalConstituents=0;
    std::map<int,std::map<std::string,int>> district_vote_tracker;
    if(candidates_.size()!=0){

        for(district_iterator=em_.electoralMap_.begin();district_iterator!=em_.electoralMap_.end();district_iterator++){
            totalConstituents += district_iterator->second.getNumConstituents();        // calculates the total number of constituents in the ElectoralMap
        }

        // for each district in the electoral map
        for(district_iterator=em_.electoralMap_.begin();district_iterator!=em_.electoralMap_.end();district_iterator++){
            // find the majority party of the district
            Party majority = findMajority(district_iterator->second.parties_);
            std::map<Party,std::vector<Candidate>>::iterator sorted_iterator;
            sorted_iterator = sorted_candidates.find(majority);
            int constituents;
            int size = sorted_iterator->second.size();
            if(size==0){              // if the majority party has no candidates
                while(size==0){       // finds the majority Party with at least 1 candidate
                    std::map<Party,int>::iterator it;
                    int max=0;
                    Party otherMajority;
                    for(it=district_iterator->second.parties_.begin();it!=district_iterator->second.parties_.end();it++){
                        if(it->second>=max&&it->first!=Party::None&&it->first!=majority){
                            max = it->second;
                            otherMajority=it->first;
                        }
                    }
                    majority = otherMajority;
                    size = sorted_candidates.find(majority)->second.size();
                }
            } 
            // calculates how many votes a candidate will get for the current district
            constituents = district_iterator->second.parties_.find(majority)->second;
            int num_votes = floor(((constituents*1.0)/totalConstituents)*district_iterator->second.getNumConstituents());

            // if there are multiple candidates from the majority party of the current district, a random candidate must be chosen
            std::vector<Candidate> current_district_candidates = sorted_candidates.find(majority)->second;
            std::map<std::string,int> current_district_candidate_votes;
            int randomIndex = rand() % current_district_candidates.size();
            for(int i=0;i<candidates_.size();i++){
                if(candidates_[i]==current_district_candidates[randomIndex]){
                    candidates_[i].numVotes+=num_votes;             // increments the candidates number of votes
                    std::map<std::string, int>::iterator it = current_district_candidate_votes.find(candidates_[i].name);
                    if(it!=current_district_candidate_votes.end()){     // tracks how many votes the candidate got for the current district
                        it->second+=num_votes;
                    } else {
                        current_district_candidate_votes[candidates_[i].name] = num_votes;
                    }
                }
            }
            // this is the map of how many votes each candidate got in each district
            district_vote_tracker.insert(std::make_pair(district_iterator->first,current_district_candidate_votes));
        }
    }

    std::cout<<std::endl;
    std::map<int,std::map<std::string,int>>::iterator details;
    std::map<std::string,int>::iterator inner_it;
    for(details=district_vote_tracker.begin();details!=district_vote_tracker.end();details++){  // outputs how many votes each candidate got in each district
        std::cout<<"District "<<details->first<<":"<<std::endl;
        for(inner_it=details->second.begin();inner_it!=details->second.end();inner_it++){
            std::cout<<inner_it->first<<": "<<inner_it->second<<std::endl;
        }
    }

    std::cout<<std::endl;

    for(int i=0;i<candidates_.size();i++){                                                     // outputs how many total votes each candidate got    
        std::cout<<candidates_[i].name<<": "<<candidates_[i].numVotes<<" votes"<<std::endl; 
    }

    std::cout<<std::endl;
}