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
