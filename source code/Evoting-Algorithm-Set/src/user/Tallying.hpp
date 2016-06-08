//
// Created by MaYuantong on 29/05/2016.
// Designed for distributed Tallying authorities
//

class PartTally
{
public:
    long long* cm;

    long long part_tallying(long long *XY, long long P, long long Q, long long sh, long long lm)
    {
        long long cm2[2];

        long long temp = (sh * lm) % Q;
        if (temp < 0) {
            temp = temp + Q;
        }
        cm2[0] = Q - temp;
        cm2[1] = t_pow(XY[0], cm2[0], P);

        return cm2[1];
    }
};

