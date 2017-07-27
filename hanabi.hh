#ifndef HANABI_HH
#define HANABI_HH

#include <map>
#include <vector>

#define NCLUE 8
#define NFUSE 4
#define NCOLOR 5
#define NRANK 5
#define NPLAYER 5

enum Color {unk_color, blue, green, red, white, yellow};
enum Rank {unk_rank, one, two, three, four, five};
std::string colorString(Color c);

class Card {
public:
    Card(Color c, Rank r) : color(c), rank(r) {}
    Color color;
    Rank rank;
    void display();
};

typedef std::vector<Card> Pile;

class Deck {
public:
    Deck();
    Card & dealCard();
    size_t size() {return deck.size();}
    void shuffle();
private:
    Pile deck;
};

class CardInfo {
public:
    CardInfo() {};
    Color color = unk_color;
    Rank rank = unk_rank;
    bool color_negative[NCOLOR] = {false, false, false, false, false};
    bool rank_negative[NRANK] = {false, false, false, false, false};
};

class Clue {
public:
    Clue() {};
    Color color;
    Rank rank;
    int pos[5];
};

class HanabiState;

class Player {
public:
    Player(int id, HanabiState & state) : id(id), state(state) {};
    int id;
    Pile hand;
    std::vector<CardInfo> hand_info;
    void playCard(size_t i);
    void discardCard(size_t i);
    void play();
    void dealNewCard();
    void removeCard(size_t i);
private:
    HanabiState & state;
    bool isPlayable(Card &c);
};

class HanabiState {
public:
    int clues = NCLUE;
    int fuses = 0;
    int score = 0;
    Deck deck;
    std::map<Color,Pile> played { {blue, Pile()}, {green, Pile()}, {red, Pile()},
                                  {white, Pile()}, {yellow, Pile()} };
    std::map<Color,Pile> discarded { {blue, Pile()}, {green, Pile()}, {red, Pile()},
                                     {white, Pile()}, {yellow, Pile()} };
    std::vector<Player> players;
};

class Hanabi {
public:
    Hanabi(size_t nplayers, bool debug);
    void deal();
    void shuffle();
    int cardsPerHand() {return state.players.size() <= 3 ? 5 : 4;}
    void play();
    void displayHands();
    int score() {return state.score;}
private:
    size_t n_players;
    bool debug;
    HanabiState state;
};

#endif
