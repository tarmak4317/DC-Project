#include <REG51.H>

void DMC(void)
{
    unsigned char i;
    unsigned char sum_tx = 0, parity_tx = 0;
    unsigned char sum_rx = 0, parity_rx = 0;
    unsigned char deltaH, mask, temp1, temp2, diff;
    unsigned char *ptr_tx, *ptr_rx, *ptr_out;

    ptr_tx = (unsigned char *)0x30;
    ptr_rx = (unsigned char *)0x40;
    ptr_out = (unsigned char *)0x50;

    sum_tx = 0;
    for(i = 0; i < 8; i++) {
        sum_tx += ptr_tx[i];
    }

    parity_tx = 0;
    for(i = 0; i < 8; i++) {
        parity_tx ^= ptr_tx[i];
    }

    sum_rx = 0;
    for(i = 0; i < 8; i++) {
        sum_rx += ptr_rx[i];
    }

    parity_rx = 0;
    for(i = 0; i < 8; i++) {
        parity_rx ^= ptr_rx[i];
    }

    if(sum_tx >= sum_rx)
        deltaH = sum_tx - sum_rx;
    else
        deltaH = sum_rx - sum_tx;

    mask = parity_tx ^ parity_rx;

    for(i = 0; i < 8; i++) {
        temp1 = ~ptr_rx[i];
        temp1 &= mask;
        temp2 = ptr_rx[i] & mask;
        diff  = temp2 - temp1;
        if(diff & 0x80) {
            diff = ~diff + 1;
        }
        if(diff == deltaH) {
            *ptr_out++ = ptr_rx[i];
            *ptr_out++ = ptr_rx[i] ^ mask;
        }
    }
}
