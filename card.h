#ifndef CARD_H
#define CARD_H

void card_init(void);
void card_write(unsigned int, unsigned char);
void card_read(unsigned int);
void card_cid(void);
void card_csd(void);

#endif
