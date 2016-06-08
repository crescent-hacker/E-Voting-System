//
// Created by MaYuantong on 13/05/2016.
//
//#include "tool2.hpp"
//#include "tool1.hpp"

//bool simpleSHA256(const char* input, unsigned long length, unsigned char* md)
//{
//    SHA256_CTX context;
//    if(!SHA256_Init(&context))
//        return false;
//
//    if(!SHA256_Update(&context, (unsigned char*)input, length))
//        return false;
//
//    if(!SHA256_Final(md, &context))
//        return false;
//
//    return true;
//}
//
//string calResponse(string input){
//        unsigned char obuf[input.length()];
//
//        simpleSHA256(input.c_str(), input.length(), obuf);
//        unsigned char *val=new unsigned char[input.length()+1];
//        strcpy((char *)val,input.c_str());
//        char* base64EncodeOutput;
//        Base64Encode(val, strlen((char*)val), &base64EncodeOutput);
//        cout<<base64EncodeOutput<<endl;
//        cout<<"=========="<<endl;
//
//        unsigned char* base64DecodeOutput;
//        size_t test;
//        Base64Decode(base64EncodeOutput, &base64DecodeOutput, &test);
//
//        char* output = new char[test+1];
//        for(int i=0;i<test+1;i++)
//        {
//            output[i]=base64DecodeOutput[i];
//        }
//
//        string mystring(output);
//
//    return mystring;
//}



//bool prime[1000010];
//long long p[1000010];
//long long pr[1000010];
//long long k=0,c;
//void isprime()
//{
//    long long i,j;
//    memset(prime,true,sizeof(prime));
//    for(i=2;i<1000010;i++)
//    {
//        if(prime[i])
//        {
//            p[k++]=i;
//            for(j=i+i;j<1000010;j+=i)
//            {
//                prime[j]=false;
//            }
//        }
//    }
//}
//void cal(long long n)
//{
//    long long i;
//    c=0;
//    for(i=0;p[i]*p[i]<=n;i++)
//    {
//        if(n%p[i]==0)
//        {
//            pr[c]=p[i];
//            while(n%p[i]==0) n/=p[i];
//            c++;
//        }
//    }
//    if(n>1)
//    {
//        pr[c]=n;
//        c++;
//    }
//}
//long long quick_mod(long long a,long long b,long long m)
//{
//    long long ans=1;
//    a%=m;
//    while(b)
//    {
//        if(b&1)
//        {
//            ans=ans*a%m;
//            b--;
//        }
//        b>>=1;
//        a=a*a%m;
//    }
//    return ans;
//}
//
//class SysPara
//{
//public:
//    long long** Gs=NULL;
//    long long GN=0;
//    long long P = Prime;
//    long long g;
//    long long h;
//
//    //  generators
//    int generators()
//    {
//        long long i, t, gg, root;
//        long long root_arr[100000] = {1};
//        isprime();
//        cal(P - 1);
//        for (gg = 2; gg < P; gg++) {
//            bool flag = true;
//            for (i = 0; i < c; i++) {
//                t = (P - 1) / pr[i];
//                if (quick_mod(gg, t, P) == 1) {
//                    flag = false;
//                    break;
//                }
//            }
//            if (flag) {
//                root = gg;
//                root_arr[GN] = gg;
//                GN++;
//            }
//        }
//        cout << "The number of generator is " << GN << endl;
//        Gs = new long long* [GN];
//        for (int ggg = 0; ggg < GN; ggg++) {
//            Gs[ggg] = new long long[2];
//        }
//        for (int j = 0; j < GN; j++) {
//            Gs[j][1] = 1;
//            Gs[j][0] = root_arr[j];
//        }
//        g = Gs[t_rand(GN)][0];
//
//        return 0;
//    }
//
//    // g,h
//    int g_h(long long s)
//    {
//        h=t_pow(g,s,P);
//
//        return 0;
//    }
//};
//
//// Generator distribution
//long long* GenD(SysPara sp,long long K)
//{
//    long long* Gset = new long long[K];
//    long long counter=0;
//    while(counter<K)
//    {
//        int r = t_rand(sp.GN);
//        if(sp.Gs[r][1]==1)
//        {
//            Gset[counter]=sp.Gs[r][0];
//            sp.Gs[r][1]=0;
//            counter++;
//        }
//        else
//        {
//            continue;
//        }
//    };
//
//    return Gset;
//}
//
//// arf
//long long PV1_1_arf(SysPara sp)
//{
//    long long a;
//    a = t_rand(int(sp.P-2))+1; // 1<arf<P-1
//
//    return a;
//}
//
//// (X,Y)
//long long* PV1_1(SysPara sp, long long Gi, long long a)
//{
//    long long* xy = new long long[2];
//    xy[0]= t_pow(sp.g,a,sp.P);
//    xy[1]= ((t_pow(sp.h,a,sp.P))*Gi)%sp.P;
//
//    return xy;
//}
//// c1
//long long PV1_2(SysPara sp)
//{
//    long long c1;
//    c1 = t_rand(int(sp.P-2))+1; // 1<c1<P-1
//
//    return c1;
//}
//
//// w1
//long long PV1_3_w1(SysPara sp)
//{
//    long long w1;
//    w1 = t_rand(int(sp.P-2))+1;    // 1<w1<P-1
//
//    return w1;
//}
//
//// (D,R)
//long long** PV1_3_dr(SysPara sp, long long K, long long w1, long long arf, long long candidate, long long c1)
//{
//    long long** dr = new long long*[K];
//    for (long long i=0;i<K;i++)
//    {
//        dr[i] = new long long[2];
//    }
//    long long sum=0;
//    for (long long i=0;i<K;i++)
//    {
//        dr[i][0] = t_rand(int(sp.P-1));
//        dr[i][1] = t_rand(int(sp.P-1));
//        if(i!=candidate)
//        {
//            sum=sum+dr[i][0];
//        }
//    }
//    sum = sum%(sp.P-1);
//
//    dr[candidate][0]= (c1-(sum))%(sp.P-1);
//    if(dr[candidate][0]<0)
//    {
//        dr[candidate][0] = dr[candidate][0]+sp.P-1;
//    }
//
//    dr[candidate][1]=(w1-arf*dr[candidate][0])%(sp.P-1);
//    if(dr[candidate][1]<0)
//    {
//        dr[candidate][1] = dr[candidate][1]+sp.P-1;
//    }
//
//    return dr;
//}
//
//// (A,B)
//long long** PV1_3_AB(SysPara sp, long long K, long long w1, long long candidate, long long** dr, long long* xy, long long* Gset)
//{
//    long long** ab = new long long*[K];
//    for (long long i=0;i<K;i++)
//    {
//        ab[i] = new long long[2];
//    }
//    for (long long i=0;i<K;i++)
//    {
//        if(i!=candidate)
//        {
//            ab[i][0] = (t_pow(sp.g,dr[i][1],sp.P)*t_pow(xy[0],dr[i][0],sp.P))%sp.P;
//            ab[i][1] = ((t_pow(sp.h,dr[i][1],sp.P))*(t_pow(xy[1],dr[i][0],sp.P))*(t_pow(Gset[i],(sp.P-1-dr[i][0]),sp.P)))%sp.P;
//        }
//    }
//    ab[candidate][0]=t_pow(sp.g,w1,sp.P);
//    ab[candidate][1]=t_pow(sp.h,w1,sp.P);
//
//    return ab;
//}
//
//// Verify first ballot
//int PV1_4(SysPara sp, long long K, long long** dr, long long* xy, long long** AB, long long c1, long long* Gset)
//{
//    int flag=1;
//    long long sum=0;
//    long long sum1, sum2;
//    for (long long i=0;i<K;i++)
//    {
//        sum = (sum+ dr[i][0])%(sp.P-1);
//    }
//    if (sum!=c1)
//    {
//        flag=0;
//    }
//
//    for (long long i=0;i<K;i++)
//    {
//        sum1 = ((t_pow(sp.g,dr[i][1],sp.P))*(t_pow(xy[0],dr[i][0],sp.P)))%sp.P;
//        if(AB[i][0]!=sum1)
//        {
//
//            flag=0;
//        }
//        sum2 = ((t_pow(sp.h,dr[i][1],sp.P))*(t_pow(xy[1],dr[i][0],sp.P))*(t_pow(Gset[i],(sp.P-1-dr[i][0]),sp.P)))%sp.P;
//
//        if(AB[i][1]!=sum2)
//        {
//
//            flag=0;
//        }
//    }
//    return flag;
//}
//
////  bta
//long long PK_1_bta(SysPara sp)
//{
//    long long bta = t_rand(int(sp.P-2))+1;
//
//    return bta;
//}
//
////  w2
//long long PK_1_w2(SysPara sp)
//{
//    long long w2 = t_rand(int(sp.P-2))+1;
//
//    return w2;
//}
//
//// (u,v)
//long long* PK_1_uv(SysPara sp, long long bta)
//{
//    long long* uv = new long long[2];
//    uv[0] = t_pow(sp.g,bta,sp.P);
//    uv[1] = t_pow(sp.h,bta,sp.P);
//    return uv;
//}
//
//// (a,b)
//long long* PK_1_ab(SysPara sp, long long w2)
//{
//    long long* ab = new long long[2];
//    ab[0] = t_pow(sp.g,w2,sp.P);
//    ab[1] = t_pow(sp.h,w2,sp.P);
//    return ab;
//}
//
//// c2
//long long PK_2_c2(SysPara sp)
//{
//    long long c2 = t_rand(int(sp.P-2))+1;
//    return c2;
//}
//
//// r
//long long PK_3_r(SysPara sp, long long w2, long long bta, long long c2)
//{
//    long long r;
//    r = (w2+bta*c2)%(sp.P-1);
//    return r;
//}
//
//int PK_4(SysPara sp, long long* ab, long long* uv, long long c2, long long r)
//{
//    long long sum1,sum2;
//    sum1 = (ab[0]*t_pow(uv[0],c2,sp.P))%sp.P;
//    sum2 = (ab[1]*t_pow(uv[1],c2,sp.P))%sp.P;
//    if ((sum1==t_pow(sp.g,r,sp.P))&&(sum2==t_pow(sp.h,r,sp.P)))
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
//}
//
//// (s,t)
//long long** PV2_1(SysPara sp, long long K, long long** AB, long long** dr, long long* uv)
//{
//    long long **st = new long long *[K];
//    for (long long i = 0; i < K; i++)
//    {
//        st[i] = new long long[2];
//    }
//    for (long long i=0;i<K;i++)
//    {
//        st[i][0] = (AB[i][0]*t_pow(uv[0],dr[i][0],sp.P))%sp.P;
//        st[i][1] = (AB[i][1]*t_pow(uv[1],dr[i][0],sp.P))%sp.P;
//    }
//
//    return st;
//}

//// Generate & verify final ballot
//int PV2_2(SysPara sp, long long K, long long* fballot, long long** st, long long* uv, long long** dr, long long* Gset)
//{
//    long long* Fballot = new long long[2];
//    Fballot[0] = (fballot[0]*uv[0])%sp.P;
//    Fballot[1] = (fballot[1]*uv[1])%sp.P;
//
//    long long flag=1;
//
//    for (long long i=0;i<K;i++)
//    {
//        long long sum1 = (t_pow(sp.g,dr[i][1],sp.P)*t_pow(Fballot[0],dr[i][0],sp.P))%sp.P;
//        long long sum2 = (t_pow(sp.h,dr[i][1],sp.P)*t_pow(Fballot[1],dr[i][0],sp.P)*t_pow(Gset[i],(sp.P-1-dr[i][0]),sp.P))%sp.P;
//        if (sum1 != st[i][0] || sum2 != st[i][1])
//        {
//            flag = 0;
//        }
//    }
//    return flag;
//}
//
//long long* PV2_3(SysPara sp, long long K, long long* fballot, long long* uv)
//{
//    long long* Fballot = new long long[2];
//    Fballot[0] = (fballot[0]*uv[0])%sp.P;
//    Fballot[1] = (fballot[1]*uv[1])%sp.P;
//    return Fballot;
//}

