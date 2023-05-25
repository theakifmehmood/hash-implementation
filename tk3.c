#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tk3.h"
#include "skinny.h"

void init(context *ctx)
{
    ctx->messageBufferSize = 0;
    ctx->messageBufferSizeLimit = 256;
    ctx->paddedMessageSize = 0;
    ctx->message = (unsigned char *)calloc(ctx->messageBufferSizeLimit, sizeof(unsigned char));
    memset(ctx->status, 0x00, 48);
    ctx->status[16] = 0x80;
}

void update(const unsigned char *a, int len, context *ctx)
{
    memcpy(ctx->message + ctx->messageBufferSize, a, len);
    ctx->messageBufferSize += len;
}

void finalize(unsigned char *a, context *ctx)
{

    int messageBufferSizeInBites = ctx->messageBufferSize * 8;
    int messageInBitesReduceOne = -1 - messageBufferSizeInBites;
    int blockSize = 128;
    int extendedSize = (messageInBitesReduceOne % blockSize + blockSize) % blockSize;

    ctx->paddedMessageSize = ctx->messageBufferSize + (extendedSize + 1) / 8;

    unsigned char hashPadded[16], hashFinal[16];
    unsigned char paddedMessage[ctx->paddedMessageSize];
    
    for (int i = 0; i < ctx->paddedMessageSize; i++)
    {
        if (i > ctx->messageBufferSize)
            paddedMessage[i] = 0x00;
        else if (i < ctx->messageBufferSize)
            paddedMessage[i] = ctx->message[i];
        else if (i == ctx->messageBufferSize)
            paddedMessage[i] = 0x80;
    }

    for (int blockStart = 0; blockStart <= ctx->paddedMessageSize - 16; blockStart += 16)
    {
        unsigned char block[48] = {0x00};

        for (int i = 0; i < 16; i++)
            block[i] = paddedMessage[blockStart + i];

        for (int i = 0; i < 48; i++)
            block[i] ^= ctx->status[i];

        computeFFunction(block, ctx->status);
    }

    memcpy(hashPadded, ctx->status, 16);
    computeFFunction(ctx->status, ctx->status);
    memcpy(hashFinal, ctx->status, 16);
    memcpy(a, hashPadded, 16);
    memcpy(a + 16, hashFinal, 16);
}

void computeFFunction(const unsigned char *tk, unsigned char *output)
{
    const unsigned char msg1[16] = {0x00};
    const unsigned char msg2[16] = {0x01};
    const unsigned char msg3[16] = {0x02};

    unsigned char output1[16], output2[16], output3[16];

    skinny(output1, msg1, tk);
    skinny(output2, msg2, tk);
    skinny(output3, msg3, tk);

    memcpy(output, output1, 16);
    memcpy(output + 16, output2, 16);
    memcpy(output + 32, output3, 16);
}
