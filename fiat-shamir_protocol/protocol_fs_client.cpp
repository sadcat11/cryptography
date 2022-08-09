#include "lib.hpp"
#include "lib_client.hpp"

#define INFO 0

using namespace std;
using std::chrono::system_clock;


client::client()
{
}

client::client(const qq n1, string name)
{
	n = n1;
	string strs;
	ifstream file;
	/* check if client registred */
	file.open("client/" + name + "_s.txt");
	if (file.is_open())
		getline(file, strs);
	file.close();
	if (strs.empty())
	{
		s = gen_coprime(n);
		ofstream file;
		file.open("client/" + name + "_s.txt");
		file << s << "\n";
		file.close();
	}
	else
	{
		s = str_to_qq(strs);
	}
	//s = 43215;
}

qq client::send_x()
{
	/* x = r ^ 2, x - open, r - private */
	r = getrand(1, n - 1);
	//r = 38177;
	x = fast_exp_modul(r, n, 2);
	return x;
}

qq client::send_v()
{
	v = fast_exp_modul(s, n, 2);
	return v;
}

qq client::send_y(int e)
{
#if INFO
	cout << r << " r\n";
	cout << s << " s\n";
	cout << v << " v\n";
	cout << n << " n\n";
#endif
	/* bit e conditions, y - open, r - private */
	if (e == 0)
	{
		y = r;
	}
	else
	{
		y = fast_exp_modul((qq)(r * s), n, 1);
	}
	return y;
}

client::~client()
{
}