#ifndef ELECTORALMAP_H
#define ELECTORALMAP_H
#include <map>
#include <vector>
#include <iostream>

enum class Party { 
    one,
    two,
    None
};

const std::vector<Party> party_vector = { Party::one, Party::two, Party::None};

std::string PartyStringify(Party p);                        // converts the inputted party to a string represention

class District {
    public:
        District();                                         // constructor, calls setsSize() and setConstituents
        void setConstituents();                             // randomly sets number of constituents in each party
        void setSize();                                     // sets the size of the district
        int getSize() const;                                      // returns the size of the district      
        int moveConstituents(Party source, Party dest);    // takes one constituents from the souce and moves it to the destiation party
        std::map<Party,int> parties_;                       // keeps track of number of constituents in each party
        int getNumConstituents(){return num_constituents_;};
        friend std::ostream& operator<<(std::ostream& os, const District &d);
    private:
        int size_;                                          // size of the district in square miles
        int num_constituents_;                              // number of constituents in the district

};

class ElectoralMap {
    public:
        static ElectoralMap& GetInstance() {
            static ElectoralMap instance;
            return instance;
        }

        static const int NUM_DISTRICTS = 4;                  // number of districts to put into the map  
        District get_district(int id);                       // returns the district given an id   
        std::map<int,District> electoralMap_;               // map of each district with a unique id
  
        // Delete the methods we don't want
        ElectoralMap(ElectoralMap const&) = delete;         // copy constructor
        void operator=(ElectoralMap const&) = delete;       // assignment operator

        friend std::ostream& operator<<(std::ostream& os, const ElectoralMap &e);
    private:
        ElectoralMap();
    };

#endif