//
// Created by MaYuantong on 26/05/2016.
// Designed for Honest verifier
//

class HV
{
public:
    long long c1;
    bool PV1_verf;
    long long bta;
    long long w2;
    long long* uv;
    long long* ab;
    long long r;
    long long** st;

    //---PV1---
    // c1
    long long PV1_2(long long Q)
    {
        long long temp;
        temp = t_rand(int(Q-1))+1; // 1<c1<P-1
        return temp;
    }

    // Verify first ballot
    bool PV1_4(long long P, long long g, long long h, long long* Gset, long long K, long long** dr, long long* fballot, long long** AB, long long Q)
    {
        bool flag=1;
        long long sum=0;
        long long sum1, sum2;
        for (long long i=0;i<K;i++)
        {
            sum = (sum+ dr[i][0])%(Q);
        }
        if (sum!=c1)
        {
            flag=0;
        }

        for (long long i=0;i<K;i++)
        {
            sum1 = ((t_pow(g,dr[i][1],P))*(t_pow(fballot[0],dr[i][0],P)))%P;
            if(AB[i][0]!=sum1)
            {

                flag=0;
            }
            sum2 = ((t_pow(h,dr[i][1],P))*(t_pow(fballot[1],dr[i][0],P))*(t_pow(Gset[i],(P-1-dr[i][0]),P)))%P;

            if(AB[i][1]!=sum2)
            {

                flag=0;
            }
        }
        return flag;
    }

    //  bta
    long long PK_1_bta(long long Q)
    {
        long long bta;
        bta = t_rand(int(Q-1))+1;
        return bta;
    }

    //  w2
    long long PK_1_w2(long long Q)
    {
        long long w2;
        w2 = t_rand(int(Q-1))+1;
        return w2;
    }

    // (u,v)
    long long* PK_1_uv(long long P, long long g, long long h, long long bta)
    {
        long long* uv = new long long[2];
        uv[0] = t_pow(g,bta,P);
        uv[1] = t_pow(h,bta,P);
        return uv;
    }

    // (a,b)
    long long* PK_1_ab(long long P, long long g, long long h, long long w2)
    {
        long long* ab = new long long[2];
        ab[0] = t_pow(g,w2,P);
        ab[1] = t_pow(h,w2,P);
        return ab;
    }

    // r
    long long PK_3_r(long long P, long long c2, long long w2, long long bta)
    {
        long long r;
        r = (w2+bta*c2)%(P-1);
        return r;
    }

    //----PV2----
    // (s,t)
    long long** PV2_1(long long P, long long K, long long* uv, long long** AB, long long** dr)
    {
        long long** st = new long long *[K];
        for (long long i = 0; i < K; i++)
        {
            st[i] = new long long[2];
        }
        for (long long i=0;i<K;i++)
        {
            st[i][0] = (AB[i][0]*t_pow(uv[0],dr[i][0],P))%P;
            st[i][1] = (AB[i][1]*t_pow(uv[1],dr[i][0],P))%P;
        }
        return st;
    }
};



