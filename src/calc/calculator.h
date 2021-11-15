//
// Created by baal on 15.11.21.
//
#ifndef VSLAB_CALCULATOR_H
#define VSLAB_CALCULATOR_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

#include "include/is_probable_prime.hpp"

using namespace boost::multiprecision;
using namespace std;

class Calculator
{
  public:
    uint512_t rho(uint512_t n, uint512_t c);
    bool isPrime(uint512_t a);

  private:
    uint512_t _gcd(uint512_t u, uint512_t v);
};
#endif // VSLAB_CALCULATOR_H
