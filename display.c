#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>

// Error checking function that will close file and exit if there is an error
void errorchecking(int fd){
	printf("Error %d\n", errno);
	close(fd);
	exit(1);
}

// Setup the 7-segment i2C display
int init_display(void){

	unsigned char buffer[17];
	int fd;
	int result;
	char i2c_device[] = "/dev/i2c-1";

	//open file descriptor
	fd = open(i2c_device, O_RDWR);
	if(fd < 0){
		errorchecking(fd);
	}

	//set slave address
	result = ioctl(fd, I2C_SLAVE, 0x70);
	if(result < 0){
		errorchecking(fd);
	}

	//turn on oscillator
	buffer[0] = (0x2 << 4)|(0x1);
	result = write(fd, buffer, 1);
	if(result < 0){
		errorchecking(fd);
	}

	//turn on display, no blink
	buffer[0] = (0x8 << 4 | 0x1);
	result = write(fd, buffer, 1);
	if(result < 0){
		errorchecking(fd);
	}


	//set brightness
	buffer[0] = (0xE << 4 | 0xF);
	result = write(fd, buffer, 1);
	if(result < 0){
		errorchecking(fd);
	}

	// return file descriptor later used for writing values
	return fd;
}

uint8_t seg_numbers(uint8_t number){
	//switch case to easily get values for the 7 segment display
	switch(number){
	case 0:
		number = 0x3F; //ABCDEF for 0
		break;
	case 1:
		number = 0x06; //BC for 1
		break;
	case 2:
		number = 0x5B; //ABDEG for 2
		break;
	case 3:
		number = 0x4F; //ABCDG for 3
		break;
	case 4:
		number = 0x66; //BCFG for 4
		break;
	case 5:
		number = 0x6D; //ACFG for 5
		break;
	case 6:
		number = 0x7D; //ACDEFG for 6
		break;
	case 7:
		number = 0x07; //ABC for 7
		break;
	case 8:
		number = 0x7F; //ABCDEFG for 8
		break;
	case 9:
		number = 0x6F; //ABCFG for 9
		break;
	default:
		break;
	}

	return number;
}

int display_oz(int fd, int button_value){
	char buffer[17];	//buffer to be used for values
	sprintf(buffer, "%d", button_value);	//decoding the value into a string to easily manipulate it for displaying

	int result;

	uint8_t tens, ones;
	uint8_t display_tens, display_ones;

	// ones digit can display values less than 10
	if(button_value < 10){
		tens = 0x00;	//getting first digit
		ones = buffer[0] - '0';	//subtracting the first string item in buffer by the ascii character of 0 which prevents the need for a division by 10
	    display_tens = 0x00;		//dont needs tens for this case
	}

	// need to use both digits to display values larger than 10
	else{
		tens = buffer[0] - '0';
		ones = buffer[1] - '0';
		display_tens = seg_numbers(tens);
	}
		display_ones = seg_numbers(ones);

	buffer[0] = 0x00;
	buffer[1] = display_tens;   //display the tens
	buffer[2] = 0x00;
	buffer[3] = display_ones;   //display the ones 
	buffer[4] = 0x00;
	buffer[5] = 0x00;   //middle colon(off)
	buffer[6] = 0x00;
	buffer[7] = 0x3F;	//display O for Oz (ABCDEF)
	buffer[8] = 0x00;
	buffer[9] = 0x5B;	//display Z (technically 2) for Oz (ABGED)

	// write the buffer to the display
	result = write(fd, buffer, 17);
	if(result < 0){ //error check
		errorchecking(fd);
	}
    return 0;
}

// Display the word "Pour" on the display
int display_pouring(int fd){

    int result;
    char buffer[17];

    buffer[0] = 0x00;
	buffer[1] = 0x73;   //display P for POUr 
	buffer[2] = 0x00;
	buffer[3] = 0x3F;   //display O for POUr
	buffer[4] = 0x00;
	buffer[5] = 0x00;   //middle colon(off)
	buffer[6] = 0x00;
	buffer[7] = 0x3E;	//display U for POUr
	buffer[8] = 0x00;
	buffer[9] = 0x50;	//display r for POUr

	result = write(fd, buffer, 17);
	if(result < 0){ //error check
		errorchecking(fd);
	}
    return 0;
}

// Shutdown the display by closing the file and exiting
int shutdown_display(int fd) {
	close(fd);
	exit(1);
	return 0;
}

