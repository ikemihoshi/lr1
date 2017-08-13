#include"Utils.h"

char szFmt[] = "0123456789abcdefghijklmnopqrstuvwxyz";

String formatInt(int value, unsigned int fmt) {
	if (fmt == 0 || fmt > sizeof(szFmt) / sizeof(szFmt[0])) {
		throw String("format error!") + fmt;
	}
	if (value < 0) {
		return String("-") + formatInt(-value, fmt);
	}
	else {
		if (value == 0) {
			return String("");
		}
		else {
			return formatInt(value / fmt, fmt) + (String("") + value % fmt);
		}
	}
}

Executor* getExecutor(LR1* lr1)
{
	return (Executor*)lr1->getParam("executor");
}

void setExecutor(LR1* lr1, Executor* exec)
{
	lr1->setParam("executor", exec);
}

int gcd(int a, int b) {
	if (a % b == 0) {
		return b;
	}
	return gcd(b, a % b);
}

int lcm(int a, int b) {
	ll m = a;
	m *= b;
	m /= gcd(a, b);
	return m;
}

void axbyg(int &a, int &b, int &g) {
	int orga = a;
	int orgb = b;
	ll x = 1, v = 0, w = b;
	g = a;
	ll y;
	while (true) {
		if (w == 0) {
			y = (g - a * x) / b;
			a = x;
			b = y;
			if (g < 0) {
				g = -g;
				a = -a;
				b = -b;
			}
			if (a >= orgb) {
				int c = a / orgb;
				a -= c * orgb;
				b += c * orga;
			}
			if (a < 0) {
				int c = a / orgb - 1;
				a -= c * orgb;
				b += c * orga;
			}
			return;
		}
		ll t = g % w;
		ll q = g / w;
		ll s = x - q * v;
		x = v;
		g = w;
		v = s;
		w = t;
	}
}

String decompose(int v) {
	String res = "";
	int count = 0;
	int testV = 2;
	while (v > 1) {
		while (v % testV == 0) {
			count++;
			v /= testV;
		}
		if (count > 0) {
			res = res + testV + "(" + count + ")";
			if (v > 1) {
				res = res + " * ";
			}
			count = 0;
		}
		if (testV == 2) {
			testV = 3;
		}
		else {
			testV += 2;
		}
	}
	return res;
}

int powermod(int a, int b, int c) {
	if (b == 0) {
		return 1;
	}
	ll t = a % c;
	ll res = 1;
	while (b != 0) {
		if (b & 1 != 0) {
			res = (res * t) % c;
		}
		t = ( t * t ) % c;
		b >>= 1;
	}
	return res;
}