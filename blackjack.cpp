#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> //shuffle
#include <random> //random generator
using namespace std;


//CARD STRUCT
/******************************************************************************/
enum Suit{club, diamond, heart, spade};
enum Rank{Two = 2,Three = 3,Four = 4,Five = 5,Six = 6,Seven = 7,Eight = 8,Nine = 9,Ten = 10,Jack = 11,Queen = 12,King = 13,Ace = 14};

struct Card{
    Rank rank;
    Suit suit;
    Card();
    Card(Rank r, Suit s){
        rank = r;
        suit = s;
    }
    string printRank(Rank);
    string printSuit(Suit);
    string printCard();
};

Rank& operator++(Rank& r){return r = Rank(static_cast<int>(r)+1);}
Suit& operator++(Suit& s){return s = Suit(static_cast<int>(s)+1);}
Rank& operator--(Rank& r){return r = Rank(static_cast<int>(r)-1);}
Suit& operator--(Suit& s){return s = Suit(static_cast<int>(s)-1);}
ostream& operator<<(ostream& out, Card& obj){return out << obj.printCard();}

string Card::printRank(Rank rank){
    switch (rank){
        case Two: return "2";
        case Three: return "3";
        case Four: return "4";
        case Five: return "5";
        case Six: return "6";
        case Seven: return "7";
        case Eight: return "8";
        case Nine: return "9";
        case Ten: return "10";
        case Jack: return "J";
        case Queen: return "Q";
        case King: return "K";
        case Ace: return "A";
        default: return "VOID";
    }
}
string Card::printSuit(Suit suit){
    switch(suit){
        case club: return "\u2663";
        case diamond: return "\u2666";
        case heart: return "\u2665";
        case spade: return "\u2660";
        default: return "VOID";
    }
}
string Card::printCard(){
  return ("[" + printRank(rank) + printSuit(suit) + "]");
}

stack<Card> getDeck(){
  vector<Card> _deck;
  Rank rank = Two;
  Suit suit = club;
  for(int i = 0; i < 4; i++){
      for(int j = 0; j < 13; j++){
          _deck.push_back(Card(rank, suit));
          ++rank;
      }
      ++suit;
      rank = Two;
  }
  shuffle(_deck.begin(), _deck.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
  stack<Card> deck;
  while(!_deck.empty()){
    deck.push(_deck.back());
    _deck.pop_back();
  }
  return deck;
}

void deal(stack<Card>& hand, stack<Card>& deck){
  if(deck.empty()) deck = getDeck();
  hand.push(deck.top());
  deck.pop();
}

//GAME LOGIC
/******************************************************************************/

int value(stack<Card> hand){
  int total = 0, value, ace = 0;
  while(!hand.empty()){
    value = hand.top().rank;
    hand.pop();
    if(value > 10 && value < 14) total += 10;
    else if(value == 14) ace++;
    else total += value;
  }
  for(int i = 0; i < ace; i++){
    if(total < 11) total += 11;
    else total++;
  }
  return total;
}

bool bust(stack<Card> hand){
  return(value(hand) > 21);
}

void print(stack<Card> hand, stack<Card> dealer){
  cout << "\033[2J\033[1;1H";
  cout << "\nDealer: " << dealer.top();
  cout << "\n   You: ";
  while(!hand.empty()){
    cout << hand.top();
    hand.pop();
  }
  cout << endl;
}

bool result(stack<Card> hand, stack<Card> dealer){
  cout << "\033[2J\033[1;1H";
  bool win = false, _bust = false;
  if(bust(hand)) _bust = true;
  int vhand = value(hand);
  int vdeal = value(dealer);
  if(!bust(hand) && (vhand >= vdeal)) win = true;
  cout << "\nDealer: " << dealer.top();
  dealer.pop();
  cout << dealer.top();
  cout << "\n   You: ";
  while(!hand.empty()){
    cout << hand.top();
    hand.pop();
  }
  cout << "\n\n";
  if(_bust) cout << "Bust! ";
  if(win){
    cout << "You Win!\n\n";
    return true;
  }else{
    cout << "You Lose.\n\n";
    return false;
  }
}

bool bet(int& money, int amount){
  if(amount > money){
    cout << "You can't cover that bet." << endl;
    return false;
  }else if(amount < 1){
    cout << "You have to bet something." << endl;
    return false;
  }else{
    money -= amount;
    return true;
  }
}

//MAIN
/******************************************************************************/
int main(){
  int money = 20, pot = 0;
  stack<Card> hand, dealer, deck;
  char c = 'h', again = 'y';
  while(again != 'n' && money > 0){
    deal(hand, deck);
    deal(dealer, deck);
    deal(dealer, deck);
    cout << "\nWhat do you want to bet?\ncurrent balance: $" << money << endl;
    do{cin >> pot;}while(!bet(money, pot));
    while(!bust(hand)){
      print(hand, dealer);
      cout << "\nHit or Stay? (h)/(s) ";
      cin >> c;
      switch(c){
        case 'h': deal(hand, deck);
          break;
        case 's': if(result(hand, dealer)) money += (2*pot);
          break;
        default: cout << "Invalid." << endl;
      }
      if(c == 's') break;
    }
    if(result(hand, dealer)){}
    cout << "Play again? (y/n): ";
    do{cin >> again;}while(again != 'y' && again != 'n');
    while(!hand.empty()) hand.pop();
    while(!dealer.empty()) dealer.pop();
  }
  if(money == 0) cout << "Your broke!" << endl;
  else cout << "\nYou walk away with $" << money << endl;
  cout << "Goodbye.\n" << endl;
  return 0;
}
