#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>

#define BUFFSIZE (64*1024)

char *devname = "/dev/dsp";

typedef struct {
  int file_descriptor;
  
  int bits;
  int channels;
  int speed;

  short buf[BUFFSIZE];
  double freq;

  double phase;
  int delay;

} SOUND_DEVICE;

void initialize_sound_device(SOUND_DEVICE *sound) {
  sound->phase = 0.0;
  sound->delay = 0;
  sound->freq = 200;

  sound->speed = 48000;
  sound->channels = 2;
  sound->bits = AFMT_S16_NE;

  if ((sound->file_descriptor = open (devname, O_WRONLY, 0)) == -1) {
      perror (devname);
      exit (-1);
    }

  if (ioctl (sound->file_descriptor, SNDCTL_DSP_CHANNELS, &(sound->channels)) == -1) {
      perror ("SNDCTL_DSP_CHANNELS");
      exit (-1);
    }

  if (ioctl (sound->file_descriptor, SNDCTL_DSP_SETFMT, &(sound->bits)) == -1) {
      perror ("SNDCTL_DSP_SETFMT");
      exit (-1);
    }

  if (sound->bits != AFMT_S16_NE) {
      fprintf (stderr,
	       "Device %s doesn't support 16 bit (native endian) format\n",
	       devname);
      exit (-1);
    }

  if (ioctl (sound->file_descriptor, SNDCTL_DSP_SPEED, &(sound->speed)) == -1) {
      perror ("SNDCTL_DSP_SPEED");
      exit (-1);
    }

  fprintf (stderr, "Outputting sweep at %d Hz, %d channels, 16 bits\n", 
           sound->speed,
	   sound->channels);
}

void play_sweep(SOUND_DEVICE *sound) {
  int p=0;

  sound->freq = 200.0;

  while (1) {
      int v;
      double step;

      v = sin (sound->phase) * 16483.0;
      
      int i;
      for (i = 0; i < sound->channels; i++) {
	  sound->buf[p++] = v;

	  if (p >= BUFFSIZE) {
	    /*write (1, buf, p * 2);*/

	      if (write (sound->file_descriptor, sound->buf, p * 2) != p * 2) {
		  perror ("write");
		  exit (-1);
		}
	      if (sound->delay > 0) {
		usleep (sound->delay * 1000);
              }
	      fprintf (stderr, "\r%d ", (int) (sound->freq));
	      fflush (stderr);
	      p = 0;
	    }
	}

      step = 2.0 * 3.1415 * (sound->freq) / (double) (sound->speed);

      sound->phase += step;

      if ((sound->freq) < (double) (sound->speed) / 2.1) {
	(sound->freq) *= 1.000002;
      }
    }

  fprintf (stderr, "\n");
  
  if (p > 0)
    write (sound->file_descriptor, sound->buf, p * 2);

}

void play_freq(SOUND_DEVICE *sound, double freq, double duration) {
  int p=0;
  clock_t start, end;
  double elapsed;

  sound->freq = freq;

  start = clock();
  end = start;

  do {
      int v;
      double step;

      v = sin (sound->phase) * 16483.0;
      
      int i;
      for (i = 0; i < sound->channels; i++) {
	  sound->buf[p++] = v;

	  if (p >= BUFFSIZE) {
	    /*write (1, buf, p * 2);*/

	      if (write (sound->file_descriptor, sound->buf, p * 2) != p * 2) {
		  perror ("write");
		  exit (-1);
		}
	      if (sound->delay > 0) {
		usleep (sound->delay * 1000);
              }
	      /*
              fprintf (stderr, "\r%d ", (int) (sound->freq));
	      fflush (stderr);
              */
	      p = 0;
	    }
	}

      step = 2.0 * 3.1415 * (sound->freq) / (double) (sound->speed);
      sound->phase += step;

      end = clock();
      elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

      /*printf("elapsed: %f - duration: %f\n", elapsed, duration);*/

    } while ( elapsed < duration );

  /*fprintf (stderr, "\n");*/
  
  if (p > 0)
    write (sound->file_descriptor, sound->buf, p * 2);
}

int main (int argc, char *argv[]) {

  SOUND_DEVICE sound;

  initialize_sound_device(&sound);

  /*play_sweep(&sound);*/

  while (1) {

    play_freq(&sound, 800.0, 0.02);

    play_freq(&sound, 1000.0, 0.02);

    play_freq(&sound, 1200.0, 0.02);

  }
  return 0;
}
