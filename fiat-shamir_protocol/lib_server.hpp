#pragma once
#include "lib.hpp"

using namespace std;

class server
{
	/* private variables, private keys */
private:
	/* p, q^ private, n = p * q - open */
	qq p, q, n, y, x;
	/* secert var s*/
	qq s;
	int e;

	vector<int> bit_e;
	/* x */
	vector<qq> proof;

public:
	server();
	qq send_n();
	qq v;
	qq send_s();
	int send_e();

	int create_bit_e(qq x);
	bool check_proof(qq x, qq y);

	~server();
};