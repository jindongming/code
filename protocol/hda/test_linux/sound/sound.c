#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#include <termios.h>


#define LENGTH    10   //¼��ʱ��,��
#define RATE    88200 //����Ƶ��
#define SIZE    16   //����λ��
#define CHANNELS 2   //������Ŀ
#define RSIZE    8    //buf�Ĵ�С��


/********������wave��ʽ�ļ����ļ�ͷ��ʽ˵��******/
/*------------------------------------------------
|             RIFF WAVE Chunk                  |
|             ID = 'RIFF'                     |
|             RiffType = 'WAVE'                |
------------------------------------------------
|             Format Chunk                     |
|             ID = 'fmt '                      |
------------------------------------------------
|             Fact Chunk(optional)             |
|             ID = 'fact'                      |
------------------------------------------------
|             Data Chunk                       |
|             ID = 'data'                      |
------------------------------------------------*/
/**********������wave�ļ���ʽͷ��ʽ˵��***********/
/*wave �ļ�һ�����ĸ�Chunk��ɣ����е�����Chunk����ʡ�ԣ�ÿ��Chunk�б�ʾ��ID��,
��С��size,���Ǳ�Chunk�����ݲ��ֳ��ȣ�,�������������*/
struct fhead
{
 /****RIFF WAVE CHUNK*/
 unsigned char a[4];//�ĸ��ֽڴ��'R','I','F','F'
 long int b;        //�����ļ��ĳ���-8;ÿ��Chunk��size�ֶΣ����Ǳ�ʾ���˱�Chunk��ID��SIZE�ֶ���ĳ���;
 unsigned char c[4];//�ĸ��ֽڴ��'W','A','V','E'
 /****RIFF WAVE CHUNK*/
 /****Format CHUNK*/
 unsigned char d[4];//�ĸ��ֽڴ��'f','m','t',''
 long int e;       //16��û�и�����Ϣ��18���и�����Ϣ��һ��Ϊ16��������ʽת���Ļ�Ϊ18
 short int f;       //���뷽ʽ��һ��Ϊ0x0001;
 short int g;       //������Ŀ��1��������2˫����;
 long int h;        //����Ƶ��;
 long int i;        //ÿ�������ֽ���;
 short int j;       //ÿ��������Ҫ�����ֽڣ���������˫��������һ����;
 short int k;       //������λ��
 /****Format CHUNK*/
 /***Data Chunk**/
    unsigned char p[4];//�ĸ��ֽڴ��'d','a','t','a' 
 long int q;        //�������ݲ��ֳ��ȣ��������ļ�ͷ���κβ���
}wavehead;//����WAVE�ļ����ļ�ͷ�ṹ��

int main(void)
{
 /*����wave �ļ�ͷ��ֵ*/
 wavehead.a[0]='R';
 wavehead.a[1]='I';
 wavehead.a[2]='F';
 wavehead.a[3]='F';
 wavehead.b=LENGTH*RATE*CHANNELS*SIZE/8-8;
 wavehead.c[0]='W';
 wavehead.c[1]='A';
 wavehead.c[2]='V';
 wavehead.c[3]='E';
 wavehead.d[0]='f';
 wavehead.d[1]='m';
 wavehead.d[2]='t';
 wavehead.d[3]=' ';
 wavehead.e=16;
 wavehead.f=1;
 wavehead.g=CHANNELS;
 wavehead.h=RATE;
 wavehead.i=RATE*CHANNELS*SIZE/8;
 wavehead.j=CHANNELS*SIZE/8;
 wavehead.k=SIZE;
 wavehead.p[0]='d';
 wavehead.p[1]='a';
 wavehead.p[2]='t';
 wavehead.p[3]='a';
 wavehead.q=LENGTH*RATE*CHANNELS*SIZE/8;
 /*����wave �ļ�ͷ��ֵ*/
  
 int i;
 unsigned char buf[RSIZE]; //ÿ��ѭ��ȡ��RSIZE��С������������buf��Ȼ��д���ļ����������෴��
  
 //�������豸��ֻ����ʽ������������������
 int fd_dev_r= open("/dev/dsp", O_RDONLY,0777);
 if (fd_dev_r < 0) 
 {
    perror("Cannot open /dev/dsp device");
    return 1;
 }
 
 int arg = SIZE;
 if (ioctl(fd_dev_r, SOUND_PCM_WRITE_BITS, &arg) == -1)    //��������λ��   
 {
        perror("Cannot set SOUND_PCM_WRITE_BITS ");
        return 1;
 }
 
 arg = CHANNELS;
 if (ioctl(fd_dev_r, SOUND_PCM_WRITE_CHANNELS, &arg) == -1)   //����������
 {
        perror("Cannot set SOUND_PCM_WRITE_CHANNELS");
        return 1;
 }
 arg = RATE;
 if (ioctl(fd_dev_r, SOUND_PCM_WRITE_RATE, &arg) == -1)      //���ò�����
 {
        perror("Cannot set SOUND_PCM_WRITE_WRITE");
        return 1;
 }
 
 //�������豸��ֻд��ʽ������������������ 
 int fd_dev_w = open("/dev/dsp", O_WRONLY,0777);
 if (fd_dev_w < 0)
 {
        perror("Cannot open /dev/dsp device");
        return 1;
 }
 arg = SIZE;
 if (ioctl(fd_dev_w, SOUND_PCM_WRITE_BITS, &arg) == -1)      //��������λ��
 {
       perror("Cannot set SOUND_PCM_WRITE_BITS ");
       return 1;
 }
 arg = CHANNELS;       
 if (ioctl(fd_dev_w, SOUND_PCM_WRITE_CHANNELS, &arg) == -1)   //���������� 
 {
        perror("Cannot set SOUND_PCM_WRITE_CHANNELS");
        return 1;
 }
 arg = RATE;        
 if (ioctl(fd_dev_w, SOUND_PCM_WRITE_RATE, &arg) == -1)       //���ò�����
 {
        perror("Cannot set SOUND_PCM_WRITE_WRITE");
        return 1;
 }
 
 /**���¿�ʼ¼��**/
 system("rm -f sound.wav");//������е�wav¼���ļ�
 int fd_f;
    if(( fd_f = open("./sound.wav", O_CREAT|O_RDWR,0777))==-1)//����һ��wave��ʽ�����ļ�
    {
        perror("cannot creat the sound file");
    }
    if(write(fd_f, &wavehead, sizeof(wavehead))==-1)//д��wave�ļ����ļ�ͷ
    {
       perror("write to sound'head wrong!!");
    }
 for(i=0;i<(LENGTH*RATE*SIZE*CHANNELS/8)/RSIZE;i++)//ÿ�δ��������RSIZE��С�����ݣ���ѭ������������/RSIZE��
 {
 
    
    if (read(fd_dev_r, buf, sizeof(buf)) != sizeof(buf))
    {
      perror("read wrong number of bytes");
    }
    if(write(fd_f, buf, sizeof(buf))==-1)
    {
          perror("write to sound wrong!!");
    }    
 }
 close(fd_dev_r);//�ر�ֻ����ʽ������
 close(fd_f);//�ر�wave�ļ�
 printf("play your sound now??\n y OR n\n");
 /***�����ǲ���wav�����ļ�**/
 if(getchar()=='y')
 {
    printf("Play...:\n");
    if(( fd_f = open("./sound.wav", O_RDONLY,0777))==-1)
    {
       perror("cannot creat the sound file");
    }
    lseek(fd_f,44,SEEK_SET);
    for(i=0;i<(LENGTH*RATE*SIZE*CHANNELS/8)/RSIZE;i++)
    { 
      
        if (read(fd_f, buf, sizeof(buf)) != sizeof(buf))   //�������ļ�
        perror("write wrong number of bytes"); 
             
        if (write(fd_dev_w, buf, sizeof(buf)) != sizeof(buf)) //����������
        perror("wrote2 wrong number of bytes");
    }
    close(fd_f);
    close(fd_dev_w);
    return 0;
 }
 else
 {
    printf("bye!\n");
    return 0;
 }


}