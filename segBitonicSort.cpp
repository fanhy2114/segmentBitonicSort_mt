//����˵����
//�����ֳ�m�� ��n�������������������Ѱ��κ����򣬵�ÿ���ڲ� ������C/C++
//��дһ���ֶ�˫������(Bitonic sort)��������ÿһ���ڲ��ĸ������������򣬵�
//��Ҫ�ı�μ��λ�á�
//�ӿڷ�ʽ��
//void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
//n, int m);
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
using namespace std;

float Max = sqrt(-1.f);//��Ϊ�����
float* data;
int* seg_id;
int* seg_start;
int n,m;

bool init()
{
    if(freopen("input.txt", "r",stdin)==NULL){
        fprintf(stderr,"Failed to open input file.\n");
        return false;
    }
    scanf("%d", &n);
    scanf("%d", &m);
    ////�ж������Ƿ�����
    if(n <= 0 || m <= 0 || m>n)
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
        fprintf(stderr,"Input error! Accurate conditions: seg_start[m]==n, seg_id[n-1]==(m-1)\n");
        return false;
    }
    fflush(stdin);
    fclose(stdin);
    return true;
}


void show_result()
{
    if(freopen("output.txt","w", stdout)==NULL){
        fprintf(stderr,"Failed to open output file.\n");
        return ;
    }
    for (int i = 0; i < n;++i) {
        cout << data[i] <<"  ";
        if( (i+1) % 25 == 0){
            cout<<endl;
        }
    }
    cout << endl;
    fflush(stdout);
    fclose(stdout);
}


void bitonicSort(int seg_index)
{
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


void segmentedBitonicSort()
{
    for(int d=0; d < m; d++){
        thread t (bitonicSort, d);
        t.detach();
    }
}



int  main()
{
    //    ��������1
//        float data[5]={0.8,0.2,0.4,0.6,0.5};
//        int seg_id[5]={0, 0, 1, 1, 1};
//        int seg_start[3]={0,2,5};
//        int n=5;
//        int m=2;

//        ��������2
        // float data[11]={ 0,sqrt(-1.f) -100 , 2, 100, 4, 0.5,sqrt(-1.f), sqrt(-1.f), 3, 0.1, 2};
        // int seg_id[11]={0, 0, 0, 1, 1, 2, 2, 2, 2,3,3};
        // int seg_start[5]={0,3,5,9,11};
        // int n=11;
        // int m=4;

//        ��������3
    //    float data[22]={ 0,sqrt(-1.f) -100 , 2, 100, 4, 0.5,sqrt(-1.f), sqrt(-1.f), 3, 0.1, 2,0.2,sqrt(-1.f) -100 , 2.2, 102, 4.2, 0.7,sqrt(-1.f), sqrt(-1.f)+1, 3.2, 0.3, 2.2};
    //    int seg_id[22]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //    int seg_start[2]={0,22};
    //    int n=22;
    //    int m=1;
    // ��ʼ��
    if(!init()){
        fprintf(stderr,"Initialization failed.\n");
        return 0;
    }
    // ���÷ֶ�˫��������
    segmentedBitonicSort();
    // ���
    show_result();
    return 0;
}
