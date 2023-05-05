//#include "std_lib_facilities.h"
#include "round.h"
#include "player.h"

// global variables
vector<int> repeatVoteIDs;
player p1, p2, p3, p4, p5, p6, p7;
vector<player> obj_players = {p1, p2, p3, p4, p5, p6, p7};
int killedNum = 0;
int savedNum = 0;
int gangNum = 0;
int docNum = 0;
int votedOutIndex = -1;
bool docAlive = true;
string doctor;
string gangster;
string last_killed = "";
string last_voted = "";
string last_left = "nobody has left the game yet";
vector<GameRound*> roundsList = {};

// functions
void createPlayers()
{
    for (int i = 0; i <= 6; i++)
    {   
        obj_players[i].setID(i);
        obj_players[i].setAlive(true);
        obj_players[i].setCurrentVotes(0);
    }
}

void outputLog()
{
    ofstream log("TownOfSalem_output.txt");

    for (int i = 0; i < roundsList.size(); i++)
    {
        log << "- Round " << roundsList[i]->getRoundNum() << "\n   " 
            << roundsList[i]->getLeftGangster() << " was killed by the gangster\n   " 
            << roundsList[i]->getLeftVote() << " was voted out" << endl;
    }

    log.close();
}

void playersInGame()
{
    for (int i = 0; i < 7; i++)
    {
        if (obj_players[i].getAlive() == true)
        {
            cout << obj_players[i].getName() << endl;
        }
    }
}

void playersInGameRoles()
{
    for (int i = 0; i < 7; i++)
    {
        if (obj_players[i].getAlive() == true)
        {
            cout << obj_players[i].getName() << ": " << obj_players[i].getRole() << endl;
        }
    }
}

void playersNotInGame()
{
    bool nobodyHasLeft = true;

    for (int i = 0; i < 7; i++)
    {
        if (obj_players[i].getAlive() == false)
        {   
            nobodyHasLeft = false;
            cout << obj_players[i].getName() << endl;
        }
    }

    if (nobodyHasLeft)
    {
        cout << "nobody has left the game yet" << endl;
    }
}

void changePlayerStatus(int index)
{
    obj_players[index].setAlive(false);
    cout << obj_players[index].getName() << " leaves the game." << endl;
}

void resetAllCurrentVotes()
{
    for (int i = 0; i <= 6; i++)
    {
        obj_players[i].setCurrentVotes(0);
    }
}

void initialize()
{   
    // open file
    ifstream infile("../players.txt");
    if (!infile.is_open())
    {
        //cout << "could not open file players.txt. Trying again..." << endl;
        infile.clear();
        ifstream infile("players.txt");
        if (!infile.is_open())
        {
            cout << "Error: could not open file players.txt.\nMake sure players.txt is in the same directory as the executable or one dir up. " << endl;
            exit(1);
            
        }
        else 
        {
            string name;
            string role;
            for (int i = 0; i <= 6; i++) 
            { 
                // read each line of the file
                infile >> name >> role;
                obj_players[i].setName(name); obj_players[i].setRole(role);
                cout << "Player " << i << ": " << name << " - " << role << endl;
                if (role == "doctor")
                {
                    doctor = obj_players[i].getName();
                }
                if (role == "gangster")
                {
                    gangster = obj_players[i].getName();
                }
            }
            infile.close(); // close the input file
        }
    }
    else 
    {
        string name;
        string role;
        for (int i = 0; i <= 6; i++) 
        { 
            // read each line of the file
            infile >> name >> role;
            obj_players[i].setName(name); obj_players[i].setRole(role);
            cout << "Player " << i << ": " << name << " - " << role << endl;
            if (role == "doctor")
            {
                doctor = obj_players[i].getName();
            }
            if (role == "gangster")
            {
                gangster = obj_players[i].getName();
            }
        }
        infile.close(); // close the input file
    }

    cout << "\nthe doctor is " << doctor << '\n';
    cout << "the gangster is " << gangster << '\n';
}

void checkEndCondition()
{
    // check if gangster left
    bool gangsterInGame = true;
    for (int i = 0; i < 7; i++)
    {
        if (obj_players[i].getRole() == "gangster" && obj_players[i].getAlive() == false)
        {
            gangsterInGame = false;
        }
    }
    if (!gangsterInGame)
    {
        cout << " the gangster has left the game.\ncitizens win!\n";
        outputLog();
        exit(0);
    }

    // check if players are 2
    int playerCount = 0;
    for (int k = 0; k < 7; k++)
    {   
        if (obj_players[k].getAlive() == true)
        {
            playerCount++;
        }
    }

    if (playerCount <= 2)
    {
        cout << "\nthe gangster has won" << endl;
        outputLog();
        exit(0);
    }
}

void checkDocInGame()
{
    bool found = false;
    for (int i = 0; i <= 6; i++)
    {
        if (obj_players[i].getRole() == "doctor" && obj_players[i].getAlive() == true)
        {
            found = true;
        }
    }

    if (found == false)
    {
        docAlive = false;
    }
}

void showMenuOptions()
{
    cout << "\n=====================| menu |=====================\n    options: type the number:\n";
    cout << "\n   1. to go to the next phase";
    cout << "\n   2. to quit the game";
    cout << "\n   3. to show players in the game";
    cout << "\n   4. to show players in the game and roles";
    cout << "\n   5. to show the players who have left";
    cout << "\n   6. to show the last player who left";
    cout << "\n==================================================\n";
}

void infoMenu()
{

    int menuChoice;
    string quitOption;
    string menuChoiceStr;

    while (true)
    {

        showMenuOptions();

        getline(cin, menuChoiceStr);

        // exception if user doesnt enter an integer etc
        try
        {
            if (menuChoiceStr.size() - 1 != 0)
            {
                throw 99;
            }
            else
            {
                menuChoice = stoi(menuChoiceStr);
                if (menuChoice >= 10)
                {
                    throw 100;
                }
            }
        }
        catch (...)
        {
            cout << "invalid input\nplease choose one of the following options \n"
                 << endl;
            cin.clear();
            cout << '\n';
            continue;
        }

        switch (menuChoice)
        {
        case 1:
            cout << "entering next phase...\n";
            break;
        case 2:
            cout << "quitting game...\n";
            exit(0);
            break;
        case 3:
            playersInGame();
            continue;
            break;
        case 4:
            playersInGameRoles();
            continue;
            break;
        case 5:
            playersNotInGame();
            continue;
            break;
        case 6:
            cout << last_left << endl;
            continue;
            break;
        default:
            cout << "please choose one of the following options ";
            continue;
            break;
        }
        break;
    }
}

void showPlayersForVote()
{
    for (int i = 0; i < 7; i++)
    {
        if (obj_players[i].getAlive() == true)
        {
            cout << obj_players[i].getID() << " - " << obj_players[i].getName() << endl;
        }
    }
}

void showPlayersRepeatVote()
{
    int pID;
    for (int i = 0; i < repeatVoteIDs.size(); i++)
    {
        pID = repeatVoteIDs[i];
        if (obj_players[pID].getAlive() == true)
        {
            cout << obj_players[pID].getID() << " - " << obj_players[pID].getName() << endl;
        }
    }
}

int gangChoose()
{
    string killedNumStr; //player input
    int index; //player input after conversion

    // input & checking
    while (true)
    {
        cout << "gangster,choose a player to remove: \n";
        showPlayersForVote();

        getline(cin, killedNumStr);

        //validate input
        try
        {
            if (killedNumStr.size() - 1 != 0)
            {
                throw 01;
            }
            else
            {
                index = stoi(killedNumStr);
                if (index > 6 || index < 0)
                {
                    throw 02;
                }
                if (obj_players[index].getAlive() == false)
                {
                    throw 03;
                }
            }
        }
        catch (...)
        {
            cout << "invalid input\nplease choose one of the following options \n"
                 << endl;
            cin.clear();
            cout << '\n';
            continue;
        }

        if (obj_players[index].getRole() == "gangster")
        {
            cout << "gangster, you can't remove yourself! " << endl;
            cin.clear();
            continue;
        }
        break;
    }
    return index;
}

int docChoose()
{
    string savedNumStr;
    int index;

    // input & checking
    while (true)
    {
        cout << "doctor,choose a player to save: \n";
        showPlayersForVote();

        getline(cin, savedNumStr);

        try
        {
            if (savedNumStr.size() - 1 != 0)
            {
                throw 01;
            }
            else
            {
                index = stoi(savedNumStr);
                if (index > 6 || index < 0)
                {
                    throw 02;
                }

                if (obj_players[index].getAlive() == false)
                {
                    throw 03;
                }
            }
        }
        catch (...)
        {
            cout << "invalid input\nplease choose one of the following options \n"
                 << endl;
            cin.clear();
            cout << '\n';
            continue;
        }
        break;
    }
    return index;
}

void night()
{
    cout << "\n\n\n\n\n\n\n\n\nNight falls in Salem...\nThe gangster is " << gangster << endl;
    
    killedNum = gangChoose();

    if (docAlive)
    {

        savedNum = docChoose();

        cout << "\nthe gangster kills " << obj_players[killedNum].getName() 
             << ", \nthe doctor saves " << obj_players[savedNum].getName() << endl;

        // if doc is the victim and he didnt save himself
        if (obj_players[killedNum].getRole() == "doctor" && obj_players[savedNum].getRole() != "doctor")
        {
            docAlive = 0;
            changePlayerStatus(killedNum);
            cout << "\nthe doctor is dead!\n";
        }

        // remove the victim
        if (killedNum - savedNum != 0)
        {
            changePlayerStatus(killedNum);
            last_killed = obj_players[killedNum].getName();
            last_left = obj_players[killedNum].getName();
        }

    }
    else if (!docAlive)
    {
        cout << "\nthe gangster kills " << obj_players[killedNum].getName() << endl;
        changePlayerStatus(killedNum);
        last_killed = obj_players[killedNum].getName();
        last_left = obj_players[killedNum].getName();
    }

    checkEndCondition();
}

void repeatVoting()
{
    string repeatVoteStr;
    int voteIndex;

    resetAllCurrentVotes();

    // asking players to vote
    for (int voter = 0; voter <= 6; voter++)
    {   
        if(obj_players[voter].getAlive() == true)
        {
            //get voteIndex - the ID of the player they voted for
            while(true)
            {
                showPlayersRepeatVote();
                voteIndex = obj_players[voter].vote();

                try
                {
                    if (obj_players[voteIndex].getAlive() == false) { throw 3; }
                    if ( !binary_search(repeatVoteIDs.begin(), repeatVoteIDs.end(), voteIndex) ) { throw 4; }
                }
                catch(...)
                {
                    cout << "invalid input, please choose one of the following: " << endl;
                    cin.clear();
                    continue;
                }
                
                break;
            }

            //increase current votes of the player voted for
            obj_players[voteIndex].gotVoted();
        }
    }
    
   // print vote board
    cout << "\n----------------------------------\nPLAYER NAME : VOTES" << endl;
    for (int k = 0; k <= 6; k++)
    {
        if (obj_players[k].getAlive() == true)
        {
            if ( binary_search(repeatVoteIDs.begin(), repeatVoteIDs.end(), k) ) //since k = id
            {
                cout << "   " << obj_players[k].getName() << " : " 
                     << obj_players[k].getCurrentVotes() << endl;
            }       
        }
    }
    cout << "\n----------------------------------\n"
         << endl;


    //create a vector that stores the votes of player j in element j
    vector<int> repeatVotes{0, 0, 0, 0, 0, 0, 0};
    for (int j = 0; j <= 6; j++)
    {
        repeatVotes[j] = obj_players[j].getCurrentVotes();
    }

    // finding the max
    int max = -1;
    int votedOut_ID;

    for (int z = 0; z <= 6 ; z++)
    {
        if (repeatVotes[z] > max)
        {
            max = repeatVotes[z];
            votedOut_ID = obj_players[z].getID();
        }
    }

    // checking for equal votes, this time if there are some equal to the max, nobody leaves
    int maxCounter = 0;
    for (int k = 0; k <= 6; k++)
    {
        if (repeatVotes[k] == max)
        {
            maxCounter++;
        }
    }

    if (maxCounter > 1)
    {
        cout << "nobody leaves \n";

    }
    else
    {
        changePlayerStatus(votedOut_ID);
        last_voted = obj_players[votedOut_ID].getName();
        last_left = obj_players[votedOut_ID].getName();
    }

    repeatVotes.clear();
    
}

void mainVoting()
{
    string voteStr;
    int voteIndex = -1;
    int activePlayers = 0;

    resetAllCurrentVotes();

    // asking players to vote
    for (int voter = 0; voter <= 6; voter++)
    {   
        if(obj_players[voter].getAlive() == true)
        {
            activePlayers++;
            //get voteIndex - the ID of the player they voted for
            while(true)
            {
                showPlayersForVote();
                voteIndex = obj_players[voter].vote();

                try
                {
                    if (obj_players[voteIndex].getAlive() == false) { throw 3; } 
                }
                catch(...)
                {
                    cout << "invalid input, please choose one of the following: " << endl;
                    cin.clear();
                    continue;
                }
                
                break;
            }

            //increase current votes of the player voted for
            obj_players[voteIndex].gotVoted();
        }
    }
    
    // print vote board
    cout << "\n----------------------------------\nPLAYER NAME : VOTES" << endl;
    for (int k = 0; k <= 6; k++)
    {
        if (obj_players[k].getAlive() == true)
        {
            cout << "   " << obj_players[k].getName() << " : " 
                << obj_players[k].getCurrentVotes() << endl;
        }
    }
    cout << "\n----------------------------------\n"
         << endl;


    //create a vector that stores the votes of player j in element j
    vector<int> votes{0, 0, 0, 0, 0, 0, 0};
    for (int j = 0; j <= 6; j++)
    {
        if (obj_players[j].getAlive() == true)
        {
            votes[j] = obj_players[j].getCurrentVotes();
        }
    }

    // deciding who leaves
    int max = -1;
    int votedOut_ID;

    for (int z = 0; z <= 6; z++)
    {
        if (votes[z] > max)
        {
            max = votes[z];
            votedOut_ID = z;
        }
    }

    // checking if some have equal votes
    int maxCounter = 0;
    repeatVoteIDs.clear(); // making sure the repeat voting members vector is empty
    for (int k = 0; k <= 6; k++)
    {
        if (votes[k] == max)
        {
            maxCounter += 1;
            repeatVoteIDs.push_back(obj_players[k].getID()); // when i find someone with equal votes to the max i keep them here
        }
    }

    if (maxCounter > 1 && maxCounter < activePlayers)
    {
        repeatVoting();
    }
    else if (maxCounter == activePlayers)
    {
        cout << "nobody leaves the game\n";
    }
    else
    {
        changePlayerStatus(votedOut_ID);
        last_voted = obj_players[votedOut_ID].getName();
        last_left = obj_players[votedOut_ID].getName();
    }

    cout << "the voting is over\n";

    // reset my counters and vectors to have them ready for the next day
    max = -1;
    maxCounter = 0;
    votedOut_ID = -1;
    votes.clear();

    checkDocInGame();
}

void day()
{
    cout << "\n\n\n\n\n\n\n\nit's day in Salem...\nthe players are ready to vote who's going to leave:" << endl;

    mainVoting();
}

void gameLoop()
{
    int roundcounter = 0;
    while (true)
    {
        roundcounter++;
        infoMenu();
        cout << "ROUND " << roundcounter << endl;
        night();
        day();
        GameRound *gameround = new GameRound(roundcounter, last_killed, last_voted);
        roundsList.push_back(gameround);
        checkEndCondition();
    }

    cout << "end\n";
}

int main()
{
    createPlayers();
    initialize();
    gameLoop(); // info-menu, night, day, info-menu, night, day,...

    return 0;
}