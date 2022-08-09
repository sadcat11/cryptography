#include "lib.hpp"

using namespace std;

typedef boost::mt19937 gent;
gent gen(time(NULL));


qq getrand(qq min, qq max)
{
    br::uniform_int_distribution<bm::cpp_int> dist(min, max);
    return (qq)dist(gen);
}

bool is_prime(qq num)
{
    uqq num1 = (uqq)num;
	if (miller_rabin_test(num1, (int)1e4))
		return true;
	return false;
}

qq gen_prime(qq min, qq max)
{
	qq num;
	do
	{
		num = getrand(min, max);
		//cout << num << "\n";
	} while (!is_prime((qq)num));

	return num;
}

/* get number form string */
qq str_to_qq(string number)
{
	qq result = 0;
	ull order = number.length();
	uint8 high, low;

	for (ull i = 0; i < order; ++i)
	{
		high = number[i] & 0b00001111;
		result += high;
		result *= 10;
	}

	return result / 10;
}


/* gen big prime using openssl to get prime with a certain number of bits */
qq gen_big_prime(uint16 bits)
{
	string param = "openssl prime -generate -bits " + to_string(bits) + " >> buffer\n";

	system(param.c_str());
	ifstream file("buffer");
	qq buffer;

	file >> buffer;
	file.close();
	remove("buffer");

	return buffer;
}

/******************************
    FAST MODULO EXPON
******************************/
/*  a^x mod b = y, returns y */
qq fast_exp_modul(qq a, qq b, qq x)
{
    qq res = 1;
    while (x != 0)
    {
        if ((x & 1) != 0)
        {
            res = (res * a) % b;
        }
        a = (a * a) % b;
        x >>= 1;
    }
    return res;
}


qq gcd(qq num1, qq num2, qq *x, qq *y)
{
	if(!num1) {
		*x = 0;
		*y = 1;
	   	return num2;
	}

	qq num, x1, y1;
	num = gcd(num2 % num1, num1, &x1, &y1);

	*x = y1 - (num2 / num1) * x1;
	*y = x1;

	return num;
}

qq gen_coprime(qq num)
{
	qq key, x;

	do {
		key = getrand(1, num);
	} while(euc(num, key) != 1);

	return key;
}


/******************************
            EUC
******************************/
// a * x + b * y = NOD(a; b)
qq euc(qq *a, qq *b, qq *nod)
{
    qq u[3], v[3], t[3], q = 0;
    t[0] = 1;
    if (a > b)
    {
        u[0] = *a;
        v[0] = *b;
    }
    else
    {
        u[0] = *b;
        v[0] = *a;
    }
    u[1] = 1;
    u[2] = 0;
    v[1] = 0;
    v[2] = 1;
    while (t[0] != 0)
    {
        q = u[0] / v[0];
        t[0] = u[0] % v[0];
        t[1] = u[1] - q * v[1];
        t[2] = u[2] - q * v[2];
        for (int i = 0; i <= 2; i++)
        {
            u[i] = v[i];
            v[i] = t[i];
        }
    }

    *a = u[1];
    *b = u[2];
    *nod = u[0];
    return 0;
}

/* return NOD of a & b */
qq euc(qq a, qq b)
{
    qq u[3], v[3], t[3], q = 0;
    t[0] = 1;
    if (a > b)
    {
        u[0] = a;
        v[0] = b;
    }
    else
    {
        u[0] = b;
        v[0] = a;
    }
    u[1] = 1;
    u[2] = 0;
    v[1] = 0;
    v[2] = 1;
    while (t[0] != 0)
    {
        q = u[0] / v[0];
        t[0] = u[0] % v[0];
        t[1] = u[1] - q * v[1];
        t[2] = u[2] - q * v[2];
        for (int i = 0; i <= 2; i++)
        {
            u[i] = v[i];
            v[i] = t[i];
        }
    }
    return u[0];
}