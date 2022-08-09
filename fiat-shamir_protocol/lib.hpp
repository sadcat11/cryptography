#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <ctime>

#include <vector>
#include <random>
#include <dirent.h>
#include <inttypes.h>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <filesystem>

#include <chrono>
#include <thread>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/to_string.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

#include <boost/multiprecision/number.hpp>

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include <boost/algorithm/hex.hpp>

namespace bm = boost::multiprecision;
#define br boost::random

#define qq bm::cpp_int

#define uqq bm::uint1024_t
typedef unsigned short uint16;

#define ll long long
#define ull unsigned long long
typedef unsigned char uint8;

#define mexp (qq)1e30

using namespace std;

/* number generation */
qq gen_number(ull order);
qq getrand(qq min, qq max);
qq gen_prime(qq min, qq max);
bool is_prime(qq num);
qq gen_coprime(qq num);

/* fast modulo exponentiation and euclidean algorithm */
qq fast_exp_modul(qq a, qq b, qq x);
qq euc(qq *a, qq *b, qq *nod);
qq euc(qq a, qq b);

qq str_to_qq(string number);
/* string qq_to_str(qq number); */

qq gen_big_prime(uint16 bits);