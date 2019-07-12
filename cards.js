const club = '\u2663',
    heart = '\u2665',
    diamond = '\u2666',
    spade = '\u2660';

class Card {
    constructor(rank, suit) {
        this.rank = rank;
        this.suit = suit;
    }
}

function shuffle(deck) {
    deck.sort(() => Math.random() - 0.5);
}

const deck = []

for (i = 0; i < 13; i++) deck.push(new Card(i + 2, club));
for (j = 0; j < 13; j++) deck.push(new Card(j + 2, heart));
for (k = 0; k < 13; k++) deck.push(new Card(k + 2, diamond));
for (l = 0; l < 13; l++) deck.push(new Card(l + 2, spade));
shuffle(deck);

class Player {
    constructor(name) {
        this.name = name;
        this.money = 100;
        this.hand = [];
    }
}

p1 = new Player('Andy');
p2 = new Player('Ben');
p3 = new Player('Cam');
console.log(p1, p2, p3);

function dealer(i) {
    if (p2.hand[i].rank == 11) document.getElementById(i + 23).innerHTML = 'J';
    else if (p2.hand[i].rank == 12) document.getElementById(i + 23).innerHTML = 'Q';
    else if (p2.hand[i].rank == 13) document.getElementById(i + 23).innerHTML = 'K';
    else if (p2.hand[i].rank == 14) document.getElementById(i + 23).innerHTML = 'A';
    else document.getElementById(i + 23).innerHTML = p2.hand[i].rank;
    document.getElementById(i + 29).innerHTML = document.getElementById(i + 23).innerHTML
    document.getElementById(i + 26).innerHTML = p2.hand[i].suit;
    if (p2.hand[i].suit == heart || p2.hand[i].suit == diamond) document.getElementById(i + 20).style.color = "red";
    document.getElementById(i + 20).style.opacity = 1;
}

var cardCount = 0;

function start() {
    p2.hand.push(deck[deck.length - 1]);
    deck.pop();
    dealer(0);
}

start();

function show(i) {
    if (p1.hand[i].rank == 11) document.getElementById(i).innerHTML = 'J';
    else if (p1.hand[i].rank == 12) document.getElementById(i).innerHTML = 'Q';
    else if (p1.hand[i].rank == 13) document.getElementById(i).innerHTML = 'K';
    else if (p1.hand[i].rank == 14) document.getElementById(i).innerHTML = 'A';
    else document.getElementById(i).innerHTML = p1.hand[i].rank;
    document.getElementById(i + 5).innerHTML = p1.hand[i].suit;
    document.getElementById(i + 10).innerHTML = document.getElementById(i).innerHTML;
    if (p1.hand[i].suit == heart || p1.hand[i].suit == diamond) document.getElementById(i + 15).style.color = "red";
    document.getElementById(i + 15).style.opacity = 1;
    console.log(total(p1.hand));
}

function total(hand) {
    let total = 0;
    let ace = 0;
    for (i = 0; i < hand.length; i++) {
        if (hand[i].rank == 14) ace += 1;
        else if (hand[i].rank > 10) total += 10;
        else total += hand[i].rank;
    }
    if (ace == 1) {
        if (total < 11) total += 11;
        else total += 1;
    } else if (ace == 2) {
        if (total < 10) total += 12;
        else total += 2;
    } else if (ace > 2) total += ace;
    return total;
}

function hit() {
    p1.hand.push(deck[deck.length - 1]);
    deck.pop();
    show(cardCount);
    if (total(p1.hand) > 21) {
        document.getElementById("bust").style.opacity = 1;
        document.getElementById("Dealer").innerHTML = Number(document.getElementById("Dealer").innerHTML) + 1;
        document.getElementById("again").style.opacity = 1;

    }
    cardCount += 1;
}

function stay() {
    p2.hand.push(deck[deck.length - 1]);
    deck.pop();
    dealer(1);
    if (total(p2.hand) < 15) {
        p2.hand.push(deck[deck.length - 1]);
        deck.pop();
        dealer(2);
    }
    console.log("Dealer: " + total(p2.hand));

    if (total(p1.hand) > total(p2.hand) || total(p2.hand) > 21) {
        document.getElementById("win").style.opacity = 1;
        document.getElementById("Player").innerHTML = Number(document.getElementById("Player").innerHTML) + 1;
    } else if (total(p1.hand) == total(p2.hand)) {
        document.getElementById("tie").style.opacity = 1;
    } else {
        document.getElementById("lose").style.opacity = 1;
        document.getElementById("Dealer").innerHTML = Number(document.getElementById("Dealer").innerHTML) + 1;
    }
    document.getElementById("again").style.opacity = 1;
}

function again() {
    for (i = 0; i < p1.hand.length; i++) deck.push(p1.hand[i]);
    for (i = 0; i < p2.hand.length; i++) deck.push(p2.hand[i]);
    shuffle(deck);
    p1.hand = [];
    p2.hand = [];
    cardCount = 0;
    document.getElementById("win").style.opacity = 0;
    document.getElementById("lose").style.opacity = 0;
    document.getElementById("bust").style.opacity = 0;
    document.getElementById("again").style.opacity = 0;
    document.getElementById("tie").style.opacity = 0;
    let x = document.getElementsByClassName("card");
    for (i = 0; i < x.length; i++) x[i].style.opacity = 0;
    start();
}