#include <stdio.h>
#include "gain_control.h"
#include <unistd.h>

void * AGC_Init(int nSampleRate)
{
    void *agcInst = NULL;
    agcInst = WebRtcAgc_Create();
    if (agcInst == NULL) return agcInst ;
    int status = WebRtcAgc_Init(agcInst, 0, 255, kAgcModeAdaptiveDigital, nSampleRate);
    if(status != 0){WebRtcAgc_Free(agcInst);agcInst=NULL;}
    WebRtcAgcConfig agcConfig;
    agcConfig.compressionGaindB = 9; // default 9 dB
    agcConfig.limiterEnable = 1; // default kAgcTrue (on)
    agcConfig.targetLevelDbfs = 1; // default 3 (-3 dBOv)
    status = WebRtcAgc_set_config(agcInst, agcConfig);
    if (status != 0) {WebRtcAgc_Free(agcInst);agcInst=NULL;}
    return agcInst;
}

/*
 *  nLen : 80 ¸ösample µÄÕ
 */
int AGC_Sample(void * pInstance,void * pBuffIn,void *pBuffOut,int nLen)
{
    const int SAMPLE_COUNT = 160;
    int nTotal = (nLen / SAMPLE_COUNT);
    short * input = (short *)pBuffIn;
    short * output = (short *)pBuffOut;     
    int num_bands = 1;
    int inMicLevel = 0, outMicLevel = -1;
    unsigned char saturationWarning = 1;
    short echo = 0; 

    int gains[11] = {};

    for (int i = 0; i < nTotal; i++) 
    {
        int nAgcRet= 0;
        inMicLevel = 0;
        memset(gains,0,sizeof(gains)/sizeof(int));
        int ret = WebRtcAgc_Analyze(pInstance, &input, num_bands, SAMPLE_COUNT, inMicLevel, &outMicLevel,echo, &saturationWarning, gains);
        /* for(int i = 0;i<11;i++){ */
        /*     printf("%x,",gains[i]); */
        /* } */
        /* printf("in mic level:%d,out mic level:%d\r\n",inMicLevel,outMicLevel); */
        if(ret == 0){
            nAgcRet = WebRtcAgc_Process(pInstance, gains,(const short *const *) &input, num_bands, (short *const *) &output);
        }
        if (nAgcRet != 0) 
        {
            printf("failed in WebRtcAgc_Process\r\n");
            WebRtcAgc_Free(pInstance);
            return -1;
        }
        input += SAMPLE_COUNT;
        output += SAMPLE_COUNT;
    }
    return 0;
}

void AGC_DeInit(void *pInstance)
{
    WebRtcAgc_Free(pInstance);
    pInstance = NULL;
}

int main()
{
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;
    int file_len = 0;
    int process_len = 0;
    const int SAMPLE_COUNT = 160;
    short in[SAMPLE_COUNT];
    short out[SAMPLE_COUNT];
    void * pInstance = NULL;
    if(!fp_in){
        fp_in = fopen("agc_in.pcm","rb");
    }
    if(!fp_out){
        fp_out = fopen("agc_out.pcm","wb");
    }

    if(!fp_in){
        printf("fp_in is not exist\r\n");
        return -1;
    }
    if(!fp_out){
        printf("fp_out is not exist\r\n");
        goto FAILED;
    }
    pInstance = AGC_Init(16000);
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
        AGC_Sample(pInstance,in,out,SAMPLE_COUNT);
        fwrite(out,SAMPLE_COUNT,sizeof(short),fp_out);
        process_len += SAMPLE_COUNT * sizeof(short);
    }
    printf("finish agc\r\n");
    fclose(fp_out);
FAILED:
    fclose(fp_in);
    AGC_DeInit(pInstance);
    return 0;
}
