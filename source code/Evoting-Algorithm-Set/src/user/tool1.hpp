//
// Created by MaYuantong on 22/05/2016.
// Designed for calculation
//
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "string.h"
#include "stdlib.h"

long long t_pow(long long a, long long b, long long m)
{
    long long r=1;
    while(b--)
    {
        r=r*a;
        r=r%m;
    }
    return r;
}

int t_rand(int range)
{
    int r;
    srand (time(NULL));
    r = rand()%range;
    if(range==0)
    {
        r=0;
    }
    return r;
}

long long* rpickup(long long voters, long long K)
{
    long long* r = new long long[K];
    long long rem = voters;
    for (long long i=0;i<K-1;i++)
    {
        r[i] = (rand()%(rem+1));
        rem = rem-r[i];
    }
    r[K-1] = rem;
    for (long long i=0;i<K-1;i++)
    {
        long long jj= (rand()%(K-i-1))+i+1;
        long long temp = r[i];
        r[i] = r[jj];
        r[jj] = temp;
    }

    return r;
}

long long* gcdD(long long a, long long b)
{
    long long* g = new long long[3];
    if (b == 0)
    {
        g[0] = a;
        g[1] = 1;
        g[2] = 0;
        return g;
    }
    else
    {
        long long n = a/b;
        long long c = a%b;
        long long* r = gcdD(b,c);
        g[0] = r[0];
        g[1] = r[2];
        g[2] = r[1]-r[2]*n;
        return g;
    }
}

long long modInverse(long long k, long long p)
{
    k = k % p;
    long long r;
    if(k < 0)
    {
        r = -gcdD(p,-k)[2];
    }
    else
    {
        r = gcdD(p,k)[2];
    }

    return (p + r) % p;
}