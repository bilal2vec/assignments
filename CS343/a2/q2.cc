#include <uPRNG.h>

#include <iostream>
#include <vector>

using namespace std;

class Potato {
  PRNG prng;
  unsigned int max_ticks;
  unsigned int n_ticks;

 public:
  _Event Explode{};
  Potato(PRNG &prng, unsigned int maxTicks = 10)
      : prng(prng), max_ticks(maxTicks) {}
  void reset(unsigned int maxTicks = 10) {
    max_ticks = maxTicks;
    n_ticks = prng(1, max_ticks);
    cout << "  POTATO goes off after " << n_ticks << " tick";

    // Make sure to print "s" if n_ticks > 1
    if (n_ticks > 1) {
      cout << "s";
    }

    cout << endl;
  }
  void countdown() {
    n_ticks--;
    if (n_ticks <= 0) {
      _Throw Explode();
    }
  }
};

_Coroutine Player {
  _Event Terminate {
   public:
    Player & victim;  // delete player
    Terminate(Player & victim) : victim(victim) {}
  };
  _Event Election {
   public:
    Player *player;  // highest player id seen so far
    Election(Player * player) : player(player) {}
  };
  Player *partner[2];  // left and right player
  PRNG & prng;         // random number generator
  unsigned int id;     // player id
  Potato & potato;     // potato
  void main() {
    while (1) {
      try {
        // This while loop with a suspend in it is needed to prevent the umpire
        // or any other player that *might* become the umpire at some point in
        // the game from finishing execution before the game is over.
        _Enable { suspend(); }
        // When a potato goes off and a player raises Terminate at the umpire,
        // the umpire will catch it here.
      } catch (Terminate &e) {
        // If the player that raised Terminate is the umpire, then we run the
        // election
        if ((unsigned int)e.victim.getId() == id) {
          cout << "E " << id;
          Player *right = partner[1];
          _Resume Election(right) _At *right;
          right->vote();

          // The umpire will catch the election here, reset the potato, relink
          // the terminated player's partners, and then toss the potato again
        } else {
          Player *left = e.victim.partner[0];
          Player *right = e.victim.partner[1];

          left->partner[1] = right;
          right->partner[0] = left;

          potato.reset();

          // If the umpire is the only player left, then the umpire wins
          if ((id == (unsigned int)left->getId()) &&
              (id == (unsigned int)right->getId())) {
            cout << "U " << Player::umpire->getId() << " wins the Match!"
                 << endl;
          } else {
            cout << "U " << Player::umpire->getId();
            toss();
          }
        }
        // When the umpire raises Election at its right partner, its right
        // partner will catch it here, handle it, and raise Election at *its*
        // right partner until we reach the old umpire again
      } catch (Election &e) {
        // If we've reached the old umire again, we set the new umpire, Raise a
        // Terminate of the old umpire at the new umpire and call its terminate
        // method to terminate the old umpire. This counts as a toss of the
        // potato so we countdown the potato by one
        if (id == (unsigned int)umpire->getId()) {
          umpire = e.player;

          cout << " : umpire " << umpire->getId() << endl;

          potato.countdown();
          _Resume Terminate(*this) _At *umpire;
          umpire->terminate();
          break;
          // If we haven't reached the old umpire yet, we raise Election at our
          // right partner with the player with the highest id seen so far as
          // the player in the Election event and call its vote method to resume
          // it
        } else {
          cout << " -> " << id;
          Player *right = partner[1];

          if (id > (unsigned int)e.player->getId()) {
            _Resume Election(this) _At *right;
          } else {
            _Resume Election(e.player) _At *right;
          }

          right->vote();
        }
      }
    }
  }
  void vote() { resume(); }
  void terminate() { resume(); }

 public:
  static Player *umpire;  // current umpire

  Player(PRNG & prng, unsigned int id, Potato &potato)
      : prng(prng), id(id), potato(potato) {
    partner[0] = nullptr;
    partner[1] = nullptr;
  }
  void init(Player & lp, Player & rp) {
    partner[0] = &lp;
    partner[1] = &rp;

    resume();
  }
  int getId() { return id; }

  // We call countdown on the potato and call toss on a randomly chosen
  // left/right partner of the player. If the potato explodes, we catch the
  // exception and check if we've won the game yet, if not, we raise Terminate
  // at the umpire.
  void toss() {
    try {
      _Enable { potato.countdown(); }
      intmax_t direction = prng(0, 1);

      cout << " -> ";
      if (direction == 0) {
        cout << partner[0]->getId();
        partner[0]->toss();
      } else {
        cout << partner[1]->getId();
        partner[1]->toss();
      }
    } catch (Potato::Explode &e) {
      cout << " is eliminated" << endl;

      if ((id == (unsigned int)partner[0]->getId()) &&
          (id == (unsigned int)partner[1]->getId())) {
        cout << "U " << umpire->getId() << " wins the Match!" << endl;
      } else {
        // cout << "caught explode" << endl;
        _Resume Terminate(*this) _At *umpire;
        umpire->terminate();
        // cout << "BAD id " << id << endl;
      }
    }
  }
};

Player *Player::umpire = nullptr;

int main(int argc, char *argv[]) {
  intmax_t n_games = 5;
  intmax_t n_players = 0;
  intmax_t seed = getpid();

  // Reading in arguments
  struct cmd_error {};
  try {
    switch (argc) {
      case 4:
        if (strcmp(argv[3], "d") != 0) {
          seed = convert(argv[3]);
          if (seed <= 0) throw cmd_error();
        }
      case 3:
        if (strcmp(argv[2], "d") != 0) {
          n_players = convert(argv[2]);
          if (n_players < 2) throw cmd_error();
        }
      case 2:
        if (strcmp(argv[1], "d") != 0) {
          n_games = convert(argv[1]);
          if (n_games < 0) throw cmd_error();
        }
      case 1:
        break;
      default:
        throw cmd_error();
    }
  } catch (...) {
    cerr << "Usage: " << argv[0]
         << " [ games (>=0) | 'd' (default 5) [ players (>=2) | 'd' (random "
            "2-10) [ seed (>0) | 'd' (random) ] ] ]"
         << endl;
    exit(EXIT_FAILURE);
  }

  // Initialize the three PRNGs
  PRNG prng_1(seed);
  PRNG prng_2(seed);
  PRNG prng_3(seed);

  // Initalize the potato once and reuse it for all games
  Potato potato(prng_2);

  for (int i = 0; i < n_games; i++) {
    if (i > 0) cout << endl << endl;

    // For each game we choose a different random number of players between 2
    // and 10 if applicable
    intmax_t n_players_i = n_players;
    if (n_players_i == 0) n_players_i = prng_1(2, 10);

    cout << n_players_i << " players in the game" << endl;
    potato.reset();

    // Create the players
    vector<Player *> players;

    for (int j = 0; j < n_players_i; j++) {
      players.push_back(new Player(prng_3, (unsigned int)j, potato));
    }

    // Randomly choose a player to be the umpire by swapping it with the first
    // player
    intmax_t zero_idx = prng_1(1, n_players_i - 1);
    Player *tmp = players[zero_idx];
    players[zero_idx] = players[0];
    players[0] = tmp;

    // Initialize the players with their left and right partners
    for (int j = 0; j < n_players_i; j++) {
      players[j]->init(*players[(j - 1 + n_players_i) % n_players_i],
                       *players[(j + 1) % n_players_i]);
    }

    // Set the umpire
    Player::umpire = players[zero_idx];

    cout << "U " << Player::umpire->getId();

    // Start the game by tossing the potato to the umpire
    _Enable { Player::umpire->toss(); }

    // Clean up by deleting the players
    for (auto player : players) {
      delete player;
    }
  }
}
