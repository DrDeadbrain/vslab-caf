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
#define SERVER_PORT 5000
#define NUM_WORKER 4 //fix number of workers
#define MODE "client" //"worker" //"server" //"client"


using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

using boost::multiprecision::gcd;
using boost::multiprecision::int512_t;
using boost::multiprecision::uint512_t;
using boost::multiprecision::int1024_t;
using boost::multiprecision::uint1024_t;

using namespace caf;

/*TODO: MONITORING
 * 1. What to monitor?
 * -> if server dies
 *   -> inform clients and kill them?
 *   -> just kill everything directly?
 * 2. How to monitor?
 *    ->weitere instanz (wie server, client, worker) : Watchdog der alles mit monitor trackt und informiert
 *    -> in jeweiligen instanzen tracken:
 *          -> worker müssten monitor auf clients haben und entsprechend reagieren??
 * TODO: line 346 : block client : dunno how atm
 * TODO: catch all messages in every actor because of errors
 * */

static int numPRho = 0;
int512_t eCode = -99;
//big numbers inc
int512_t Z1 = 8806715679; // 3 * 29 * 29 * 71 * 211 * 233
int512_t Z2 = 9398726230209357241; // 443 * 503 * 997 * 1511 * 3541 * 7907 //0x826efbb5b4c665b9;
//int512_t Z3 = 0xc72af6a83cc2d3984fedbe6c1d15e542556941e7;

//uint1024_t Z3 = 1137047281562824484226171575219374004320812483047;
/*7907 * 12391 * 12553 * 156007 * 191913031 * 4302407713 * 7177162612387*/

//int512_t Z4 = 0x1aa0d675bd49341ccc03fff7170f29cd7048bf40430c22ced5a391d015d19677bde78a7b95b5d59b6b26678238fa7;

//Z4 = 1000602106143806596478722974273666950903906112131794745457338659266842446985022076792112309173975243506969710503;
/*  =
  10657 * 11657 * 13264529 * 10052678938039 * 2305843009213693951 *
  (26196077648981785233796902948145280025374347621094198197324282087) =*/





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

    struct rhoNumber {
        int512_t number;
        int iterationCount;
        int512_t p;
    };

    rhoNumber pRho(int512_t N) {
        int iteration;
        struct rhoNumber retNum;
        // init rnd seed
        std::mt19937 mt_rand(time(0));

        // no prime divisor for 1
        if (N == 1) {
            retNum.iterationCount = 0;
            retNum.number = int512_t(1);
            return retNum;
        }

        // even number means one of the divisors is 2
        if (N % 2 == 0) {
            retNum.iterationCount = 0;
            retNum.number = int512_t(2);
            return retNum;
        }

        // we will pick from range [1, N)
        int512_t x = (mt_rand() % N + 1);
        int512_t y = x;

        /*
            * the constant in f(x)
            * algorithm can be re-run with a different c
            * if it throws failure for a composite
            **/
        int512_t c = (mt_rand() % (N - 1)) + 1;

        // init difference
        int512_t d = int512_t(0);

        // init candidate divisor (or result)
        int512_t p = 1;

        /**
         * until prime factor isn't obtained
         */
        int numberOfRuns = 0;
        int512_t M = (int512_t) sqrt(sqrt(N));
        M = (int512_t) (M * 118);
        M = (int512_t) (M / 100);

        do {
            // if running longer than Middle M 1.18 * sqrt(sqrt(N)): try with new random number
            if (numberOfRuns > M) {
                cerr << "Pollard is running longer than Middle M 1.18 * sqrt(sqrt(N)):"
                     << endl;
                return pRho(N);
            }
            /* Tortoise move: x(i+1) = f(x(i)) */
            x = (x * x + c) % N;

            /* Hare Move: y(i+1) = f(f(y(i))) */
            y = (y * y + c) % N;
            y = (y * y + c) % N;

            /* Difference between (y - x) % N */
            d = (y - x) % N;

            /* Check GCD of |x-y| and N */
            p = gcd(d, N);

            /* make divisor positive */
            if (p < 0) {
                p *= -1;
            }
            numberOfRuns++;
            numPRho++;
        } while (p == 1);

        /* retry if algo fails to find prime number with chosen x and c */
        if (p != N) {
            struct rhoNumber retNum;
            retNum.number = p;
            retNum.iterationCount = numPRho;
            numPRho = 0;
            retNum.p = p;
            return retNum;
        } else {
            retNum.p = eCode;
            return retNum;
        }
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
        int iterationCount;

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
    };

    behavior client(stateful_actor<client_state>* self, caf::group grp) {
        // Join group and save it to send messages later.
        self->join(grp);
        self->state.grp = grp;

        string input;
        cout << "Enter number to factorize: " << endl;
        std::getline(std::cin, input);
        int512_t task = boost::lexical_cast<int512_t>(input);

        if (task == -1) {
            task = Z1;
        }
        if (task == -2) {
            task = Z2;
        }
        /*
        if (task == -3) {
            task = Z3;
        }

        if (task == -4) {
            task = Z2;
        }*/
        self->state.beginW = std::chrono::steady_clock::now();
        self->send(grp, init_num_atom_v, task);


        return {
                //new job from user
                [=](init_num_atom, int512_t task) {
                    //haven't gotten job yet
                    if (!self->state.problems.count(task)) {
                        self->state.problems.insert(std::pair<int512_t, int512_t>(task, 0));

                        //check if number is prime or 2
                        if (is_probable_prime(task) || task == 2) {
                            self->send(grp, done_msg_atom_v, task);
                        } else {
                            self->send(grp, new_num_atom_v, task, self->address());
                        }
                    }
                },
          
                [=](result_atom, int512_t factor, int512_t problem, double time, int iteration) {
                    int512_t currentProblem = problem;
                    self->state.usedCPUTime = self->state.usedCPUTime + time;
                    self->state.iterationCount = self->state.iterationCount + iteration;

                    if (self->state.problems.count(currentProblem) > 0) {
                        int512_t factorForProblems = self->state.problems.at(currentProblem);
                        if (factorForProblems == 0) {
                            self->state.problems.at(currentProblem) = factor;
                            currentProblem = currentProblem / factor;
                            if(! self->state.problems.count(currentProblem)){
                                self->state.problems.insert(std::pair <int512_t, int512_t> (currentProblem, 0));
                            }
                            //check if done
                            if (is_probable_prime(currentProblem) || currentProblem == 2) {
                                self->send(grp, done_msg_atom_v, currentProblem);
                            } else {
                                self->send(grp, client_num_atom_v, currentProblem);
                            }
                        }
                    }
                },

                [=](done_msg_atom, int512_t number) {
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
                    cout << "Iterations: " << (self->state.iterationCount) << endl;
                    cout << endl << "------------------ ------------------ ------------------" << endl << endl;
                    self->state.problems.clear();
                    self->state.usedCPUTime = 0;
                    self->state.usedWallTime = 0;
                    //self->send(grp, block_false_atom_v);
                },

                [=](const group_down_msg&){
                    std::cerr << "FATAL: server is down.\n Press ENTER to exit.\n";
                    fclose(stdin);
                    self->quit();
                },

                //catch all other messages to prevent error that kills actor
                [=](new_num_atom, int512_t N, caf::actor_addr address) {},
                [=](client_num_atom, int512_t N) {}
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

        //blocked by a client
        bool blocked;

        rhoNumber rhoNum;

        caf::actor_addr address;
    };

    behavior worker(stateful_actor<worker_state>* self, caf::group grp) {
        // Join group and save it to send messages later.
        self->join(grp);
        self->state.grp = grp;
        self->state.blocked = false;
        self->state.address = nullptr;

        return {
                [=](new_num_atom, int512_t N, caf::actor_addr address) {
                    //TODO:block if other client : Clientid speichern und abfragen, wenn bearbeitung done -> clientID = 0 lokal

                    if(((self->state.address != address) and (self->state.blocked == false)) ||
                       ((self->state.address == address))) {

                        self->state.address = address;
                        self->state.blocked = true;

                        cout << "\nNEW_NUM SUCCESSFUL" << endl;
                        cout << to_string(self->state.address) + " registered as current client" << endl;
                        cout << "Blocked: " << self->state.blocked << endl;


                        std::clock_t c_start = std::clock();
                        auto t_start = std::chrono::steady_clock::now();

                        if(!(self->mailbox().empty())) {
                            cout << "Worker got new Task, cancel current calc" << endl;
                            return;
                        }

                        self->state.rhoNum = pRho(N);

                        if(!(self->mailbox().empty())) {
                            cout << "Worker got new Task, cancel current calc" << endl;
                            return;
                        }

                        while(self->state.rhoNum.p == eCode) {

                            if(is_probable_prime(self->state.rhoNum.number)) {
                                std::clock_t c_end = std::clock();
                                auto t_end = std::chrono::steady_clock::now();
                                double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

                                self->send(grp, result_atom_v, self->state.rhoNum.number, N, cpu_time_used, self->state.rhoNum.iterationCount);
                            }
                            self->state.rhoNum = pRho(self->state.rhoNum.number);

                            if(!(self->mailbox().empty())) {
                                cout << "Worker got new Task, cancel current calc" << endl;
                                return;
                            }
                        }
                        std::clock_t c_end = std::clock();
                        auto t_end = std::chrono::steady_clock::now();
                        double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

                        self->send(grp, result_atom_v, self->state.rhoNum.number, N, cpu_time_used, self->state.rhoNum.iterationCount);
                    }
                    else {
                        cout << "\nNEW_NUM FAILED" << endl;
                        cout << to_string(address) << " is blocked by " << to_string(self->state.address) << endl;
                        cout << "Blocked: " << self->state.blocked << endl;
                    }
                },

                [=](client_num_atom, int512_t N) {

                    std::clock_t c_start = std::clock();
                    auto t_start = std::chrono::steady_clock::now();

                    self->state.rhoNum = pRho(N);

                    if(self->state.rhoNum.p == eCode) {
                        if(is_probable_prime(self->state.rhoNum.number)) {
                            std::clock_t c_end = std::clock();
                            auto t_end = std::chrono::steady_clock::now();
                            double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

                            self->send(grp, result_atom_v, self->state.rhoNum.number, N, cpu_time_used, self->state.rhoNum.iterationCount);
                        }
                        self->state.rhoNum = pRho(self->state.rhoNum.number);
                    }
                    std::clock_t c_end = std::clock();
                    auto t_end = std::chrono::steady_clock::now();
                    double cpu_time_used = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end-t_start).count();

                    self->send(grp, result_atom_v, self->state.rhoNum.number, N, cpu_time_used, self->state.rhoNum.iterationCount);
                },

                [=](const group_down_msg&){
                    std::cerr << "FATAL: server is down!\nPress ENTER to exit.\n";
                    self->quit();
                },

                //catch all messages to counter error that kills actor
                [=](init_num_atom, int512_t task) {},
                [=](result_atom, int512_t factor, int512_t problem, double time, int iteration){},

                [=](done_msg_atom, int512_t number) {
                    //unblock if done
                    self->state.blocked = false;

                    cout << "\nDONE_MSG" << endl;
                    cout << to_string(self->state.address) << " is done." << endl;
                    cout << "Blocked: " << self->state.blocked << endl;
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
