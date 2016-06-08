//
// Created by MaYuantong on 26/05/2016.
// Designed for Main server
//
#include "tool1.hpp"

#define NN 100001
class SysPara
{
private:

    bool prime[NN];
    long long p[NN];
    long long pr[NN];
    long long k,c;
//    long long Prime;
    long long s;

    void gens()
    {
        s = t_rand(Q-1)+1;
    }

    void isprime()
    {
        long long i,j;
        memset(prime,true,sizeof(prime));
        for(i=2;i<NN;i++)
        {
            if(prime[i])
            {
                p[k++]=i;
                for(j=i+i;j<NN;j+=i)
                {
                    prime[j]=false;
                }
            }
        }
    }

    void cal(long long n)
    {
        long long i;
        c=0;
        for(i=0;p[i]*p[i]<=n;i++)
        {
            if(n%p[i]==0)
            {
                pr[c]=p[i];
                while(n%p[i]==0) n/=p[i];
                c++;
            }
        }
        if(n>1)
        {
            pr[c]=n;
            c++;
        }
    }

    long long quick_mod(long long a,long long b,long long m)
    {
        long long ans=1;
        a%=m;
        while(b)
        {
            if(b&1)
            {
                ans=ans*a%m;
                b--;
            }
            b>>=1;
            a=a*a%m;
        }
        return ans;
    }

public:
    long long** Gs;
    long long GN;
    long long P;
    long long Q;
    long long g;
    long long h;
    long long K;
    long long* Gset;

    long long Nv;
    long long Nb;
    int Needed;
    int Available;

    SysPara(long long GN, long long Q, int Needed, int Available, long long k, long long P)
    {
        this->GN=GN;
        this->Q=Q;
        this->Needed=Needed;
        this->Available=Available;
        this->k = k;
        this->P = P;
    }
    //  Number of generator for P
    long long GenNo(long long P)
    {
        long long GN = 0;
        long long i, t, gg, root;
        long long root_arr[NN] = {1};
        isprime();
        cal(P - 1);
        for (gg = 2; gg < P; gg++) {
            bool flag = true;
            for (i = 0; i < c; i++) {
                t = (P - 1) / pr[i];
                if (quick_mod(gg, t, P) == 1) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                root = gg;
                root_arr[GN] = gg;
                GN++;
            }
        }
        return GN;
    }

    // All possible generators for candidate
    long long** Genlist(long long P)
    {
        long long GN = 0;
        long long i, t, gg, root;
        long long root_arr[NN] = {1};
        isprime();
        cal(P - 1);
        for (gg = 2; gg < P; gg++) {
            bool flag = true;
            for (i = 0; i < c; i++) {
                t = (P - 1) / pr[i];
                if (quick_mod(gg, t, P) == 1) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                root = gg;
                root_arr[GN] = gg;
                GN++;
            }
        }

        long long**Gs = new long long* [GN];
        for (int ggg = 0; ggg < GN; ggg++) {
            Gs[ggg] = new long long[2];
        }
        for (int j = 0; j < GN; j++) {
            Gs[j][1] = 1;
            Gs[j][0] = root_arr[j];
        }
        return Gs;
    }

    // g
    long long sp_g(long long Q)
    {
        long long i, t, gg, root;
        long long GN = 0;
        long long root_arr[NN] = {1};
        isprime();
        cal(Q - 1);
        for (gg = 2; gg < Q; gg++) {
            bool flag = true;
            for (i = 0; i < c; i++) {
                t = (Q - 1) / pr[i];
                if (quick_mod(gg, t, Q) == 1) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                root = gg;
                root_arr[GN] = gg;
                GN++;
            }
        }

        long long** Gs = new long long* [GN];
        for (int ggg = 0; ggg < GN; ggg++) {
            Gs[ggg] = new long long[2];
        }
        for (int j = 0; j < GN; j++) {
            Gs[j][1] = 1;
            Gs[j][0] = root_arr[j];
        }

        long long g5;
        long long* gset;
        long long gn=0;
        for (long long r=0;r<GN;r++)
        {
            if(t_pow(Gs[r][0],Q,P)==1)
            {
                gn++;
                Gs[r][1]=0;
            }
        }
        gset = new long long[gn];

        long long rr=0;
        for (long long r=0;r<GN;r++)
        {
            if(Gs[r][1]==0)
            {
                gset[rr] = Gs[r][0];
                rr++;
            }
        }

        g5 = gset[t_rand(gn)];

        delete [] gset;
        return g5;
    }

    // h
    long long g_h(long long P, long long g)
    {
        long long h;
        gens();
        h=t_pow(g,s,P);
        return h;
    }

    // Candidate list
    long long* GenD(long long GN, long long** Gs, long long K)
    {
        long long* Gset = new long long[K];
        long long counter=0;
        while(counter<K)
        {
            int r = t_rand(GN);
            if(Gs[r][1]==1)
            {
                Gset[counter]=Gs[r][0];
                Gs[r][1]=0;
                counter++;
            }
            else
            {
                continue;
            }
        };

        for (int i=0;i<GN;i++)
        {
            Gs[i][1] = 1;
        }

        return Gset;
    }

    // shamir shares
    long long** split(int Available, int Needed, long long Q)
    {
        int avb=Available;
        long long** sh = new long long*[avb];
        for (int i=0;i<avb;i++)
        {
            sh[i] = new long long[2];
        }
        int a1=t_rand(Q);
        int a2;
        do
        {
            a2 = t_rand(Q);
        }while(a1==a2);

        long long coef[3] = {s,a1,a2};
        for (long long i=1;i<=avb;i++)
        {
            long long accum=coef[0];
            for (long long j=1;j<Needed;j++)
            {
                accum = (accum + coef[j]*t_pow(i,j,Q))%(Q);
            }
            sh[i-1][0] = i;
            sh[i-1][1] = accum;
        }

        return sh;
    }
};

