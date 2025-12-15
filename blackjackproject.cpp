// Abdullah Siddiqui
// CIS-7 Final Project: BlackJack Game


#include <iostream>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

// Enumerations for card ranks and suits
enum Rank {
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
};

enum Suit {
    CLUBS, DIAMONDS, HEARTS, SPADES
};

// Structure representing a card
struct Card {
    Rank rank;
    Suit suit;
};

// Class representing a deck of cards
class Deck {
private:
    vector<Card> cards;
    int index;
public:
    Deck() {
        cards.reserve(52);
        index = 0;
        for (int suit = CLUBS; suit <= SPADES; ++suit) {
            for (int rank = ACE; rank <= KING; ++rank) {
                Card card{ static_cast<Rank>(rank), static_cast<Suit>(suit) };
                cards.push_back(card);
            }
        }
        shuffle();
    }

    void shuffle() {
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(cards.begin(), cards.end());
        index = 0;
    }

    Card drawCard() {
        return cards[index++];
    }
};

// Class representing a hand (player or dealer)
class Hand {
protected:
    vector<Card> cards;
public:
    // Add a card to the hand
    void addCard(Card card) {
        cards.push_back(card);
    }

    // Get the total value of the hand
    int getValue() const {
        int value = 0;
        int numAces = 0;

        // Calculate the total value, taking into account Aces and their possible values (1 or 11)
        for (const auto& card : cards) {
            if (card.rank >= TWO && card.rank <= TEN) {
                value += card.rank + 1;
            }
            else if (card.rank >= JACK && card.rank <= KING) {
                value += 10;
            }
            else if (card.rank == ACE) {
                value += 11;
                ++numAces;
            }
        }

        // Adjust the value if there are Aces and the total value exceeds 21
        while (value > 21 && numAces > 0) {
            value -= 10;
            --numAces;
        }

        return value;
    }

    // Display the cards in the hand
    void displayHand() const {
        for (const auto& card : cards) {
            switch (card.rank) {
            case ACE:
                cout << "A";
                break;
            case TWO:
                cout << "2";
                break;
            case THREE:
                cout << "3";
                break;
            case FOUR:
                cout << "4";
                break;
            case FIVE:
                cout << "5";
                break;
            case SIX:
                cout << "6";
                break;
            case SEVEN:
                cout << "7";
                break;
            case EIGHT:
                cout << "8";
                break;
            case NINE:
                cout << "9";
                break;
            case TEN:
                cout << "10";
                break;
            case JACK:
                cout << "J";
                break;
            case QUEEN:
                cout << "Q";
                break;
            case KING:
                cout << "K";
                break;
            }

            switch (card.suit) {
            case CLUBS:
                cout << "C ";
                break;
            case DIAMONDS:
                cout << "D ";
                break;
            case HEARTS:
                cout << "H ";
                break;
            case SPADES:
                cout << "S ";
                break;
            }
        }
    }
};

// Class representing the player's hand
class PlayerHand : public Hand {
public:
    // Check if the hand is busted (total value > 21)
    bool isBusted() const {
        return getValue() > 21;
    }

    // Check if the hand is a blackjack (two cards with total value = 21)
    bool isBlackjack() const {
        return cards.size() == 2 && getValue() == 21;
    }
};

// Class representing the dealer's hand
class DealerHand : public Hand {
public:
    // Determine if the dealer should hit (total value <= 16)
    bool shouldHit() const {
        return getValue() <= 16;
    }
};

// Calculate the winning probability for the player's hand
double calculateWinningProbability(const Hand& playerHand, const Hand& dealerHand, const Deck& deck, int iterations) {
    int wins = 0;

    for (int i = 0; i < iterations; ++i) {
        Deck shuffledDeck = deck;

        PlayerHand currentPlayerHand = static_cast<const PlayerHand&>(playerHand);
        DealerHand currentDealerHand = static_cast<const DealerHand&>(dealerHand);

        // Simulate the dealer's actions until they stop hitting
        while (!currentPlayerHand.isBusted() && !currentPlayerHand.isBlackjack()) {
            currentDealerHand.addCard(shuffledDeck.drawCard());

            if (currentDealerHand.shouldHit()) {
                currentDealerHand.addCard(shuffledDeck.drawCard());
            }
            else {
                break;
            }

            if (currentDealerHand.getValue() > 21) {
                ++wins;
                break;
            }
        }

        // Check the outcome of the game and increment the win count
        if (!currentPlayerHand.isBusted() && !currentPlayerHand.isBlackjack()) {
            if (currentDealerHand.getValue() > 21 || currentPlayerHand.getValue() > currentDealerHand.getValue()) {
                ++wins;
            }
        }
    }

    return static_cast<double>(wins) / iterations;
}

int main() {
    Deck deck;
    PlayerHand playerHand;
    DealerHand dealerHand;

    // Initial dealing of cards
    playerHand.addCard(deck.drawCard());
    dealerHand.addCard(deck.drawCard());
    playerHand.addCard(deck.drawCard());
    dealerHand.addCard(deck.drawCard());

    // Introduction
    cout << "Hello! Welcome to BlackJack!" << endl << endl << "Here are the cards:" << endl << endl;

    cout << "Dealer's hand: ";
    dealerHand.displayHand();
    cout << endl;
    cout << "Player's hand: ";
    playerHand.displayHand();
    cout << endl << endl;

    int iterations = 10000;
    double probabilityBeforeHit = calculateWinningProbability(playerHand, dealerHand, deck, iterations);
    cout << "Winning probability before hit: " << probabilityBeforeHit * 100 << "%" << endl;

    // If the player hasn't busted or achieved a blackjack, simulate hitting a card and calculate the new probability
    if (!playerHand.isBusted() && !playerHand.isBlackjack()) {
        playerHand.addCard(deck.drawCard());

        cout << "Player's hand after hit: ";
        playerHand.displayHand();
        cout << endl;

        double probabilityAfterHit = calculateWinningProbability(playerHand, dealerHand, deck, iterations);
        cout << "Winning probability after hit: " << probabilityAfterHit * 100 << "%" << endl << endl << endl;

        cout << "Thanks for playing! Be sure to come by again!" << endl << endl;
    }

    return 0;
}

