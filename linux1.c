/*Linux programming*/
/// C library headers
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
int main()
{
  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
 int i,j,f=0,c;
 char rfid[13],time_date[30],buf[10];
 char name[20],em_id[20],rf_id[13],count[5];
 while(1)
  {
  int serial_port = open("/dev/ttyUSB0", O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 100;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }

// Write to serial port
//  unsigned char msg[50] ={'v','e','c','t','o','r','\n'};
//  write(serial_port, msg, sizeof(msg));
// int i;
//  usleep(100);
  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
  }
//  printf("%s\n",read_buf);
for(i=0,j=0;i<12;i++,j++)
      rfid[j]=read_buf[i];
      rfid[j]='\0';
for(i=14,j=0;read_buf[i];i++,j++)
	time_date[j]=read_buf[i];
	time_date[j]='\0';

	FILE *fp=fopen("employee_details","r+");
	FILE *fd=fopen("database","a+");
	while(fscanf(fp,"%s %s %s %s",name,em_id,rf_id,count)!=EOF)
             {
		     if(strcmp(rfid,rf_id)==0)
		     {
			     f=1;
		     c=atoi(count);
		     c++;

		     if(c%2!=0)		     {
			     fprintf(fd,"%s %s %s %s\n","IN",name,em_id,time_date);
		     }
		     else
			     fprintf(fd,"%s %s %s %s\n","OUT",name,em_id,time_date);

			     sprintf(buf,"%d",c);
			     fseek(fp,-1,SEEK_CUR);
			     fprintf(fp,"%s",buf);
			     printf("card scaned successfuly\n");
		     }
	     }
	     fclose(fp);
	     fclose(fd);

 if(f)
 {
	 printf("valid user\n");
	 f=0;
 }
 else
	 printf("invalid user\n");

  close(serial_port);
}
}

