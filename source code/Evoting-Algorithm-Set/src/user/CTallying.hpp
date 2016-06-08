//
// Created by MaYuantong on 1/06/2016.
// Designed for Central Tallying authorities
//

class CentralTally
{
private:

    long long* ballot_pdc(long long voters,long long** ballots, long long P)
    {
        long long* XY = new long long[2];
        XY[0]=1;
        XY[1]=1;
        for (long long i=0;i<voters;i++)
        {
            XY[0] = (XY[0]*ballots[i][0])%P;
            XY[1] = (XY[1]*ballots[i][1])%P;
        }
        return XY;
    }

public:
    long long** XYs;
    long long* t_r;
    long long* lm;
    int Nt;
    int* ptp;
    int times;
    int res;

//    long long* ballot_pdc(long long voters,long long** ballots, long long P)
//    {
//        long long* XY = new long long[2];
//        XY[0]=1;
//        XY[1]=1;
//        for (long long i=0;i<voters;i++)
//        {
//            XY[0] = (XY[0]*ballots[i][0])%P;
//            XY[1] = (XY[1]*ballots[i][1])%P;
//        }
//        return XY;
//    }

    int NumT(int Available, int Needed)
    {
        int Nt = t_rand(Available-Needed+1)+Needed;
        return Nt;
    }

    int* PT(int Nt, int Available)
    {
        int* ptp = new int[Nt];

        for (int i=0;i<Nt;i++)
        {
            ptp[i]=t_rand(Available)+1;
            int flag=1;
            if(i>0)
            {
                for(int j=0;j<i;j++)
                {
                    if(ptp[j]==ptp[i])
                    {
                        flag=0;
                    }
                }
            }
            if(flag==0)
            {
                i--;
            }
        }
        return ptp;
    }

    long long* lmta(long long Nt, long long Q, int* ptp)
    {
        long long* lm = new long long[Nt];
        for (int i=0;i<Nt;i++)
        {
            lm[i]=1;
            for (int j=0;j<Nt;j++)
            {
                if (ptp[i]!=ptp[j])
                {
                    long long temp = (ptp[j]-ptp[i])%Q;
                    if(temp<0)
                    {
                        temp = temp+Q;
                    }
                    lm[i] = (lm[i]*ptp[j]*modInverse(temp,Q))%Q;
                }
            }
        }
        return lm;
    }
    long long* tallying(long long* XY, long long* cm, long long K, long long voters, int Nt, long long P, long long* Gset)
    {
        long long* t_r;
        long long W;
        W = XY[1];
        for (int i=0;i<Nt;i++)
        {
            W = (W*cm[i])%P;
        }

        int flag=0;

        while(flag==0) {
            long long *rp = rpickup(voters, K);
            long long cW = 1;
            for (long long i = 0; i < K; i++) {
                cW = (cW * t_pow(Gset[i], rp[i], P)) % P;
            }
            if (cW == W)
            {
                t_r = rp;
                flag=1;
            }
            else
            {
                delete rp;
            }
        };

        return t_r;
    }

    long long** t_msg(int times, int res, long long** Allballots, long long P)
    {
        long long** XYs = new long long*[times+1];
        for (int t=0;t<times;t++)
        {
            long long voters = 4;
            long long **ballots = new long long*[voters];
            for (int i = 0; i < voters; i++)
            {
                ballots[i] = new long long[2];
                ballots[i][0] = Allballots[i+t*4][0];
                ballots[i][1] = Allballots[i+t*4][1];
            }
            // tallying results
            XYs[t] = ballot_pdc(voters,ballots,P);
            // delete ballots
            for (int i=0;i<voters;i++)
            {
                delete[] ballots[i];
            }
            delete [] ballots;
        }

        long long voters = res;
        long long **ballots = new long long*[voters];
        for (long long i = 0; i < voters; i++)
        {
            ballots[i] = new long long[2];
            ballots[i][0] = Allballots[i+times*4][0];
            ballots[i][1] = Allballots[i+times*4][1];
        }
        // tallying results
        XYs[times] = ballot_pdc(voters,ballots,P);
        // delete ballots
        for (int i=0;i<voters;i++)
        {
            delete[] ballots[i];
        }
        delete [] ballots;
        return XYs;
    }
};

