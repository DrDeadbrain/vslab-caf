//
// Created by baal on 15.11.21.
//

#include <cmath>
#include <iostream>
#include <fstream>

#include "calculator.h"

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

/**
 * uses pollard-rho method to calc prime factors for given uint512_t number.
 * gives back prime factor or given uint512_t if it is a prime
 * -1 doesn't mean it has no prime factor -> if given is no prime try again
 *
 * @param n number to find prime factors for
 * @param c workes choose of random sequence
 *
 * @return uint512_t prime factor of param or param itself
 */
uint512_t Calculator::rho(uint512_t n, uint512_t c) {
  if (c == 0) {
    cout << "c is not allowed to be 0 or -2";
    return -1;
  }

  uint512_t x = abs(rand()) % n;
  uint512_t y = x;
  uint512_t p = 1;
  uint512_t d = 0;

  int increment_count = 0;
  do {
    x = (((uint512_t)(boost::multiprecision::pow(x, 2))) + c) % n;
    y = (((uint512_t)(boost::multiprecision::pow(y, 2))) + c) % n;
    y = (((uint512_t)(boost::multiprecision::pow(y, 2))) + c) % n;
    d = boost::multiprecision::abs(x - y) % n;
    p = gcd(d, n);
    increment_count++;
  } while (p == 1);

  if (p != n) {return p;}
}

/**
 * isPrime method
 * checks if the given number is a prime number using the is_probable_prime
 * method
 * ->uses miller rabin test
 *
 * @param uint512_t u
 *
 * @return uint512_t v
*/
bool Calculator::isPrime(uint512_t a) {
  return is_probable_prime(a);
}

/**
 * gcd
 * searches biggest common divider for 2 numbers
 * found on : https://www.geeksforgeeks.org/c-program-find-gcd-hcf-two-numbers/
 * @param u
 * @param v
 * @return
 */
uint512_t Calculator::_gcd(uint512_t u, uint512_t v) {
  if (v == 0)
    return u;
  return _gcd(v, u % v);
}