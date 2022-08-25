/* c3
 * Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>
 */
#ifndef UCD_H
#define UCD_H

#define UCD_MAX 0x11000

#define UCD_FLAG_LOWERCASE 0x01
#define UCD_FLAG_UPPERCASE 0x02
/* unknown flags */
#define UCD_FLAG_C         0x0
#define UCD_FLAG_P         0x0
#define UCD_FLAG_S         0x0
#define UCD_FLAG_Z         0x0
#define UCD_FLAG_c         0x0
#define UCD_FLAG_e         0x0
#define UCD_FLAG_l         0x0
#define UCD_FLAG_o         0x0
#define UCD_FLAG_p         0x0
#define UCD_FLAG_s         0x0

typedef unsigned long ucd_flags;

typedef struct ucd {
  ucd_flags flags;
  char *name;
} s_ucd;

extern const s_ucd g_ucd[UCD_MAX];

#endif /* UCD_H */
