#include "hanabi.hh"
#include <iostream>
#include <random>
#include <sstream>

std::string
colorString(Color c) {
    if (c == blue) {
        return "B";
    } else if (c == green) {
        return "G";
    } else if (c == red) {
        return "R";
    } else if (c == white) {
        return "W";
    } else if (c == yellow) {
        return "Y";
    }
    return "U";
}

std::ostream&
operator<<(std::ostream &os, Card const &c) {
    return os << c.rank << colorString(c.color);
}

void
Card::display() {
    std::cout << this << std::endl;
}

Deck::Deck() {
    // Create deck
    for (int i = 1; i <= NCOLOR; ++i) {
        for (int k = 0; k < 3; ++k) {
            deck.push_back(Card(static_cast<Color>(i), one));
        }
        for (int j = 2; j <= 4; ++j) {
            for (int k = 0; k < 2; ++k) {
                deck.push_back(Card(static_cast<Color>(i), static_cast<Rank>(j)));
            }
        }
        deck.push_back(Card(static_cast<Color>(i), five));
    }
}

Card &
Deck::dealCard() {
    Card & c = deck.back();
    deck.pop_back();
    return c;
}

void
Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

// Display a player as follows:
// Player <id>: <rank><color> [info] <rank><color> [info] ...
std::ostream&
operator<<(std::ostream &os, Player const &p) {
    os << "Player " << p.id << ": ";
    for (int i = 0; i < p.hand.size(); ++i) {
        os << p.hand[i] << " [";
        if (p.hand_info[i].rank != unk_rank) {
            os << p.hand_info[i].rank;
        } else {
            for (int r = 1; r <= NRANK; ++r) {
                if (p.hand_info[i].rank_negative[r]) {
                    os << "!" << r;
                }
            }
        }
        if (p.hand_info[i].color != unk_color) {
            os << p.hand_info[i].color;
        } else {
            for (int c = 1; c <= NCOLOR; ++c) {
                if (p.hand_info[i].color_negative[static_cast<Color>(c)]) {
                    os << "!" << colorString(static_cast<Color>(c));
                }
            }
        }
        os << "] ";
    }
    return os;
}

bool
Player::isPlayable(Card &c) {
    auto pd = state.played[c.color];
    return (pd.size() && pd.back().rank == (c.rank - 1)) ||
           (!pd.size() && c.rank == 1);
}

void
Player::playCard(size_t i) {
    auto c = hand[i];
    if (!isPlayable(c)) {
        state.discarded[c.color].push_back(c);
        state.fuses++;
    } else {
        state.played[c.color].push_back(c);
        state.score++;
    }
    removeCard(i);
}

void
Player::discardCard(size_t i) {
}

void
Player::removeCard(size_t i) {
    hand.erase(hand.begin() + i);
    hand_info.erase(hand_info.begin() + i);
}

void
Player::dealNewCard() {
    if (state.deck.size()) {
        hand.insert(hand.begin(), state.deck.dealCard());
        hand_info.insert(hand_info.begin(), CardInfo());
    }
}

void
Player::play() {
    playCard(hand.size()-1);
    dealNewCard();
}

Hanabi::Hanabi(size_t nplayers, bool debug) : n_players(nplayers), debug(debug) {
    // Instantiate the appropriate number of players
    for (auto i = 0; i < n_players; ++i) {
        state.players.push_back(Player(i, state));
    }
}

void
Hanabi::deal() {
    state.deck.shuffle();
    for (int i = 0; i < cardsPerHand(); ++i) {
        for (int j = 0; j < state.players.size(); ++j) {
            state.players[j].dealNewCard();
        }
    }
}

void
Hanabi::displayHands() {
    for (auto p = state.players.begin(); p != state.players.end(); ++p) {
        std::cout << p << std::endl;
    }
}

void
Hanabi::play() {
    size_t p = 0;
    size_t turns_remaining = state.players.size();

    while (true) {
        // If all the fuses have been used up, the game ends.
        if (state.fuses == NFUSE) {
            return;
        }

        // When the deck runs out, each player gets one more turn.
        if (!state.deck.size() && !(--turns_remaining)) {
            return;
        }

        state.players[p].play();
        std::cout << state.players[p] << std::endl;
        p = (p + 1) % n_players;
        if (debug) displayHands();
    }
}
