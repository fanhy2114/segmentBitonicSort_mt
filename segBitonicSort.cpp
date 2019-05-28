//问题说明：
//给出分成m段 的n个浮点数，输入数据已按段号有序，但每段内部 无序。用C/C++
//编写一个分段双调排序(Bitonic sort)函数，对每一段内部的浮点数进行排序，但
//不要改变段间的位置。
//接口方式：
//void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
//n, int m);
//
//输入数据中，data包含需要分段排序的n个float值，seg_id给出data中n个元素各
//自所在的 段编号。seg_start共有m+1个元素，前m个分别给 出0..m-1共m个段的起
//始位置，seg_start[m]保证等于n。
//
//seg_id中的元素保证单调不下降，即对任意的i<j,seg_id[i]<=seg_id[j]。
//seg_id所有元 素均在0到m-1范围内。
//
//输出结果覆盖data，保证每一段内排序，但不改变段间元素的顺序
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <thread>
using namespace std;

float Max = sqrt(-1.f);//作为填充数
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
    ////判断输入是否有误
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
    while (len < cur_len)//循环退出的条件
        len = len << 1;//寻找大于n的最小的2的幂次方len
    for (int i = 0; i < len-cur_len; i++)
        seg_id[i+start_point] =(int&)Max;
    ///对整个数组进行排序
    for (int step = 2; step <= len; step <<= 1)
    {
        for (int i = 0; i < len; i += step<<1)//1
        {
            //此处到上面紧邻的for循环结束之前的部分可以封装成BitionicSort,为了满足不使用函数条件，直接替换成源码
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
                            if (f_temp1 > f_temp2||f_temp1!=f_temp1)  //交换数据使升序排列,同时判断二者之中是否有NaN
                            {
                                //不调用函数，此处手动代码实现，交换data
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
                            if (data[temp3+temp1] > f_temp||data[temp3+temp1]!=data[temp3+temp1])  //交换数据使升序排列,同时判断二者之中是否有NaN
                            {
                                //不调用函数，此处手动代码实现，交换data
                                float T=data[temp1+temp3];
                                data[temp1+temp3]=f_temp;
                                seg_id[temp2+start_point]=(int&)T;
                            }
                        }
                        else if(temp1<cur_len&&temp2<cur_len)
                        {
                            int temp3=seg_start[seg_index]-seg_start[0];
                            if (data[temp1+temp3] > data[temp2+temp3]||data[temp1+temp3]!=data[temp1+temp3])  //交换数据使升序排列,同时判断二者之中是否有NaN
                            {
                                //不调用函数，此处手动代码实现，交换data
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
                                if (f_temp1 < f_temp2||f_temp2!=f_temp2)  //交换数据使升序排列,同时判断二者之中是否有NaN
                                {
                                    //不调用函数，此处手动代码实现，交换data
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
                                if (data[temp3+temp1] < f_temp||f_temp!=f_temp)  //交换数据使升序排列,同时判断二者之中是否有NaN
                                {
                                    //不调用函数，此处手动代码实现，交换data
                                    float T=data[temp1+temp3];
                                    data[temp1+temp3]=f_temp;
                                    seg_id[temp2+start_point]=(int&)T;
                                }
                            }
                            else if(temp1<cur_len&&temp2<cur_len)
                            {
                                int temp3=seg_start[seg_index]-seg_start[0];
                                if (data[temp1+temp3] < data[temp2+temp3]||data[temp2+temp3]!=data[temp2+temp3])  //交换数据使升序排列,同时判断二者之中是否有NaN
                                {
                                    //不调用函数，此处手动代码实现，交换data
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
    //    测试用例1
//        float data[5]={0.8,0.2,0.4,0.6,0.5};
//        int seg_id[5]={0, 0, 1, 1, 1};
//        int seg_start[3]={0,2,5};
//        int n=5;
//        int m=2;

//        测试用例2
        // float data[11]={ 0,sqrt(-1.f) -100 , 2, 100, 4, 0.5,sqrt(-1.f), sqrt(-1.f), 3, 0.1, 2};
        // int seg_id[11]={0, 0, 0, 1, 1, 2, 2, 2, 2,3,3};
        // int seg_start[5]={0,3,5,9,11};
        // int n=11;
        // int m=4;

//        测试用例3
    //    float data[22]={ 0,sqrt(-1.f) -100 , 2, 100, 4, 0.5,sqrt(-1.f), sqrt(-1.f), 3, 0.1, 2,0.2,sqrt(-1.f) -100 , 2.2, 102, 4.2, 0.7,sqrt(-1.f), sqrt(-1.f)+1, 3.2, 0.3, 2.2};
    //    int seg_id[22]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //    int seg_start[2]={0,22};
    //    int n=22;
    //    int m=1;
    // 初始化
    if(!init()){
        fprintf(stderr,"Initialization failed.\n");
        return 0;
    }
    // 调用分段双调排序函数
    segmentedBitonicSort();
    // 输出
    show_result();
    return 0;
}
