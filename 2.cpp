#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<math.h>
#include<stdlib.h>
#include<assert.h>
#include<malloc.h>
#include<sys/time.h>
#include "papi.h"
#define For(i,j,n) for(int i=j;i<n;++i)
using namespace std;
int flag=0,Na=0,Nb=0,N=0,Type=0;
int pos(int x,int y){return y+N*x;}

template<class T>
T get_num(FILE* fd,T a)
{
    char t[1];
    T sum=0;a=0;
    fread(t,1,1,fd);
    while(t[0]<'0'||t[0]>'9')fread(t,1,1,fd);
    while(t[0]>='0'&&t[0]<='9')sum=sum*10+t[0]-'0',fread(t,1,1,fd);
    
    return sum;
}

template<class T>
void Read(T*a,FILE *fa)
{ 
   T k;k=0;
   For(i,0,N)For(j,0,N)a[pos(i,j)]=get_num(fa,k);
}

char buf[20];
int L=0;
template<class T>
void Writebuf(T x)
{
    L=0;
    if(x==0){buf[0]='0';buf[1]='\0';L=1;return;}
    while(x>0)
    {
        buf[L]=x%10+'0';x/=10;++L;
    }
    For(i,0,L/2){char t=buf[i];buf[i]=buf[L-1-i];buf[L-1-i]=t;}
    buf[L]=' ';++L;
    buf[L]='\0';
}
template<class T>
void Printing(T*c,FILE*fc)
{
    For(i,0,N)
    {
        For(j,0,N)
        {
            Writebuf(c[pos(i,j)]);
            fwrite(buf,1,L,fc);
        }
        fwrite("\n",1,1,fc);
    }
}

template<class T>
void ijk(T*a,T*b,T*c)
{
    For(i,0,N)For(j,0,N)
    {
        T sum=0;
        For(k,0,N)sum+=a[pos(i,k)]*b[pos(k,j)];
        c[pos(i,j)]=sum;
    }
}

template<class T>
void jik(T*a,T*b,T*c)
{
    For(j,0,N)For(i,0,N)
    {
        T sum=0;
        For(k,0,N)sum+=a[pos(i,k)]*b[pos(k,j)];
        c[pos(i,j)]=sum;
    }
}

template<class T>
void ikj(T*a,T*b,T*c)
{
    For(i,0,N)For(k,0,N)
    {
        T r=a[pos(i,k)];
        For(j,0,N)c[pos(i,j)]+=r*b[pos(k,j)];
    }
}

template<class T>
void kij(T*a,T*b,T*c)
{
    For(k,0,N)For(i,0,N)
    {
        T r=a[pos(i,k)];
        For(j,0,N)c[pos(i,j)]+=r*b[pos(k,j)];
    }
}

template<class T>
void jki(T*a,T*b,T*c)
{
    For(j,0,N)For(k,0,N)
    {
        T r=b[pos(k,j)];
        For(i,0,N)c[pos(i,j)]+=r*a[pos(i,k)];
    }
}

template<class T>
void kji(T*a,T*b,T*c)
{
    For(k,0,N)For(j,0,N)
    {
        T r=b[pos(k,j)];
        For(i,0,N)c[pos(i,j)]+=r*a[pos(i,k)];
    }
}

template<class T>
void work(T*a,T*b,T*c,FILE *fc)
{    
    switch(Type)
    {
        case 0:
           ijk(a,b,c);
           break;
        case 1:
           ikj(a,b,c);
           break;
        case 2:
           kij(a,b,c);
           break;
        case 3:
           jik(a,b,c);
           break;
        case 4:
           jki(a,b,c);
           break;
        case 5:
           kji(a,b,c);
           break;
        default:
           cout<<"Type error."<<endl;
    }
}
long_long values[10], values1[10], values2[10];
int main(int argc,char**argv)
{
    Type=atoi(argv[4]);
    FILE *fa=fopen(argv[1],"r");
    if(fa==NULL){printf("file a error\n");return 1;}
    FILE *fb=fopen(argv[2],"r");
    if(fb==NULL){printf("file b error\n");return 1;}
    FILE *fc=fopen(argv[3],"w");
    if(fc==NULL){printf("file c error\n");return 1;}

    srand(time(0));
    
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)exit(-1);
    int EventSet = PAPI_NULL;

    if (PAPI_create_eventset(&EventSet) != PAPI_OK)exit(-1);

    //if (PAPI_add_event(EventSet, PAPI_L1_LDM) != PAPI_OK)exit(-1);
    //if (PAPI_add_event(EventSet, PAPI_L1_STM) != PAPI_OK)exit(-1);
    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK)exit(-1);

    //if (PAPI_add_event(EventSet, PAPI_L2_LDM) != PAPI_OK)exit(-1);
    //if (PAPI_add_event(EventSet, PAPI_L2_STM) != PAPI_OK)exit(-1);
    if (PAPI_add_event(EventSet, PAPI_L2_DCM) != PAPI_OK)exit(-1);


    char type_a[1],type_b[1];
    fread(type_a,1,1,fa);
    fread(type_b,1,1,fb);
    Na=get_num(fa,Na);
    Nb=get_num(fb,Nb);
    N=max(Na,Nb);
    if(type_a[0]=='l'&&type_b[0]=='l')flag=1;

    cout<<"N="<<N<<endl;
    cout<<"flag="<<flag<<endl;
    cout<<"Type="<<Type<<endl;

    if(flag)
    {
        long long*a,*b,*c;
        a=(long long*)malloc(N*N*sizeof(long long));
        b=(long long*)malloc(N*N*sizeof(long long));
        c=(long long*)malloc(N*N*sizeof(long long));
        For(i,0,N)For(j,0,N)c[pos(i,j)]=0;
        Read(a,fa);Read(b,fb);

        cout<<"a="<<endl;
        For(i,0,N){For(j,0,N)cout<<a[pos(i,j)]<<" ";cout<<endl;}
        cout<<"b="<<endl;
        For(i,0,N){For(j,0,N)cout<<b[pos(i,j)]<<" ";cout<<endl;}

        if (PAPI_start(EventSet) != PAPI_OK)exit(-1);
        if (PAPI_read(EventSet, values1) != PAPI_OK)exit(-1);
        work(a,b,c,fc);
        if (PAPI_stop(EventSet, values2) != PAPI_OK)exit(-1);
        for(int i=0;i<=5;++i)values[i]=values[i]+values2[i]-values1[i];

        Printing(c,fc);
        cout<<"c="<<endl;
        For(i,0,N){For(j,0,N)cout<<c[pos(i,j)]<<" ";cout<<endl;}
        free(a);free(b);free(c);
    
    }
    else
    {
        int*a,*b,*c;
        a=(int*)malloc(N*N*sizeof(int));
        b=(int*)malloc(N*N*sizeof(int));
        c=(int*)malloc(N*N*sizeof(int));
        Read(a,fa);Read(b,fb);

        cout<<"a="<<endl;
        For(i,0,N){For(j,0,N)cout<<a[pos(i,j)]<<" ";cout<<endl;}
        cout<<"b="<<endl;
        For(i,0,N){For(j,0,N)cout<<b[pos(i,j)]<<" ";cout<<endl;}

        if (PAPI_start(EventSet) != PAPI_OK)exit(-1);
        if (PAPI_read(EventSet, values1) != PAPI_OK)exit(-1);
        work(a,b,c,fc);
        if (PAPI_stop(EventSet, values2) != PAPI_OK)exit(-1);
        for(int i=0;i<=9;++i)values[i]=values[i]+values2[i]-values1[i];

        Printing(c,fc);
        cout<<"c="<<endl;
        For(i,0,N){For(j,0,N)cout<<c[pos(i,j)]<<" ";cout<<endl;}
        free(a);free(b);free(c);
    }
    
    fflush(fc);
    fclose(fa);fclose(fb);fclose(fc);

    if (PAPI_cleanup_eventset(EventSet) != PAPI_OK)exit(-1);
    if (PAPI_destroy_eventset(&EventSet) != PAPI_OK)exit(-1);
    PAPI_shutdown();

    
    //printf("L1 load=%lld\n",values[0]);
    //printf("L1 store=%lld\n",values[1]);
    printf("L1 cache=%lld\n",values[0]);

    //printf("L2 load=%lld\n",values[2]);
    //printf("L2 store=%lld\n",values[3]);
    printf("L2 cache=%lld\n",values[1]);
    return 0;
}
