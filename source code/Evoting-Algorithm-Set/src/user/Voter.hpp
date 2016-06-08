//
// Created by MaYuantong on 26/05/2016.
// Designed for Voter client
//

class Voter
{
public:
    int candidate;
    long long Gi;
    long long arf;
    long long* fballot;
    long long w1;
    long long** dr;
    long long** AB;
    long long c2;
    bool PK_verf;
    bool PV2_verf;
    long long* Fballot;

    //------------PV1------------
    // Gi
    long long GetGi(long long* Gset, int a)
    {
        return Gset[a-1];
    }

    // arf
    long long PV1_1_arf(long long Q)
    {
        long long temp;
        temp = t_rand(int(Q-1))+1; // 1<arf<P-1
        return temp;
    }

    // (x,y)
    long long* PV1_1(long long P, long long g, long long h, long long arf, long long Gi)
    {
        long long* temp = new long long[2];
        temp[0]= t_pow(g,arf,P);
        temp[1]= ((t_pow(h,arf,P))*Gi)%P;
        return temp;
    }

    // w1
    long long PV1_3_w1(long long Q)
    {
        long long temp;
        temp = t_rand(int(Q-2))+1;    // 1<w1<P-1
        return temp;
    }

    // (D,R)
    long long** PV1_3_dr(long long Q, long long K, long long c1, int a, long long w1, long long arf)
    {
        long long** temp;
        temp = new long long*[K];
        for (long long i=0;i<K;i++)
        {
            temp[i] = new long long[2];
        }
        long long sum=0;
        for (long long i=0;i<K;i++)
        {
            temp[i][0] = t_rand(int(Q));
            temp[i][1] = t_rand(int(Q));
            if(i!=(a-1))
            {
                sum=sum+temp[i][0];
            }
        }
        sum = sum%(Q);

        temp[a-1][0]= (c1-(sum))%(Q);
        if(temp[a-1][0]<0)
        {
            temp[a-1][0] = temp[a-1][0]+Q;
        }

        temp[a-1][1]=(w1-arf*temp[a-1][0])%(Q);
        if(temp[a-1][1]<0)
        {
            temp[a-1][1] = temp[a-1][1]+Q;
        }

        return temp;
    }

    // (A,B)
    long long** PV1_3_AB(long long P, long long g, long long h, long long K, int candidate, long long w1, long long* fballot, long long* Gset, long long** dr)
    {
        long long** AB = new long long*[K];
        for (long long i=0;i<K;i++)
        {
            AB[i] = new long long[2];
        }
        for (long long i=0;i<K;i++)
        {
            if(i!=candidate-1)
            {
                AB[i][0] = (t_pow(g,dr[i][1],P)*t_pow(fballot[0],dr[i][0],P))%P;
                AB[i][1] = ((t_pow(h,dr[i][1],P))*(t_pow(fballot[1],dr[i][0],P))*(t_pow(Gset[i],(P-1-dr[i][0]),P)))%P;
            }
        }
        AB[candidate-1][0]=t_pow(g,w1,P);
        AB[candidate-1][1]=t_pow(h,w1,P);

        return AB;
    }

    // -----------------PK------------------

    // c2
    long long PK_2_c2(long long Q)
    {
        long long c2;
        c2 = t_rand(int(Q-1))+1;
        return c2;
    }

    // verfy pk
    bool PK_4(long long P, long long g, long long h, long long* ab, long long* uv, long long r, long long c2)
    {
        bool PK_verf;
        long long sum1,sum2;
        sum1 = (ab[0]*t_pow(uv[0],c2,P))%P;
        sum2 = (ab[1]*t_pow(uv[1],c2,P))%P;
        if ((sum1==t_pow(g,r,P))&&(sum2==t_pow(h,r,P)))
        {
            PK_verf=1;
        }
        else
        {
            PK_verf=0;
        }
        return PK_verf;
    }

    //-------------------PV2-----------------
    // Generate final ballot
    long long* PV2_2(long long P, long long* fballot, long long* uv)
    {
        long long* Fballot;
        Fballot = new long long[2];
        Fballot[0] = (fballot[0]*uv[0])%P;
        Fballot[1] = (fballot[1]*uv[1])%P;
        return Fballot;
    }

    // Verify final ballot
    bool PV2_3(long long P, long long g, long long h, long long* Gset, long long K, long long** dr, long long** st, long long* uv, long long* Fballot)
    {
        bool flag=1;

        for (long long i=0;i<K;i++)
        {
            long long sum1 = (t_pow(g,dr[i][1],P)*t_pow(Fballot[0],dr[i][0],P))%P;
            long long sum2 = (t_pow(h,dr[i][1],P)*t_pow(Fballot[1],dr[i][0],P)*t_pow(Gset[i],(P-1-dr[i][0]),P))%P;
            if (sum1 != st[i][0] || sum2 != st[i][1])
            {
                flag = 0;
            }
        }
        return flag;
    }
};



