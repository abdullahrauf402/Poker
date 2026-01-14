#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <locale>

using namespace std;
/* ---------- Card Data ---------- */
vector<string> RANKS = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
vector<string> SUITS = { "S","H","D","C" };
map<string, int> RANK_VALUES = {
    {"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},
    {"9",9},{"10",10},{"J",11},{"Q",12},{"K",13},{"A",14}
};
map<string, string> SUIT_SYMBOLS = {
    {"S","S"}, {"H","H"}, {"D","D"}, {"C","C"}
};
struct Card {
    string rank;
    string suit;
};
vector<Card> createDeck() {
    vector<Card> deck;
    for (string r : RANKS)
        for (string s : SUITS)
            deck.push_back({ r,s });
    return deck;
}
vector<string> asciiCard(Card c) {
    string r = c.rank;
    string s = SUIT_SYMBOLS[c.suit];
    string left = r + string(2 - r.length(), ' ');
    string right = string(2 - r.length(), ' ') + r;
    return {
        "┌─────┐",
        "|" + left + "   |",
        "|  " + s + "  |",
        "|   " + right + "|",
        "└─────┘"
    };
}
void printAsciiCards(vector<Card> cards) {
    vector<vector<string>> arts;
    for (Card c : cards)
        arts.push_back(asciiCard(c));
    for (int i = 0; i < 5; i++) {
        for (auto& a : arts)
            cout << a[i] << "  ";
        cout << endl;
    }
}
pair<int, vector<int>> evaluateHand(vector<Card> cards) {
    vector<int> ranks;
    map<int, int> rankCount;
    map<string, int> suitCount;
    for (Card c : cards) {
        int val = RANK_VALUES[c.rank];
        ranks.push_back(val);
        rankCount[val]++;
        suitCount[c.suit]++;
    }
    sort(ranks.begin(), ranks.end(), greater<int>());
    bool flush = false;
    for (auto& s : suitCount)
        if (s.second >= 5) flush = true;
    vector<int> unique = ranks;
    sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
    reverse(unique.begin(), unique.end());
    bool straight = false;
    for (int i = 0; i + 4 < unique.size(); i++) {
        if (unique[i] - unique[i + 4] == 4) {
            straight = true;
            break;
        }
    }
    bool four = false, three = false;
    int pairs = 0;
    for (auto& r : rankCount) {
        if (r.second == 4) four = true;
        if (r.second == 3) three = true;
        if (r.second == 2) pairs++;
    }
    if (straight && flush) return { 8, ranks };
    if (four) return { 7, ranks };
    if (three && pairs == 1) return { 6, ranks };
    if (flush) return { 5, ranks };
    if (straight) return { 4, ranks };
    if (three) return { 3, ranks };
    if (pairs == 2) return { 2, ranks };
    if (pairs == 1) return { 1, ranks };
    return { 0, ranks };
}
char cpuDecision() {
    int r = rand() % 100;
    if (r < 60) return 'c';
    if (r < 85) return 'b';
    return 'f';
}
bool bettingRound(int& playerMoney, int& cpuMoney, int& pot) {
    char action;
    cout << "\nYour money: $" << playerMoney << endl;
    cout << "CPU money: $" << cpuMoney << endl;
    while (true) {
        cout << "Check (c), Bet (b), or Fold (f): ";
        cin >> action;
        if (action == 'f') {
            cout << "You fold.\n";
            return false;
        }
        if (action == 'c') {
            cout << "You check.\n";
            break;
        }
        if (action == 'b') {
            int bet;
            cout << "Enter bet amount: ";
            cin >> bet;
            if (bet <= 0 || bet > playerMoney) {
                cout << "Invalid bet.\n";
                continue;
            }
            playerMoney -= bet;
            pot += bet;
            cout << "You bet $" << bet << endl;
            break;
        }
    }
    char cpuAction = cpuDecision();
    if (cpuAction == 'f') {
        cout << "CPU folds.\n";
        playerMoney += pot;
        pot = 0;
        exit(0);
    }
    if (cpuAction == 'c') {
        cout << "CPU checks.\n";
        return true;
    }
    if (cpuAction == 'b' && cpuMoney >= 50) {
        int cpuBet = min(50, cpuMoney);
        cpuMoney -= cpuBet;
        pot += cpuBet;
        cout << "CPU bets $" << cpuBet << endl;
    }
    return true;
}
int main() {
    setlocale(LC_ALL, "");
    srand(time(0));
    int playerMoney = 500;
    int cpuMoney = 500;
    int pot = 0;
    cout << "\nCommand Line Poker\n\n";
    vector<Card> deck = createDeck();
    shuffle(deck.begin(), deck.end(), default_random_engine(rand()));
    vector<Card> player = { deck.back(), deck[deck.size() - 2] };
    deck.pop_back(); deck.pop_back();
    vector<Card> cpu = { deck.back(), deck[deck.size() - 2] };
    deck.pop_back(); deck.pop_back();
    vector<Card> community;
    cout << "Your hand:\n";
    printAsciiCards(player);
    if (!bettingRound(playerMoney, cpuMoney, pot)) return 0;
    for (int i = 0; i < 3; i++) {
        community.push_back(deck.back());
        deck.pop_back();
    }
    cout << "\nFlop:\n";
    printAsciiCards(community);
    if (!bettingRound(playerMoney, cpuMoney, pot)) return 0;
    community.push_back(deck.back());
    deck.pop_back();
    cout << "\nTurn:\n";
    printAsciiCards(community);
    if (!bettingRound(playerMoney, cpuMoney, pot)) return 0;
    community.push_back(deck.back());
    deck.pop_back();
    cout << "\nRiver:\n";
    printAsciiCards(community);
    if (!bettingRound(playerMoney, cpuMoney, pot)) return 0;
    cout << "\nShowdown\n";
    cout << "Your hand:\n";
    printAsciiCards(player);
    cout << "\nCPU hand:\n";
    printAsciiCards(cpu);
    vector<Card> playerAll = player;
    vector<Card> cpuAll = cpu;
    playerAll.insert(playerAll.end(), community.begin(), community.end());
    cpuAll.insert(cpuAll.end(), community.begin(), community.end());
    auto pScore = evaluateHand(playerAll);
    auto cScore = evaluateHand(cpuAll);
    if (pScore > cScore) {
        cout << "\nYou win the pot of $" << pot << endl;
        playerMoney += pot;
    }
    else if (cScore > pScore) {
        cout << "\nCPU wins the pot of $" << pot << endl;
        cpuMoney += pot;
    }
    else {
        cout << "\nTie game. Pot split.\n";
        playerMoney += pot / 2;
        cpuMoney += pot / 2;
    }
    cout << "\nFinal money:\n";
    cout << "You: $" << playerMoney << endl;
    cout << "CPU: $" << cpuMoney << endl;
    return 0;
}