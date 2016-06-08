#include "Systemp.hpp"
#include "HV.hpp"
#include "Voter.hpp"
#include "Tallying.hpp"
#include "CTallying.hpp"

using namespace std;

int main(int argv,char* args[]){

    // ====================System setup Start=====================

    // secret key
//    long long s;
//    cout<<"Input secret key: ";
//    cin>>s;
    // system parameters
    SysPara sp(0,883,2,5,0,8831);
        // Gs[][0]: generator  Gs[][1]: state of usage
        // GN: number of generators
        // P: large prime
        // Q: large prime
        // g: a generator of GP
        // h: public key
        // Available: (n,t) -> n
        // Needed: (n,t) ->t

    sp.GN = sp.GenNo(sp.P);
    sp.Gs = sp.Genlist(sp.P);
    sp.g = sp.sp_g(sp.Q);
    sp.h = sp.g_h(sp.P, sp.g);
    // shares
    long long** sh;
    sh = sp.split(sp.Available, sp.Needed, sp.Q);
    //-***-
    cout<< "\nShares: \n";
    for (int i=0;i<sp.Available;i++)
    {
        cout<<sh[i][0]<<"   "<<sh[i][1]<<endl;
    }
    //-***-
    cout<<"\n------------System parameters--------------\n";
    cout<< "P: "<<sp.P<<endl<<"g: "<<sp.g<<endl<<"h: "<<sp.h<<endl<<"GN: "<<sp.GN<<endl<<"Gs:\n";
    for (int i=0;i<sp.GN;i++)
    {
        cout<< sp.Gs[i][0]<<endl;
    }

    cout<<"\nHow many candidates: ";
//    int K;
    cin>>sp.K;
    sp.Gset = sp.GenD(sp.GN, sp.Gs, sp.K);
    cout<<"\nThere are "<<sp.K<<" candidates and their ballots are "<<endl;
    for (int i=0;i<sp.K;i++)
    {
        cout<<sp.Gset[i]<<endl;
    }
//
//    //++++++++++++For testing++++++++++++++++++
//    sp.Gset[0]=290;
//    sp.Gset[1]=7004;
//    sp.Gset[2]=4953;
//    sp.Gset[3]=2866;
//    sp.Gset[4]=811;
//    sp.Gset[5]=7499;
//    sp.Gset[6]=5431;
//    sp.Gset[7]=3367;
//    sp.Gset[8]=1302;
//    sp.Gset[9]=7990;
//    //+++++++++++++++++++++++++++++++++++++++++
    // =====================System setup End==========================

    // =====================Voting process Start======================

    // voters information
    Voter vt;
    HV hv;
    cout<<"Enter the number of voters: ";
    cin >> sp.Nv;
    long long** Allballots = new long long*[sp.Nv];
    for (long long i=0;i<sp.Nv;i++)
    {
        Allballots[i] = new long long[2];
    }

    // casting votes
    for (long long n=0;n<sp.Nv;n++)
    {
        cout<<"\nVoter "<<n+1<<": \n";

        // ----------------step 1: PV1---------------------
        //
        cout<<"\n\n------------The proof of the validity of the first ballot (PV1)-------------\n";
        cout<<"The candidate you choose: ";
        cin>>vt.candidate;

        cout<<"\n++++++Step 1++++++\n";
        // Choose Gi
        vt.Gi = vt.GetGi(sp.Gset,vt.candidate);

        // Randomly choose arf
        vt.arf = vt.PV1_1_arf(sp.Q);
        cout<< "a: "<<vt.arf<<endl;

        // First ballot
        vt.fballot = vt.PV1_1(sp.P, sp.g, sp.h, vt.arf, vt.Gi);
        cout<<"(x,y): ("<<vt.fballot[0]<<","<<vt.fballot[1]<<")\n\n";

        cout<<"++++++Step 2++++++\n";
        // Challenge c1
        hv.c1 = hv.PV1_2(sp.Q);
        cout<<"c1: "<<hv.c1<<endl<<endl;

        cout<<"++++++Step 3++++++\n";
        // Random number w1
        vt.w1 = vt.PV1_3_w1(sp.Q);
        cout<< "w1: "<<vt.w1<<endl;

        // (D,R)
        vt.dr = vt.PV1_3_dr(sp.Q, sp.K, hv.c1, vt.candidate, vt.w1, vt.arf);
        cout<<"(D,R):"<<endl;
        for (long long i=0;i<sp.K;i++)
        {
            cout<<"("<<vt.dr[i][0]<<","<<vt.dr[i][1]<<")"<<"   ";
        }
        cout<<endl;

        // (A,B)
        vt.AB = vt.PV1_3_AB(sp.P, sp.g, sp.h, sp.K, vt.candidate, vt.w1, vt.fballot, sp.Gset, vt.dr);
        cout<<"(A,B):"<<endl;
        for (long long i=0;i<sp.K;i++)
        {
            cout<<"("<<vt.AB[i][0]<<","<<vt.AB[i][1]<<")"<<"   ";
        }
        cout<<endl;

        cout<<"\n++++++Step 4++++++\n";
        // Verify the validity of the first ballot
        hv.PV1_verf= hv.PV1_4(sp.P, sp.g, sp.h, sp.Gset, sp.K, vt.dr, vt.fballot, vt.AB, sp.Q);

        if(hv.PV1_verf==1)
        {
            cout<<"First ballot is Valid!\n";
        }
        else
        {
            cout<<"First ballot is not Valid!\n";
        }

        // ---------------step 2: PK---------------------
        //
        cout<<"\n-------------The proof of knowledge of common exponent (PK)-------------\n";

        cout<<"\n++++++Step 1++++++\n";
        // bta
        hv.bta = hv.PK_1_bta(sp.Q);
        cout<<"bta: "<< hv.bta<<endl;

        // w2
        hv.w2 = hv.PK_1_w2(sp.Q);
        cout<<"w2: "<< hv.w2<<endl;

        // (u,v)
        hv.uv = hv.PK_1_uv(sp.P, sp.g, sp.h, hv.bta);
        cout<<"(u,v): "<<"("<<hv.uv[0]<<","<<hv.uv[1]<<")\n";

        // (a,b)
        hv.ab = hv.PK_1_ab(sp.P, sp.g, sp.h, hv.w2);
        cout<<"(a,b): "<<"("<<hv.ab[0]<<","<<hv.ab[1]<<")\n";

        cout<<"\n++++++Step 2++++++\n";
        // Challenge c2
        vt.c2 = vt.PK_2_c2(sp.Q);
        cout<<"c2: "<< vt.c2<<endl;

        cout<<"\n++++++Step 3++++++\n";
        // r
        hv.r = hv.PK_3_r(sp.P, vt.c2, hv.w2, hv.bta);
        cout<<"r: "<<hv.r<<endl;

        cout<<"\n++++++Step 4++++++\n";
        // Varify the common knowledge
        vt.PK_verf = vt.PK_4(sp.P, sp.g, sp.h, hv.ab, hv.uv, hv.r, vt.c2);
        if(vt.PK_verf==1)
        {
            cout<<"Knowledge of common exponent is proved!\n";
        }
        else
        {
            cout<<"Knowledge of common exponent is not right!\n";
        }

        // -----------------step3: PV2-----------------------
        //
        cout<<"\n-------------The proof of the validity of the final ballot (PV2)-------------\n";

        cout<<"\n++++++Step 1++++++\n";
        // (S,T)
        hv.st = hv.PV2_1(sp.P, sp.K, hv.uv, vt.AB, vt.dr);
        cout<<"(S,T):"<<endl;
        for (long long i=0;i<sp.K;i++)
        {
            cout<<"("<<hv.st[i][0]<<","<<hv.st[i][1]<<")"<<"   ";
        }
        cout<<endl;


        // Generate the final ballot
        vt.Fballot = vt.PV2_2(sp.P, vt.fballot, hv.uv);

        cout<<"\n++++++Step 2++++++\n";
        // Varify the validity of the final ballot
        vt.PV2_verf = vt.PV2_3(sp.P, sp.g, sp.h, sp.Gset, sp.K, vt.dr, hv.st, hv.uv, vt.Fballot);
        if(vt.PV2_verf==1)
        {
            cout<<"Final ballot is Valid!\n";
        }
        else
        {
            cout<<"Final ballot is not Valid!\n";
        }
        cout<<"\n++++++Step 2++++++\n";
        cout<<"Final ballot (x,y) is "<<"("<<vt.Fballot[0]<<","<<vt.Fballot[1]<<").\n\nCounting.......\n";
        Allballots[n][0] = vt.Fballot[0];
        Allballots[n][1] = vt.Fballot[1];

        // ------------free memory---------------
        // delete fballot
        delete [] vt.fballot;
        // delete dr
        for (int i=0;i<sp.K;i++)
        {
            delete[] vt.dr[i];
        }
        delete [] vt.dr;
        // delete AB
        for (int i=0;i<sp.K;i++)
        {
            delete[] vt.AB[i];
        }
        delete [] vt.AB;
        // delete uv
        delete [] hv.uv;
        // delete ab
        delete [] hv.ab;
        // delete st
        for (int i=0;i<sp.K;i++)
        {
            delete[] hv.st[i];
        }
        delete [] hv.st;
        // delete Fballot
        delete [] vt.Fballot;
        // --------------------------------------
    }
    //========================Voting process End===========================

    // =======================Tallying process Start=======================

    // ----------------step 1: ballots management----------------
    //
    // ballots management
    sp.Nb = sp.Nv;
    CentralTally ctly;

    ctly.times = sp.Nb/4;
    ctly.res = sp.Nb%4;

    // Product of ballots
    ctly.XYs = ctly.t_msg(ctly.times, ctly.res, Allballots, sp.P);

    // store tallying result for each round
    int** rset = new int*[ctly.times+1];
    //-***-
    for (int i=0;i<ctly.times+1;i++)
    {
        rset[i] = new int[sp.K];
    }
    //-***-

    // ------------step 2: secrete share scheme------------
    //
//    // shares
//    long long** sh;
//    sh = sp.split(sp.Available, sp.Needed, sp.Q);
//    //-***-
//    cout<< "\nShares: \n";
//    for (int i=0;i<sp.Available;i++)
//    {
//        cout<<sh[i][0]<<"   "<<sh[i][1]<<endl;
//    }
//    //-***-

    // Randomly choose tallying authorities
    ctly.Nt = ctly.NumT(sp.Available,sp.Needed);
    //-***-
    cout<<"\nRandomly choose "<<ctly.Nt<<" participants (1 to "<<sp.Available<<"): ";
    //-***-

    // ptp
    ctly.ptp = ctly.PT(ctly.Nt, sp.Available);
    //-***-
    for (int i=0;i<ctly.Nt;i++)
    {
        cout<<ctly.ptp[i]<<" ";
    }
    cout<<endl;
    //-***-
    // lmta parameter
    ctly.lm = ctly.lmta(ctly.Nt, sp.Q, ctly.ptp);

    // ----------step 3: distributed calculation and tallying----------
    //
    PartTally ptly;
    ptly.cm = new long long[ctly.Nt];

    // +++first 4*times ballots+++
    for (int t=0;t<ctly.times;t++)
    {
        long long voters = 4;

        // distributed calculation results
        for (int i=0;i<ctly.Nt;i++)
        {
            ptly.cm[i] = ptly.part_tallying(ctly.XYs[t], sp.P, sp.Q, sh[ctly.ptp[i]-1][1], ctly.lm[i]);
        }

        // gather all calculation results and tallying
        ctly.t_r = ctly.tallying(ctly.XYs[t],ptly.cm, sp.K, voters, ctly.Nt, sp.P, sp.Gset);

        for (int i=0;i<sp.K;i++)
        {
            rset[t][i] = ctly.t_r[i];
        }

        //--------free memory-------------------
        // delete t_r
        delete [] ctly.t_r;
        //--------------------------------------
    }

    // ++++the rest ballots+++
    long long voters = ctly.res;
    // distributed calculation results
    for (int i=0;i<ctly.Nt;i++)
    {
        ptly.cm[i] = ptly.part_tallying(ctly.XYs[ctly.times], sp.P, sp.Q, sh[ctly.ptp[i]-1][1], ctly.lm[i]);
    }

    // gather all calculation results and tallying
    ctly.t_r = ctly.tallying(ctly.XYs[ctly.times],ptly.cm, sp.K, voters, ctly.Nt, sp.P, sp.Gset);

    for (int i=0;i<sp.K;i++)
    {
        rset[ctly.times][i] = ctly.t_r[i];
    }

    //--------free memory-------------------
    // delete ptp
    delete [] ctly.ptp;
    // delete lmt
    delete [] ctly.lm;
    // delete cm
    delete [] ptly.cm;
    // delete t_r
    delete [] ctly.t_r;
    //--------------------------------------

    // ------------step 4: calculate the final tallying result------------
    //
    int* result = new int[sp.K];
    for (int i=0;i<sp.K;i++)
    {
        result[i]=0;
    }
    for (int i=0;i<ctly.times+1;i++)
    {
        for (int j = 0; j < sp.K; j++)
        {
            result[j] = result[j]+ rset[i][j];
        }
    }
    //-------free memory-----
    // delete rset
    for (int i=0;i<ctly.times+1;i++)
    {
        delete[] rset[i];
    }
    delete [] rset;
    //-----------------------

    cout<<"\n---------------Final tallying results-----------------\n";

    for (long long i=0;i<sp.K;i++)
    {
        cout<<"Candidate "<<i+1<<": ";
        cout<<result[i]<<endl;
    }
    cout<<"------------------------------------------------\n";

    //-------free memory-----
    // delete result
    delete [] result;
    //-----------------------

    //======================Tallying process End=======================
    return 0;
}