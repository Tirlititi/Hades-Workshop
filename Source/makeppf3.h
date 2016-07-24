/*
 *     MakePPF3.h
 *     written by Icarus/Paradox
 *     some compatibility changes by Tirlititi
 *
 *     Big Endian support by Hu Kares.
 *
 *     Creates PPF3.0 Patches.
 *     Feel free to use this source in and for your own
 *     programms.
 *
 */

#ifndef _MAKEPPF3_H
#define _MAKEPPF3_H

#include <inttypes.h>

int		PPFOpenFilesForCreate(char* ppfpath, bool undo, bool blockcheck, char* description);
int		PPFCreateHeader(unsigned char binblock[1024]);
int		PPFWriteChanges(unsigned char *x, unsigned char *y, uint32_t amount, uint64_t chunk);
void	PPFCloseAllFiles(void);

#endif
