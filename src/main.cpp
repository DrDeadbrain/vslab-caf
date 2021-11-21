// C++ standard library includes
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// CAF includes
#include "caf/all.hpp"
#include "caf/io/all.hpp"

                // Boost includes
                CAF_PUSH_WARNINGS
#ifdef CAF_GCC
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
                CAF_POP_WARNINGS

// Own includes
#include "int512_serialization.hpp"
#include "is_probable_prime.hpp"
#include "types.hpp"


#define SERVER_IP "localhost"
#define SERVER_PORT 5555
#define NUM_WORKER 4 //fix number of workers
#define MODE "worker" //"worker" //"server" //"client"


                using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

using boost::multiprecision::gcd;
using boost::multiprecision::int512_t;

using namespace caf;

/*TODO: MONITORING
 * 1. What to monitor?
 * ->if client dies
 *   -> inform all workers if they are working for that client
 *     -> worker müssen client id übergeben bekommen oder selber speichern dafür
 *     -> vielleicht andere/bessere option?
 * ->if worker dies ? keine ahnung ob das wichtig ist
 *      -> starten die sich selbst neu? wenn ja dann egal
 *         -> dann aber bei spawn in gruppe: nachricht um aufgabe zu erhalten ergänzen
 *      ->sonst neu starten???
 *         -> keine ahnung wie
 * -> if server dies
 *   -> inform clients and kill them?
 *   -> just kill everything directly?
 * 2. How to monitor?
 *    ->weitere instanz (wie server, client, worker) : Watchdog der alles mit monitor trackt und informiert
 *    -> in jeweiligen instanzen tracken:
 *          -> worker müssten monitor auf clients haben und entsprechend reagieren??
 * TODO: line 346 : block client : dunno how atm
 *
 * client übergibt seine actor_addr mit init_num_atom und mit dem next_num_atom
 * worker erhalten diese addr für das monitoring und wissen dadurch, ob sie die aktuelle aufgabe abbrechen können (gelicher client)
 * oder auch nicht (anderer client)
 * */



//big numbers inc
int512_t Z1 = 8806715679; // 3 * 29 * 29 * 71 * 211 * 233
int512_t Z2 = 0x826efbb5b4c665b9; // 9398726230209357241; // 443 * 503 * 997 * 1511 * 3541 * 7907
//int512_t Z3 = 0xc72af6a83cc2d3984fedbe6c1d15e542556941e7;
/*
Z3 = 1137047281562824484226171575219374004320812483047 =
7907 * 12391 * 12553 * 156007 * 191913031 * 4302407713 * 7177162612387
*/
//int512_t Z4 = 0x1aa0d675bd49341ccc03fff7170f29cd7048bf40430c22ced5a391d015d19677bde78a7b95b5d59b6b26678238fa7;
/*
Z4 = 1000602106143806596478722974273666950903906112131794745457338659266842446985022076792112309173975243506969710503
   =
   10657 * 11657 * 13264529 * 10052678938039 * 2305843009213693951 *
   (26196077648981785233796902948145280025374347621094198197324282087) =
*/

int512_t task = 0;



namespace {

struct config : actor_system_config {
  string host = SERVER_IP;
  uint16_t port = SERVER_PORT;
  size_t num_workers = NUM_WORKER;
  string mode = MODE;
  config() {
    opt_group{custom_options_, "global"}
      .add(host, "host,H", "server host (ignored in server mode)")
      .add(port, "port,p", "port")
      .add(num_workers, "num-workers,w", "number of workers (in worker mode)")
      .add(mode, "mode,m", "one of 'server', 'worker' or 'client'");
  }
};

/**
 * Pollard rho helpers
 * and pollard func itself
 */
int512_t gcd(int512_t u, int512_t v) {
  while (v != 0) {
    int512_t r = u % v;
    u = v;
    v = r;
  }
  return u;
}

int512_t modular_pow(int512_t base, int512_t exponent, int512_t modulus) {
  /*init result*/
  int512_t result = 1;

  while (exponent > 0) {
    //if y is odd, multiply base with result
    if (exponent & 1)
      result = (result * base) % modulus;
    exponent = exponent >> 1;
    //base = base * base
    base = (base * base) % modulus;
  }
  return result;
}

int512_t pRho(int512_t N) {
  //init rnd seed
  std::mt19937 mt_rand(time(0));

  //no prime divisor for 1
  if (N == 1) return N;

  //even number means one of the divisors is 2
  if (N % 2 == 0) return int512_t(2);

  //we will pick from range [2, N)
  int512_t x = (mt_rand() % (N-2)) + 2;
  int512_t y = x;

  /*
      * the constant in f(x)
      * algorithm can be re-run with a different c
      * if it throws failure for a composite
      **/
  int512_t c = (mt_rand() % (N-1)) + 1;

  //init candidate divisor (or result)
  int512_t d = int512_t(1);

  //until prime factor isn't obtained|| if n is prime, return n
  while (d == int512_t(1)) {
    /* Tortoise move: x(i+1) = f(x(i)) */
    x = (modular_pow(x, int512_t(2), N) + c + N) % N;

    /* Hare move: y(i+1) = f(f(y(i))) */
    y = (modular_pow(y, int512_t(2), N) + c + N) % N;
    y = (modular_pow(y, int512_t(2), N) + c + N) % N;

    /* check gcd of |x-y| and N */
    d = gcd(abs(x - y), N);

    /* retry if the algo fails to find prime factor with chosen x and c */
    if (d == N) return pRho(N);
  }
  return d;
}



// -- CLIENT -------------------------------------------------------------------

// Client state, keep track of factors, time, etc.
struct client_state {
  // The joined group.
  group grp;
  //client numbers

  vector<int512_t> clientNumbers;

  //problems for the workers /factors
  std::map<int512_t, int512_t> problems;


  //timings

  //begin of factorization
  clock_t begin;
  //end of factorization
  clock_t end;
  double usedCPUTime;

  //begin of factorization
  std::chrono::steady_clock::time_point beginW;
  //end of factorization
  std::chrono::steady_clock::time_point endW;
  double usedWallTime;

  //the clients' address
  caf::actor_addr address;

  //for check if client has already work getting done
  bool inProgress;
};

behavior client(stateful_actor<client_state>* self, caf::group grp) {
  // Join group and save it to send messages later.
  self->join(grp);
  self->state.grp = grp;
  self->state.address = self->address();
  self->state.inProgress = false;

  string input;
  cout << "Enter number to factorize: " << endl;
  std::getline(std::cin, input);
  task = boost::lexical_cast<int512_t>(input);
  //while even number
  while (task % 2 == 2) {
    cout << "ERROR: Even number provided" << endl
         << "Insert number to factorize: " << endl << endl;
    std::getline(std::cin, input);
    task = boost::lexical_cast<int512_t>(input);
  }
  if (task == -1) {
    task = Z1;
  }
  if (task == -2) {
    task = Z2;
  }
  if (task == -3) {
    task = Z1;
  }
  if (task == -4) {
    task = Z2;
  }
  self->state.beginW = std::chrono::steady_clock::now();
  //TODO: worker anfragen
  self->send(grp, need_worker_atom_v, self->address());


  return {
    //worker available for client
    [=](worker_available_atom, caf::actor_addr address){
      if(self->state.inProgress == false){
        //if worker is available for THIS client
        if(self->address() == address){
          self->state.inProgress = true;
          self->send(grp, init_num_atom_v, task, self->state.address);
        }
        else {
          //TODO: wait
          sleep(3);
          //TODO worker anfragen
          self->send(grp, need_worker_atom_v, self->address());
        }
      }
      //inProgress == true
      else {
        cout << "already in progress" << endl;
      }

    },

    [=](need_worker_atom, caf::actor_addr address){
      if(self->address() == address){
        cout << "applied for a worker" << endl;
      }
    },

    //new job from user
    [=](init_num_atom, int512_t task) {
      //haven't gotten job yet
      if (!self->state.problems.count(task)) {
        self->state.problems.insert(std::pair<int512_t, int512_t>(task, 0));

        //check if number is prime or 2
        if (is_probable_prime(task) || task == 2) {
          self->send(grp, done_msg_atom_v, task, self->address());
        } else {
          self->send(grp, new_num_atom_v, task, self->state.address);
        }
      }
    },
    [=](result_atom, int512_t factor, int512_t problem, double time, caf::actor_addr address) {
      //only work if own business
      if(self->address() == address){
        int512_t currentProblem = problem;
        self->state.usedCPUTime = self->state.usedCPUTime + time;

        if (self->state.problems.count(currentProblem) > 0) {
          int512_t factorForProblems = self->state.problems.at(currentProblem);
          if (factorForProblems == 0) {
            self->state.problems.at(currentProblem) = factor;
            currentProblem = currentProblem / factor;

            //check if done
            if (is_probable_prime(currentProblem) || currentProblem == 2) {
              self->send(grp, done_msg_atom_v, currentProblem, self->address());
            } else {
              self->send(grp, client_num_atom_v, currentProblem, self->address());
            }
          }
        }
      }
    },

    [=](done_msg_atom, int512_t number, caf::actor_addr address) {
      //only work if message from self
      if(self->address() == address){
        //we are done
        self->state.endW = std::chrono::steady_clock::now();
        self->state.problems.at(number) = number;
        std::cout << "--------------------- Found Answer ---------------------" << endl << endl;
        cout << "Found Factors: ";
        for (auto elem: self->state.problems) {
          cout << elem.second << " * ";
        }
        self->state.usedWallTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
                                     self->state.endW - self->state.beginW).count();
        cout << endl << "CPU time used: " << (self->state.usedCPUTime / 1000000000) << " s" << endl;
        cout << "Wall clock time used: " << (self->state.usedWallTime / 1000000000) << " s" << endl;
        cout << endl << "------------------ ------------------ ------------------" << endl << endl;
        self->state.problems.clear();
        self->state.usedCPUTime = 0;
        self->state.usedWallTime = 0;
        self->send(grp, block_false_atom_v, self->address());
      }
    },

    [=](new_num_atom, int512_t task, caf::actor_addr address){
      if(self->address() == address){
        cout << "sent new task" << endl;
      }
    },

    [=](client_num_atom, int512_t currentproblem, caf::actor_addr address){
      if(self->address() == address){
        cout << "problem not yet solved, send new task" << endl;
      }
    },

    [=](block_false_atom, caf::actor_addr address){
      if(self->address() == address){
        self->state.inProgress = false;
        cout << "problem solved, unblock worker" << endl;
      }
    }
  };
}

void run_client(actor_system& sys, const config& cfg) {
  int512_t number;
  if (auto eg = sys.middleman().remote_group("vslab", cfg.host, cfg.port)) {
    auto grp = *eg;
    sys.spawn(client, grp);
  } else {
    cerr << "error: " << caf::to_string(eg.error()) << '\n';
  }
}

// -- WORKER -------------------------------------------------------------------

// State specific to each worker.
struct worker_state {
  // The joined group.
  group grp;

  //employer is the client the worker is currently working for
  caf::actor_addr employer;

};

behavior worker(stateful_actor<worker_state>* self, caf::group grp) {
  // Join group and save it to send messages later.
  self->join(grp);
  self->state.grp = grp;
  self->state.employer = nullptr;

  return {
    [=](need_worker_atom, caf::actor_addr address){
      //check if worker is employed
      if(self->state.employer == nullptr){
        //employ applying client
        self->state.employer = address;
        //notify client(s)
        cout << "not yet employed: " << to_string(address) << endl;
        self->send(grp, worker_available_atom_v, address);
      }
      else {
        //notify client(s)
        cout << "already employed" << to_string(self->state.employer) << endl;
        self->send(grp, worker_available_atom_v, self->state.employer);

      }
    },

    [=](worker_available_atom, caf::actor_addr address){
      cout << "working for: " << to_string(address) << endl;
    },

    [=](init_num_atom, int512_t task, caf::actor_addr address) {
      cout << "whatever" << endl;
    },

    [=](new_num_atom, int512_t N, caf::actor_addr address) {

      //if client is employer -> MONITORING
      if(self->state.employer == address){
        self->monitor(address);
      }
      //if worker gets down message from client -> get unemployed/available
      self->set_down_handler(
        [=](const down_msg& dm) {
          self->state.employer = nullptr;
        });

      //work only if employed
      if(self->state.employer != nullptr){
        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::steady_clock::now();

        int512_t fac = pRho(N);

        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::steady_clock::now();
        double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

        self->send(grp, result_atom_v, fac, N, cpu_time_used, self->state.employer);
      }
    },

    [=](client_num_atom, int512_t N) {
      std::clock_t c_start = std::clock();
      auto t_start = std::chrono::steady_clock::now();

      int512_t fac = pRho(N);

      std::clock_t c_end = std::clock();
      auto t_end = std::chrono::steady_clock::now();
      double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

      self->send(grp, result_atom_v, fac, N, cpu_time_used, self->state.employer);
    },

    [=](block_false_atom, caf::actor_addr address) {
      //if unblock message from own employer -> unemploy
      if(self->state.employer == address){
        self->state.employer = nullptr;
      }
    },

    [=](done_msg_atom, int512_t number, caf::actor_addr address){
      if(self->state.employer == address){
        cout << "seems we're done" << endl;
      }
    },

    [=](result_atom, int512_t factor, int512_t problem, double time, caf::actor_addr address){
      cout << "whatever" << endl;
    }
  };
}

void run_worker(actor_system& sys, const config& cfg) {
  if (auto eg = sys.middleman().remote_group("vslab", cfg.host, cfg.port)) {
    auto grp = *eg;
    // TODO: Spawn workers, e.g:
    // sys.spawn(worker, grp);
    for (int i = 0; i < NUM_WORKER; i++) {
      sys.spawn(worker, grp);
    }
  } else {
    cerr << "error: " << caf::to_string(eg.error()) << '\n';
  }
  sys.await_all_actors_done();
}

// -- SERVER -------------------------------------------------------------------

void run_server(actor_system& sys, const config& cfg) {
  if (auto port = sys.middleman().publish_local_groups(cfg.port))
    cout << "published local groups at port " << *port << '\n';
  else
    cerr << "error: " << caf::to_string(port.error()) << '\n';
  cout << "press any key to exit" << std::endl;
  getc(stdin);
}

// -- MAIN ---------------------------------------------------------------------

// dispatches to run_* function depending on selected mode
void caf_main(actor_system& sys, const config& cfg) {
  // Dispatch to function based on mode.
  using map_t = unordered_map<string, void (*)(actor_system&, const config&)>;
  map_t modes{
    {"server", run_server},
    {"worker", run_worker},
    {"client", run_client},
  };
  auto i = modes.find(cfg.mode);
  if (i != modes.end())
    (i->second)(sys, cfg);
  else
    cerr << "*** invalid mode specified" << endl;
}
}

 // namespace

CAF_MAIN(io::middleman, id_block::vslab)
