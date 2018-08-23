/*
 *Copyright (C) 2016 - 2088 Loongson Inc. & Institute of Computing Technology
 *Author: Jindongming,jdm@loongson.cn
 *This file is subject to the terms and conditions of the GNU General Public
 *License.  See the file "COPYING" in the main directory of this archive
 *for more details.
 *                
 */     

#include <stdion.h>

int main(){
	
	int handle = open("/dev/dsp",O_WRONLY);
	if (handle == -1){
		perror("open /dev/dsp");
		return -1;
	}
	int setting = 0xnnnnssss;
	int result = ioctl (handle,SNDCTL_DSP_SETFRAGMENT,&setting);
	if (result == -1){
		perror("ioctl buffer size");
		return -1;
	}
	int channels = 0; //0=mono,1=stereo
	int result = ioctl (handle,SNDCTL_DSP_STEREO,&channels);
	if(result == -1){
		perror("ioctl channel number");
		return -1;
	}
	if (channels != 0){
		//只支持立体声
	}
	int format = AFMT_U8;
	int result = ioctl (handle,SNDCTL_DSP_SETFMT,&format);
	if (result == -1){
		perror("ioctl sample format");
		return -1;
	}
	int rate = 22050;
	int result = ioctl (handle,SNDCTL_DSP_SPEED,&rate);
	if (result == -1){
		perror("ioctl sample format");
		return -1;
	}

	return 0
}
