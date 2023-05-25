/**
 * This is your SKINNY-tk3-Hash instance context, i.e., hash state.
 * You could put the following stuff inside here:
 * - chaining value
 * - chaining value offsets
 * - buffer offsets
 * - leprechaun gold
 * - ...
 */
typedef struct {
    unsigned char status[48];
    unsigned char* message;
    unsigned int messageBufferSizeLimit;
    unsigned int messageBufferSize;
    unsigned int paddedMessageSize;
} context;

/**
 * Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */
void computeFFunction(const unsigned char *tk, unsigned char *output);


/* Initialize fresh context ctx. */
void init(context *ctx);

/* Under context ctx, incrementally process len bytes at a. */
void update(const unsigned char *a, int len, context *ctx);

/* Under context ctx, finalize the hash and place the digest at a. */
void finalize(unsigned char *a, context *ctx);

