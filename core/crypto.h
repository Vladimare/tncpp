#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#define BLOCKSIZE     16      //размер блока для алгоритма кодирования. Зависит от алгоритма кодирования. Не изменять
#define ROUNDS        10      //количество повторений кодирования блока. Зависит от алгоритма кодирования. Не изменять
#define BPOLY         0x1B    //полином для алгоритма кодирования, младшая восьмибитовая часть от
                              //   (x ^ 8 + x ^ 4 + x ^ 3 + x + 1), т.е. (x ^ 4 + x ^ 3 + x + 1)
#define EXPANDED_KEY_SIZE (BLOCKSIZE * (ROUNDS + 1))//длина расширенного ключа

extern unsigned long cr_decrypt_sector(unsigned char* blk);

#endif /*_CRYPTO_H_*/
