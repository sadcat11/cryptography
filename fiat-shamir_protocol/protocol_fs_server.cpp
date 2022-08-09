#include "lib.hpp"
#include "lib_server.hpp"

#define INFO 0

using namespace std;
using std::chrono::system_clock;

server::server()
{
	string strr = "";
#if INFO
	cout << "p q gen\n";
#endif
	ifstream file;
	/* check if have created n */
	file.open("server/n.txt");
	if (file.is_open())
		getline(file, strr);
	file.close();
	/* if n is in file: use it
		else: gen new p & q, calc n = p * q and write n in file */
	if (strr.empty())
	{
		do
		{
			p = gen_big_prime(256);
			q = gen_big_prime(256);
			//p = 683;
			//q = 811;
		} while (p == q);
		n = p * q;
		ofstream file;
		file.open("server/n.txt");
		file << n << "\n";
		file.close();
	}
	else
	{
		n = str_to_qq(strr);
	}
}

qq server::send_n()
{
	return n;
}

int server::send_e()
{
	return e;
}

qq server::send_s()
{
	return s;
}

int server::create_bit_e(const qq x)
{
	e = rand() % 2;
	bit_e.push_back(e);
	proof.push_back(x);
	return e;
}

bool server::check_proof(const qq y, const qq x)
{
	vector<qq>::iterator it = find(proof.begin(), proof.end(), x);
	ull ind = std::distance(proof.begin(), it);
	if (y == 0)
		return false;
	if (fast_exp_modul(y, n, 2) == fast_exp_modul(x * pow(v, bit_e[ind]), n, 1))
		return true;
	return false;
}

server::~server()
{
}