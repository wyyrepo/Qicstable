/*----------------------------------------------------------------------------
* COPYRIGHT NOTICE
* Copyright (c) 2006 Integrated Computer Solutions
*
* ALL RIGHTS RESERVED.  This notice is  intended  as  a  precaution
* against  inadvertent publication, and shall not be deemed to con-
* stitute an acknowledgment that publication has  occurred  nor  to
* imply  any  waiver  of confidentiality.  The year included in the
* notice is the year of the creation of the work.
*----------------------------------------------------------------------------
*/

/* stype.c - simulate typing on a terminal

*/

#include	<stdio.h>
#include	<sys/file.h>
#include        <sys/fcntl.h>
#include        <termios.h>
#include	<sgtty.h>


main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	dev, err;

	if(argc != 3) {
		fprintf(stderr, "usage: stype dev text\n");
		exit(1);
	}

	dev = open(argv[1], O_RDWR, 0);

	if(dev < 0) {
		fprintf(stderr, "open fails %d\n", err);
		exit(1);
	}

	stype(dev, argv[2]);
	exit(0);
}

stype(dev, text)
	int	dev;
	char	*text;
{
	char	c;
	int	cc;

	if(strcmp(text, "-") == 0) {
		/* Use stdin */
		while((cc = fgetc(stdin)) >= 0) {
			c = cc;
			ioctl(dev, TIOCSTI, &c);
			if(c == '\r') {
				/* delay after CR */
				sleep(1);
			}
		}
	} else {
		while(c = *text++) {
			ioctl(dev, TIOCSTI, &c);
		}
	}
}
