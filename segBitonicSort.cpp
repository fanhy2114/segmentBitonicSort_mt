//����˵����
//�����ֳ�m�� ��n�������������������Ѱ��κ����򣬵�ÿ���ڲ� ������C/C++
//��дһ���ֶ�˫������(Bitonic sort)��������ÿһ���ڲ��ĸ������������򣬵�
//��Ҫ�ı�μ��λ�á�
//�ӿڷ�ʽ��
//void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int n, int m);
//
//���������У�data������Ҫ�ֶ������n��floatֵ��seg_id����data��n��Ԫ�ظ�
//�����ڵ� �α�š�seg_start����m+1��Ԫ�أ�ǰm���ֱ�� ��0..m-1��m���ε���
//ʼλ�ã�seg_start[m]��֤����n��
//
//seg_id�е�Ԫ�ر�֤�������½������������i<j,seg_id[i]<=seg_id[j]��
//seg_id����Ԫ �ؾ���0��m-1��Χ�ڡ�
//
//����������data����֤ÿһ�������򣬵����ı�μ�Ԫ�ص�˳��
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <sys/time.h>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <string.h>
#include <string>
using namespace std;

string in_file = "data.txt";
string out_file_single = "output_single.txt";
string out_file_mt = "output_mt.txt";

float Max = sqrt(-1.f);//��Ϊ�����
float* data;
int* seg_id;
int* seg_start;
int n,m;
int num_threads;
pthread_t* threads;
struct timeval tstart_single, tend_single,tstart_mt, tend_mt;

void generateData(int n, int m){
    // Initialization
    if(freopen(in_file.c_str(),"w", stdout)==NULL){
        fprintf(stderr,"Failed to open input file.\n");
        return ;
    }
    float* values = (float*)malloc(n*sizeof(float));
    int* ids =(int*)malloc(n*sizeof(int));
    int* starts=(int*)malloc((m+1)*sizeof(int));

    // Generate Data
    srand((unsigned int)time(NULL));
    for (int i=0; i<n; i++){
        values[i] = rand()%(11234567)*pow(-1,rand());
        ids[i] = rand()%m;
    }
    int count=0;
    for(int i=0; i< m;i++){
        starts[i]=(i==0)?count:count+1;
        for(int j=0; j< n; j++){
            if(ids[j] == i){
                count++;
            }
        }
    }
    starts[m]=n;

    // Output to input.txt
    int index = 0;
    cout<<n<<" "<<m<<endl;
    for(int i=0; i<n; i++){
        cout<<values[i]<<" ";
        if(i+1 == starts[index]){
            index++;
            cout<<endl;
        }
    }
    cout<<endl;

    for(int i=0; i<m; i++){
        for(int j=0; j<starts[i+1]-starts[i]; j++){
            cout<<i<<" ";
        }
        cout<<endl;
    }

    for(int i=0; i<=m; i++){
        cout<<starts[i]<<" ";
    }
    fflush(stdout);
    fclose(stdout);
}

bool init()
{
    if(freopen(in_file.c_str(), "r",stdin)==NULL){
        fprintf(stderr,"Failed to open input file.\n");
        return false;
    }
    scanf("%d", &n);
    scanf("%d", &m);
    ////�ж������Ƿ�����
    if(n <= 0 || m <= 0 || m > n)
    {
        fprintf(stderr,"Input error! Accurate conditions:n>m>0\n");
        return false;
    }
    data = (float*)malloc(n*sizeof(float));
    seg_id = (int*)malloc(n*sizeof(int));
    seg_start = (int*)malloc((m+1)*sizeof(int));

    for(int i=0; i<n; i++)
        scanf("%f", &(data[i]));
    for(int i=0; i<n; i++)
        scanf("%d", &(seg_id[i]));
    for(int i=0; i<=m; i++)
        scanf("%d", &(seg_start[i]));
    
    if(!(seg_start[m]== n && seg_id[n-1]==(m-1)))
    {
        fprintf(stderr,"%d---%d\n",seg_start[m],seg_id[n-1]);
        fprintf(stderr,"Input error! Accurate conditions: seg_start[m]==n, seg_id[n-1]==(m-1)\n");
        return false;
    }
    fflush(stdin);
    fclose(stdin);
    return true;
}

void show_time(){
    if(freopen("result.txt", "w", stdout)==NULL){
        fprintf(stderr,"Failed to open output file.\n");
        return ;
    }
    double exectime_single = (tend_single.tv_sec - tstart_single.tv_sec) * 1000.0; // sec to ms
    exectime_single += (tend_single.tv_usec - tstart_single.tv_usec) / 1000.0; // us to ms 
    printf("Sort with single Thread spent: %.3lf sec\n", exectime_single/1000);

    double exectime_mt = (tend_mt.tv_sec - tstart_mt.tv_sec) * 1000.0; // sec to ms
    exectime_mt += (tend_mt.tv_usec - tstart_mt.tv_usec) / 1000.0; // us to ms 
    printf("Sort with %d Thread spent: %.3lf sec\n", num_threads, exectime_mt/1000);

    printf("Sort with mt_threads is %f times faster than single thread.\n", exectime_single/exectime_mt);
    fflush(stdout);
    fclose(stdout);
}

void show_result(int state)
{
    string out_file = (state==1)?out_file_single:out_file_mt;
    if(freopen(out_file.c_str(), "w", stdout)==NULL){
        fprintf(stderr,"Failed to open output file.\n");
        return ;
    }
    int index = 1;
    for (int i = 0; i < n;++i) {
        cout << data[i] <<"  ";
        if( i+1 == seg_start[index]){
            index++;
            cout<<endl;
        }
    }
    cout << endl;
    fflush(stdout);
    fclose(stdout);
}


void* bitonicSort(void *para)
{
    int seg_index = *((int *)para);
    int len=1;
    int cur_len=seg_start[seg_index+1]-seg_start[seg_index];
    int start_point = seg_start[seg_index]-seg_start[0];
    while (len < cur_len)//ѭ���˳�������
        len = len << 1;//Ѱ�Ҵ���n����С��2���ݴη�len
    for (int i = 0; i < len-cur_len; i++)
        seg_id[i+start_point] =(int&)Max;
    ///�����������������
    for (int step = 2; step <= len; step <<= 1)
    {
        for (int i = 0; i < len; i += step<<1)//1
        {
            //�˴���������ڵ�forѭ������֮ǰ�Ĳ��ֿ��Է�װ��BitionicSort,Ϊ�����㲻ʹ�ú���������ֱ���滻��Դ��
            for (int step0 = step>>1; step0 >0; step0 >>= 1)// 2
            {
                for (int j = 0; j < step; j += step0<<1)//3
                {
                    for (int k = 0; k < step0; ++k)//4
                    {
                        int temp1=i+j+k;
                        int temp2=i+j+k+step0;
                        if(temp1>=cur_len)
                        {
                            temp1-=cur_len;
                            temp2-=cur_len;
                            float f_temp1=(float&)seg_id[temp1+start_point];
                            float f_temp2=(float&)seg_id[temp2+start_point];
                            if (f_temp1 > f_temp2||f_temp1!=f_temp1)  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                            {
                                //�����ú������˴��ֶ�����ʵ�֣�����data
                                int T=seg_id[temp1+start_point];
                                seg_id[temp1+start_point]=seg_id[temp2+start_point];
                                seg_id[temp2+start_point]=T;
                            }
                        }
                        else if(temp1<cur_len&&temp2>=cur_len)
                        {
                            temp2-=cur_len;
                            int temp3=seg_start[seg_index]-seg_start[0];
                            float f_temp=(float&)seg_id[temp2+start_point];
                            if (data[temp3+temp1] > f_temp||data[temp3+temp1]!=data[temp3+temp1])  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                            {
                                //�����ú������˴��ֶ�����ʵ�֣�����data
                                float T=data[temp1+temp3];
                                data[temp1+temp3]=f_temp;
                                seg_id[temp2+start_point]=(int&)T;
                            }
                        }
                        else if(temp1<cur_len&&temp2<cur_len)
                        {
                            int temp3=seg_start[seg_index]-seg_start[0];
                            if (data[temp1+temp3] > data[temp2+temp3]||data[temp1+temp3]!=data[temp1+temp3])  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                            {
                                //�����ú������˴��ֶ�����ʵ�֣�����data
                                float T=data[temp1+temp3];
                                data[temp1+temp3]=data[temp2+temp3];
                                data[temp2+temp3]=T;
                            }
                        }
                    }
                }
            }

            if (i + step < len)
            {
                for (int step0= step>>1;step0 >0 ;step0 >>= 1) //1
                {
                    for (int j = 0; j < step; j += step0<<1) //2
                    {
                        for (int k = 0; k < step0; ++k) //3
                        {
                            int temp1=i+j+k+step;
                            int temp2=i+j+k+step+step0;
                            if(temp1>=cur_len)
                            {
                                temp1-=cur_len;
                                temp2-=cur_len;
                                float f_temp1=(float&)seg_id[temp1+start_point];
                                float f_temp2=(float&)seg_id[temp2+start_point];
                                if (f_temp1 < f_temp2||f_temp2!=f_temp2)  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                                {
                                    //�����ú������˴��ֶ�����ʵ�֣�����data
                                    int T=seg_id[temp1+start_point];
                                    seg_id[temp1+start_point]=seg_id[temp2+start_point];
                                    seg_id[temp2+start_point]=T;
                                }
                            }
                            else if(temp1<cur_len&&temp2>=cur_len)
                            {
                                temp2-=cur_len;
                                int temp3=seg_start[seg_index]-seg_start[0];
                                float f_temp=(float&)seg_id[temp2+start_point];
                                if (data[temp3+temp1] < f_temp||f_temp!=f_temp)  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                                {
                                    //�����ú������˴��ֶ�����ʵ�֣�����data
                                    float T=data[temp1+temp3];
                                    data[temp1+temp3]=f_temp;
                                    seg_id[temp2+start_point]=(int&)T;
                                }
                            }
                            else if(temp1<cur_len&&temp2<cur_len)
                            {
                                int temp3=seg_start[seg_index]-seg_start[0];
                                if (data[temp1+temp3] < data[temp2+temp3]||data[temp2+temp3]!=data[temp2+temp3])  //��������ʹ��������,ͬʱ�ж϶���֮���Ƿ���NaN
                                {
                                    //�����ú������˴��ֶ�����ʵ�֣�����data
                                    float T=data[temp1+temp3];
                                    data[temp1+temp3]=data[temp2+temp3];
                                    data[temp2+temp3]=T;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void segmentedBitonicSort(int state)
{
    if(!init()){
        fprintf(stderr,"Initialization failed.\n");
        return ;
    }
    if(state==1){
        // Linear ranking
        num_threads = 1;
        for(int i=0; i<m; i++){
            int* index =(int*)malloc(sizeof(int));
            *index =i; 
            bitonicSort((void*)index);
        }
    }
    else if(state==2){
        // With multi-thread
        int rv;
        num_threads = (int)thread::hardware_concurrency()-2;
        if(num_threads >= m){
            num_threads = m;
        }
        threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
        for(int i=0; i<num_threads; ++i){
            int* tid = (int*)malloc(sizeof(int));
            *tid = i;
            rv = pthread_create(&threads[i], 0, bitonicSort, (void *)tid );
            if(rv){
                fprintf(stderr, "Error: Unable to create thread.\n");
                exit(-1);
            }
        }
        for(int i=0; i<num_threads; i++){
            pthread_join(threads[i], NULL);
        }
    }
    else{
        fprintf(stderr,"Wrong Execute state.\n");
        return ;
    }
    show_result(state);
}



int  main()
{
    // ��������
    // float data[11]={ 0,sqrt(-1.f) -100 , 2, 100, 4, 0.5,sqrt(-1.f), sqrt(-1.f), 3, 0.1, 2};
    // int seg_id[11]={0, 0, 0, 1, 1, 2, 2, 2, 2,3,3};
    // int seg_start[5]={0,3,5,9,11};
    // int n=11;
    // int m=4;

    // generate data
    generateData(10000000,10);
    // ���÷ֶ�˫��������-����ִ��
    gettimeofday(&tstart_single, NULL);
    segmentedBitonicSort(1);
    gettimeofday(&tend_single, NULL);

    // ���÷ֶ�˫��������-����ִ��
    gettimeofday(&tstart_mt, NULL);
    segmentedBitonicSort(2);
    gettimeofday(&tend_mt, NULL);
    
    show_time();
    return 0;
}
