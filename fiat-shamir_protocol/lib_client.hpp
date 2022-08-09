#pragma once
#include "lib.hpp"

using namespace std;

class client
{
private:
	qq rnd;
	qq y;
	qq r;
	qq inv_r_mod_n;
	qq h, h1;
	qq s;
	qq x;
	int e;

public:
	client();
	qq n;
	qq v;
	/* create client, n1 - n of server, name - name of client */
	client(const qq n1, string name);
	/* send open keys & vars to server functions */
	qq send_y(int e);
	qq send_x();
	qq send_v();

	~client();
};