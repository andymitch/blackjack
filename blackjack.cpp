#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> //shuffle
#include <random> //random generator
#include <string> //stoi
#include <cctype> //isdigit
#include <fstream> //high score
#include <iomanip> //set width

using namespace std;


//CARD STRUCT
/******************************************************************************/
enum Suit{club, diamond, heart, spade};
enum Rank{Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};

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

//DECK FUNCTIONS
/******************************************************************************/

stack<Card> getDeck(){
  cout << "\nShuffling new deck..\n";
  vector<Card> _deck;
  Rank rank = Two; //lowest rank
  Suit suit = club; //lowest suit
  for(int i = 0; i < 4; i++){ //get deck of cards sequentially
      for(int j = 0; j < 13; j++){
          _deck.push_back(Card(rank, suit));
          ++rank;
      }
      ++suit;
      rank = Two;
  }

  shuffle(_deck.begin(), _deck.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count())); //shuffle deck

  stack<Card> deck;
  while(!_deck.empty()){ //convert vector to stack
    deck.push(_deck.back());
    _deck.pop_back();
  }

  return deck;
}


void deal(stack<Card>& hand, stack<Card>& deck){
  if(deck.empty()) deck = getDeck(); //when deck is empty, get a new one

  hand.push(deck.top()); //deal card from deck
  deck.pop();
}

//GAME LOGIC
/******************************************************************************/

int value(stack<Card> hand){
  int total = 0, value, ace = 0; //value = current card value, total = sum of values, ace = number of aces

  while(!hand.empty()){
    value = hand.top().rank; //get card value
    hand.pop();

    if(value > 10 && value < 14) total += 10; //if face card
    else if(value == 14) ace++; //if ace
    else total += value; //if number card
  }
  
  if(ace == 1){
    if(total < 11) total += 11;
    else total++;
  }else if(ace == 2){
    if(total < 10) total += 12;
    else total += 2;
  }else if(ace > 2) total += ace;

  return total;
}


bool bust(stack<Card> hand){
  return(value(hand) > 21); //if hand value is over 21
}


void print(stack<Card> hand, stack<Card> dealer){
  cout << "\033[2J\033[1;1H"; //clear screen

  cout << "\nDealer: " << dealer.top(); //print one dealer card
  cout << "[  ]" << endl;

  cout << "   You: ";
  while(!hand.empty()){ //print all player cards
    cout << hand.top();
    hand.pop();
  }
  cout << endl;
}


bool result(stack<Card> hand, stack<Card> dealer){
  cout << "\033[2J\033[1;1H"; //clear screen

  bool win = false, _bust = false;
  if(bust(hand)) _bust = true; //player busts

  int vhand = value(hand); //get card values
  int vdeal = value(dealer); //get card values

  if(!bust(hand) && (vhand >= vdeal)) win = true; //player wins

  cout << "\nDealer: " << dealer.top();
  dealer.pop();
  cout << dealer.top(); //print both dealer cards

  cout << "\n   You: ";
  while(!hand.empty()){ //print all player cards
    cout << hand.top();
    hand.pop();
  }

  cout << "\n\n";
  if(_bust) cout << "Bust! "; //prints if bust

  if(win){
    cout << "You Win!\n";
    return true; //returns true if player wins
  }else{
    cout << "You Lose.\n";
    return false;
  }
}


bool bet(int& money, int amount){
  if(amount > money){
    cout << "You can't cover that bet." << endl;
    return false; //too much amount

  }else if(amount < 1){
    cout << "You have to bet something." << endl;
    return false; //no positive amount

  }else{
    money -= amount; //takes amount from money
    return true;
  }
}


void title(){
  cout << "\033[2J\033[1;1H"; //clear screen
  cout << "************************************************************************************\n";
  cout << "WELCOME TO BLACKJACK!!!\n\n";
  cout << "The objective is to get closer to 21 than the dealer without busting (going over 21)\n";
  cout << " - Number cards retain their value.\n";
  cout << " - Face cards are worth 10.\n";
  cout << " - Aces are worth either 1 or 11.\n";
  cout << "************************************************************************************\n";
}

//MAIN
/******************************************************************************/

bool isnum(string number){ //if string is a number
  bool is = true;
  for(int i = 0; i < number.length(); i++) if(!isdigit(number[i])) is = false;
  return is;
}

void quit(int money){
  int scores[5]{0,0,0,0,0};
  string names[5]{"*","*","*","*","*"};
  int score = money;
  string name = " ";

  if(money != 0) cout << "\nYou walk away with $" << money << endl;

  //get high scores
  ifstream infile;
  infile.open("highscore.txt");
  int i = 0;
  while(infile){
    infile >> scores[i];
    infile >> names[i];
    i++;
  }
  infile.close();

  //sort high scores
  bool newHigh = false;
  for(int i = 0; i < 5; i++){
    if(score > scores[i]){
      newHigh = true;
      int s = score;
      string n = name;
      score = scores[i];
      name = names[i];
      scores[i] = s;
      names[i] = n;
    }
  }

  //get name for high score
  for(int i = 0; i < 5; i++){
    if(scores[i] < money){
      cout << "\nNew High Score!!\n\nWhat's your name?: ";
      cin >> name;
      names[i-1] = name;
      cout << "\n\n";
      break;
    }
  }

  //print high scores
  cout << "\n\n************************************************************************************\n";
  cout << "\t- High Scores - \n";
  cout << "************************************************************************************\n";
  for(int i = 0; i < 5; i++){
    cout << i+1 << ".\t";
    if(scores[i] > 0) cout << setw(6) << left << scores[i];
    else cout << setw(6) << right << " ";
    if(names[i] != "*") cout << "\t" << names[i] << endl;
    else cout << "\t" << endl;
  }
  cout << "************************************************************************************\n";

  //save high scores
  if(newHigh){
    ofstream outfile;
    outfile.open("highscore.txt");
    for(int i = 0; i < 5; i++){
      outfile << scores[i] << " ";
      outfile << names[i] << " ";
    }
    outfile.close();
  }
}

bool lose(){
  char c;
  cout << "\nYour broke!\nWould you like to play again? (y/n): ";
  do{
    cin >> c;
  }while(c != 'y' && c != 'n');
  if(c == 'y') return false;
  else return true;
}

int main(){
  title();
  while(true){
    int money = 20, pot;
    string spot; //string pot
    stack<Card> hand, dealer, deck;
    char c = 'h', again = 'y'; //choice and yes/no

    while(money > 0){
      deal(hand, deck);
      deal(hand, deck);
      deal(dealer, deck);
      deal(dealer, deck); //deals new cards

      cout << "\nWhat do you want to bet? Or (q) to quit.\ncurrent balance: $" << money << endl;
      do{
        cin >> spot;
        if(spot == "q"){ //quit game
          quit(money);
          return 0;
        }
        else while(!isnum(spot)){
          if(spot == "q"){ //quit game
            quit(money);
            return 0;
          }
          cin >> spot; //get bet until integer
        }
        pot = stoi(spot); //convert string to int
      }while(!bet(money, pot)); //get bet until valid amount

      while(!bust(hand)){ //while player hasn't busted
        print(hand, dealer); //show cards
        cout << "\nHit or Stay? (h/s): ";
        cin >> c;
        switch(c){
          case 'h': deal(hand, deck); //hit
            break;
          case 's': if(result(hand, dealer)) money += (2*pot); //stay. If result() returns true, player wins the bet.
            break;
          default: cout << "Invalid." << endl; // neither 'h' or 's', repeat while loop
        }
        if(c == 's') break; //if stay, break while loop
      }
      if(result(hand, dealer)){} //empty if() after while loop to print result assuming that player busted

      while(!hand.empty()) hand.pop(); //clear cards
      while(!dealer.empty()) dealer.pop(); //clear cards
    }

    if(lose()){ //game over, player is broke
      quit(money);
      cout << "\nGoodbye.\n" << endl;
      return 0;
    }
  }
}
