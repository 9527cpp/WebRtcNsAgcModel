#include <stdio.h>
#include "noise_suppression.h"
#include <unistd.h>

//focus webrtc only process 10ms data
//so 160 short per sec if samplerate equal 16000
//numbands equal 1 if sample_rate is smaller than 32000 otherwise equal 2
#define SAMPLE_RATE 16000
#define SAMPLE_COUNT (SAMPLE_RATE/1000*10)


void * ANS_Init(int nSampleRate)
{
    void *ansInst = NULL;
    ansInst = WebRtcNs_Create();
    if (ansInst == NULL){
        printf("ans init failed\r\n");
    }
    if(0 != WebRtcNs_Init(ansInst,nSampleRate))
        goto FAILED;
    if(0 !=WebRtcNs_set_policy(ansInst,1))
        goto FAILED;
    return ansInst;
FAILED:
    WebRtcNs_Free(ansInst);
    ansInst = NULL;
    return ansInst;
}

/*
 *  nLen : 80 ¸ösample µÄÕ
 */
int ANS_Sample(void * pInstance,void * pBuffIn,void *pBuffOut,int nLen)
{
    int nTotal = (nLen / SAMPLE_COUNT);
    short * input = (short *)pBuffIn;
    short * output = (short *)pBuffOut;     
    int numbands = 2;
    /* short inputL [SAMPLE_COUNT/2]; */
    float outNear[SAMPLE_COUNT];
    float inNear[SAMPLE_COUNT];
    
    for(int j = 0;j< nTotal;j++){
        memset(inNear,0,sizeof(float)*SAMPLE_COUNT);
        memset(outNear,0,sizeof(float)*SAMPLE_COUNT);
        for(int i = 0;i < SAMPLE_COUNT;i++)
        {
            inNear[i] = *(input + i);
        }
        float *const p = inNear;
        const float * const*ptrp = &p;
        float *const q = outNear;
        float * const*ptrq = &q;
        WebRtcNs_Analyze(pInstance,ptrp);
        WebRtcNs_Process(pInstance,ptrp,numbands,ptrq);
        for(int i = 0;i < SAMPLE_COUNT;i++)
        {
            *(output +i) = (short)outNear[i];
        }
        input += SAMPLE_COUNT;
        output += SAMPLE_COUNT;
    }
    return 0;
}

void ANS_DeInit(void *pInstance)
{
    WebRtcNs_Free(pInstance);
    pInstance = NULL;
}

int main()
{
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;
    int file_len = 0;
    int process_len = 0;
    short in[SAMPLE_COUNT];
    short out[SAMPLE_COUNT];
    void * pInstance = NULL;
    if(!fp_in){
        fp_in = fopen("agc_in.pcm","rb");
    }
    if(!fp_out){
        fp_out = fopen("ans_out.pcm","wb");
    }

    if(!fp_in){
        printf("fp_in is not exist\r\n");
        return -1;
    }
    if(!fp_out){
        printf("fp_out is not exist\r\n");
        goto FAILED;
    }
    pInstance = ANS_Init(SAMPLE_RATE);
    if(!pInstance){
        printf("agc init falied\r\n");
        goto FAILED;
    }

    fseek(fp_in,0,SEEK_END);
    file_len = ftell(fp_in);
    fseek(fp_in,0,SEEK_SET);

    printf("begin agc\r\n");
    while(process_len < file_len){
        fread(in,SAMPLE_COUNT,sizeof(short),fp_in);
        ANS_Sample(pInstance,in,out,SAMPLE_COUNT);
        fwrite(out,SAMPLE_COUNT,sizeof(short),fp_out);
        process_len += SAMPLE_COUNT * sizeof(short);
    }
    printf("finish agc\r\n");
    fclose(fp_out);
FAILED:
    fclose(fp_in);
    ANS_DeInit(pInstance);
    return 0;
}
