/**
 * Zach Akin-Amland
 * CSCI 3010
 * HW 3 - Election
 * 
 * This is the main file for the Election program
 * This program randomly generates an ElectoralMap with a number of Districts
 * In each district a random number of constituents is generated for each Party
 * The user is asked if they want to simulate a direct or representative election
 * Regardless of the user's choice, the user then registers candidates for the elction,
 * then the user can choose to campaign each candidate in any District
 * Depending on the election type, votes are counted differently
 * and the candidate that won the election is displayed
 * 
 * To change the number of districts: 
 * change the value of 'static const int NUM_DISTRICTS' in ElectoralMap.hpp ElectoralMap class
 * 
 * To add or remove a Party:
 * In ElectoralMap, add the party's name to 'enum class Party' and add Party::<name>
 * to the declaration of 'const std::vector<Party> party_vector'
 * 
 * Commands to run the program:
 * $ make clean
 * $ make all
 * $ ./election
*/
#include "Election.hpp"

int main(){
    srand(time(0));
    TextUI ui;
    int x =-1;
    while(x!=0){
        x=ui.routeChoice();
        std::vector<Candidate> c;
        std::string name="";
        int id_counter = 1;
        int y;
        switch (x){
            case 1:
            case 2:
                for(int i=0;i<party_vector.size()-1;i++){
                    y=-1;
                    while(y!=0){
                        y=ui.registerCandidate(party_vector[i],name);
                        if(y==1){
                            Candidate a;
                            a.name=name;
                            a.partyType=party_vector[i];
                            a.numVotes=0;
                            a.id=id_counter;
                            id_counter++;
                            c.push_back(a);
                        }
                    }
                }
                if(x==1){
                    Election* E = new Election(c);
                    E->campaign();
                    E->votes();
                    E->getWinner();
                } else {
                    RepresentativeElection* R = new RepresentativeElection(c);
                    R->campaign();
                    R->votes();
                    R->getWinner();
                }
            break;
            case 0:
            break;
            default:
            break;
        }
    }
    return 0;
}