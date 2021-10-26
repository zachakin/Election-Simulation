#include "ElectoralMap.hpp"
#include <stdlib.h>
#include <time.h>

/**
 * This function returns the inputted Party type as a string representation
 * @param Party, type of party
 * @return string version of the Party type
*/
std::string PartyStringify(Party p){
    if(p == Party::one){
		return "one";		
	} else if(p == Party::two){
		return "two";					
	} else {
        return "None";
    }
}

/**
 * Default constructor, sets the size of the district to a random
 * number between 5 and 29, and sets the number of constituents in each
 * party in the District to a random number between 0 and 9
 * @param none
 * @return none
*/
District::District(){
    setSize();
    setConstituents();
}

/**
 * sets the number of constituents in each party in the 
 * District to a random number between 0 and 9
 * @param none
 * @return none
*/
void District::setConstituents(){
    for(int i=0;i<party_vector.size();i++){
        int x = rand()%10;
        parties_.insert(std::make_pair(party_vector[i],x));
        num_constituents_+=x;
    }
}


/**
 * sets the size of the district to a random
 * number between 5 and 29
 * @param none
 * @return none
*/
void District::setSize(){
    size_ = (rand()%25) + 5;
}

/**
 * returns the size of the District
 * @param none
 * @return int, size of the district in square miles
*/
int District::getSize() const{
    return size_;
}

/**
 * Moves a constituent from on Party to another
 * @param Party src and Party dest, the party types where the number of constituents is being changed
 * @return none
*/
int District::moveConstituents(Party src, Party dest){
    typename std::map<Party,int>::iterator it;
    it = parties_.find(src);
    if(it->second!=0){
        it->second--;
        it = parties_.find(dest);
        it->second++;
        return 1;
    }
    return 0;
}

/**
 * returns the district with the inputted id
 * @param int id, id of disrict that is being searched for
 * @return District, the district with the given id
*/
District ElectoralMap::get_district(int id){
    typename std::map<int,District>::iterator it;
    it = electoralMap_.find(id);
    return it->second;
}

/**
 * Overrides the << operator to output each Party and how many constituents it has
 * @param ostream& and District&
 * @return ostream&
*/
std::ostream& operator<<(std::ostream& os, const District &d){
    typename std::map<Party,int>::const_iterator it;
    for(it = d.parties_.begin();it!=d.parties_.end();it++){
        os << "[Party: "<<PartyStringify(it->first)<<"] : "<<it->second<<"      ";
    }
    os<<"\n";
    return os;
}

/**
 * constructor, reates the electoralMap_ with districts based on the number 
 * of districts size_, and assigns each district a unique id
 * @param none
 * @return none
*/
ElectoralMap::ElectoralMap(){
    for(int i=1;i<=NUM_DISTRICTS;i++){
        District *d = new District();
        electoralMap_.insert(std::make_pair(i,*d));
    }
}

/**
 * Overrides the << operator to output each district id, district square miles, and district party/constituent details
 * @param ostream& and ElectoralMap&
 * @return ostream&
*/
std::ostream& operator<<(std::ostream& os, const ElectoralMap &e){
    typename std::map<int,District>::const_iterator e_it;
    for(e_it = e.electoralMap_.begin();e_it!=e.electoralMap_.end();e_it++){
        os<<"\nDistrict "<<e_it->first<<":\n";
        os<<"Square Miles: "<<e_it->second.getSize()<<"\n";
        os<<e_it->second;
    }
    return os;
}