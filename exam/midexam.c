#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>




int main(){ 
	int sw, cpsw, y, ChunkSize, SubChunkSize;
	char buf[20];

	//원본 음성 파일 오픈
	sw = open("swvader03.wav", O_RDONLY, 00777);

	//수정된 음성 파일 생성
	cpsw = open("sw2.wav", O_RDWR | O_CREAT | O_TRUNC, 00777);
	
	lseek(sw, 4, SEEK_SET);
	y = read(sw, &ChunkSize, 4);

	//SubChunkSize 를구하고 반으로 나누기
	lseek(sw, 40, SEEK_SET);
	y = read(sw, &SubChunkSize, 4);

	SubChunkSize /= 2;
	ChunkSize -= SubChunkSize;//데이터가 사라진만큼 ChunkSize도 수정

	//Riff 헤더, SubChunk 부분을 그대로 복사
	lseek(sw, 0, SEEK_SET);
	for(int i = 0; i<44; i++){
		y = read(sw, buf, 20);
		if(y == 0) break;
		write(cpsw, buf, y);
	}

	//ChunkSize와 SubChunkSize 데이터를 변경
	lseek(cpsw, 4, SEEK_SET);
	write(cpsw, &ChunkSize, 4);
	lseek(cpsw, 40, SEEK_SET);
	write(cpsw, &SubChunkSize, 4);




	//음성 데이터 중 Master 부분만복사
	lseek(sw, 44 + SubChunkSize, SEEK_SET);
	lseek(cpsw, 44, SEEK_SET);
	for(;;){
		y = read(sw, buf, 20);
		if(y == 0) break;
		write(cpsw, buf, y);
	}

}
