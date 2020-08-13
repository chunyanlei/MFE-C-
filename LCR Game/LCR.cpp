//
//  main.cpp
//  HW4
//
//  Created by 雷春妍 on 2020/1/18.
//  Copyright © 2020年 雷春妍. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

class Player
{
private:
    const int * chipDist;
    int numChips;
    Player* leftPlayer;
    Player* rightPlayer;
public:
    Player(){}
    Player(const int* chipDistribution);
    ~Player(){}
    void setLeftPlayer(Player* p);
    Player* getLeftPlayer();
    void setRightPlayer(Player* p);
    Player* getRightPlayer();
    int getNumChips();
    void setNumChips(int nc);
    int addChips(int nc);
    int play(int d1, int d2, int d3);
    int eachPlay(int d);
    
};

// Part 1 - Creating Classes
// A.
// a.
Player::Player(const int* chipDistribution)
{
    chipDist = chipDistribution;
}

// b.
void Player::setLeftPlayer(Player *p)
{
    leftPlayer = p;
}

Player* Player::getLeftPlayer()
{
    return leftPlayer;
}

// c.
void Player::setRightPlayer(Player *p)
{
    rightPlayer = p;
}

Player* Player::getRightPlayer()
{
    return rightPlayer;
}

// d.
int Player::getNumChips()
{
    return numChips;
}

void Player::setNumChips(int nc)
{
    numChips = nc;
}

// e.
int Player::addChips(int nc)
{
    setNumChips(getNumChips() + nc);
    return getNumChips();
}

// f.
int Player::play(int d1, int d2, int d3)
{
    
    int num = getNumChips();
    int centerChips = 0;
    if(num >= 1)
        centerChips += eachPlay(d1);
    if(num >= 2)
        centerChips += eachPlay(d2);
    if(num >= 3)
        centerChips += eachPlay(d3);
    
    return centerChips;
}

int Player::eachPlay(int d)
{
    int centerChips = 0;
    if(chipDist[d] == 1)
    {
        this->addChips(-1);
        leftPlayer->addChips(1);
        //cout<<"Move chip to left"<<endl;
    }
    else if(chipDist[d] == 2)
    {
        this->addChips(-1);
        centerChips++;
        //cout<<"Move chip to center"<<endl;
    }
    else if(chipDist[d] == 3)
    {
        this->addChips(-1);
        rightPlayer->addChips(1);
        //cout<<"Move chip to right"<<endl;
    }
    return centerChips;
}


// B.
// a.
class Game
{
private:
    int numP;
    Player** players;
    long seed;
    int* chip_count;
    mt19937 mt_rand;
    
public:
    Game(int numPlayers, const int* chipDistribution, long seed);
    virtual ~Game();
    int countPlayersWithChips();
    int playRound(int startingPlayer);
    const int* playGame(const int* startingChips, int maxRounds);
};

Game::Game(int numPlayers, const int* chipDistribution, long seed)
{
    numP = numPlayers;
    this->seed = seed;
    players = new Player* [numP];
    chip_count = new int[numP];
    
    // set Chip Distribution for each player
    for(int i = 0; i < numPlayers; i++)
    {
        players[i] = new Player(chipDistribution);
    }
    
    // set left player and right player for each player
    for(int i = 0; i < numPlayers; i++)
    {
        int leftIndex = (i + 1 + numPlayers) % numPlayers;
        int rightIndex = (i - 1 + numPlayers) % numPlayers;
    
        players[i]->setLeftPlayer(players[leftIndex]);
        players[i]->setRightPlayer(players[rightIndex]);
    
    }
    mt19937 mt_rand(seed);
}


// b.
Game::~Game()
{
    
    for(int i = 0; i < numP; i++)
    {
        delete players[i];
    }
    
    delete [] players;
    delete [] chip_count;
}

// c.
int Game::countPlayersWithChips()
{
    int cnt = 0;
    for(int i = 0; i < numP; i++)
    {
        if(players[i]->getNumChips() > 0)
            cnt++;
    }
    return cnt;
}

// d.
int Game::playRound(int startingPlayer)
{
    int cnt = 0;
    uniform_int_distribution<int> dis_unif(0, 5);
    for(int i = 0; i < numP; i++)
    {
        int index = (i + startingPlayer) % numP;

        int d1 = dis_unif(mt_rand);
        int d2 = dis_unif(mt_rand);
        int d3 = dis_unif(mt_rand);
        cnt += players[index]->play(d1, d2, d3);
    }
    return cnt;
}

const int* Game::playGame(const int *startingChips, int maxRounds)
{
    
    uniform_int_distribution<int> dis_unif(0, numP);
    int startPlayer = dis_unif(mt_rand);
    for(int i = 0; i < numP; i++)
    {
        players[i]->setNumChips(startingChips[i]);
        chip_count[i] = startingChips[i];
    }
    
    int chip_center = 0;
    
    for(int i = 0; i < maxRounds; i++)
    {
        chip_center += playRound(startPlayer);
        if(countPlayersWithChips() == 1)
        {
            break;
        }
    }
    
    
    // when the game is concluded
    if(countPlayersWithChips() == 1)
    {
        for(int i = 0; i < numP; i++)
        {
            chip_count[i] = players[i]->getNumChips();
            if(players[i] -> getNumChips() > 0)
                players[i] -> addChips(chip_center);
        }
    }
    
    return chip_count;
}

void SimulateGame(const char* desc
                  , int numPlayers
                  , const int* chipDistribution
                  , long seed
                  , const int* startingChips
                  , int maxRounds
                  , ostream& outputStream)
{
    
    // create the game object
    Game game(numPlayers, chipDistribution, seed);
    
    // initialize our expected chips array
    double* expectedChips= new double[numPlayers];
    for (int i = 0; i < numPlayers; i++) {
        expectedChips[i] = 0;
    }
    
    double roundsWithWinner = 0;
    // run 100000 simulations
    for (int s = 0; s < 100000; s++) {
        
        // play a single game
        const int* playerChips = game.playGame(startingChips, maxRounds);
        
        // keep track of chips held at end of game
        for (int i = 0; i < numPlayers; i++) {
            expectedChips[i] += playerChips[i];
        }
        
        // was there a single winner?
        if (game.countPlayersWithChips() == 1) {
            roundsWithWinner += 1;
        }
        
        // for simulation iterations of 100, 1000, 10000 and 100000, write the output
        if ((s + 1) == 100 || (s + 1) == 1000 || (s + 1) == 10000 || (s + 1) == 100000) {
            cout << desc << ',' << (s + 1) << endl;
            outputStream << desc << ',' << (s + 1) << ',' << (roundsWithWinner/(s+1));
            for (int i = 0; i < numPlayers; i++) {
                double ev = expectedChips[i] / (s + 1);
                outputStream << ',' << ev;
            }
            outputStream << endl;
        }
        
    }
}


int main()
{
    ofstream outfile;
    
    // open the file
    outfile.open("lcr_output.csv");
    
    // write a header
    outfile << "Game,MaxRounds,RoundsWithWinner" ;
    for (int i = 0; i < 9; i++) {
        outfile << ",Player" << (i + 1);
    }
    outfile << endl;
    
    // standard game
    int chipDistribution[] = { 0,0,0,1,2,3 };
    int playerChips[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10};
    SimulateGame("standard", 9, chipDistribution, (long)time(0), playerChips, 100, outfile);
    
    // standard game - one player has more
    int playerChipsTilted[] = { 5, 5, 5, 5, 50, 5, 5, 5, 5, 5 };
    SimulateGame("standard-tilted", 9, chipDistribution, (long)time(0), playerChipsTilted, 100, outfile);
    
    // game with greater chance of passing but no center
    int chipDistributionNoCenter[] = { 0,0,1,1,3,3 };
    SimulateGame("nocenter", 9, chipDistributionNoCenter, (long)time(0), playerChips, 100, outfile);
    
    SimulateGame("nocenter-tilted", 9, chipDistributionNoCenter, (long)time(0), playerChipsTilted, 100, outfile);
    
    // game with greater chance of passing but no 'holding'
    int chipDistributionNoHold[] = { 1,1,2,2,3,3 };
    SimulateGame("nohold", 9, chipDistributionNoHold, (long)time(0), playerChips, 100, outfile);
    
    SimulateGame("nohold-tilted", 9, chipDistributionNoHold, (long)time(0), playerChipsTilted, 100, outfile);
    
    
    // close the file
    outfile.close();
    
    return 0;
}









