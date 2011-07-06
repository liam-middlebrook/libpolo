/**
 * libpolosnd
 * Lightweight sound module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 */

/*
 * Polosnd is capable of playing sound files.
 *
 * Usage:
 * - Call openSound() to open the sound device.
 * - Call closeSound() to close the sound device.
 *
 * - Call playSound() to start playing back a sound file.
 * - Call stopSound() to stop playing.
 *
 * Supported file formats:
 * - WAV, AIFF, AU, OGG, FLAC
 */

#ifndef _LIBPOLOSND_H
#define _LIBPOLOSND_H

#ifdef __cplusplus
extern "C" {
#endif

/* Functions */
int openSound();
void closeSound();

int playSound(char *path);
void stopSound();

#ifdef __cplusplus
}
#endif

#endif

