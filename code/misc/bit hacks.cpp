#define ckbit(n, k) (((n)>>(k)) & 1)
#define toggle(n, k) ((n) ^= (1LL << (k)))
#define setbit(n, k) ((n) |= (1LL << (k)))
#define unsetbit(n, k) ((n) &= ~(1LL << (k)))
#define lowbit(n) ((n) & -(n))
#define highbit(n) (63 - __builtin_clzll(n)) // = floor log2(n)
// a|b = a^b + a&b
// a ^ (a&b) = b ^ (a|b)
// (a&b) ^ (a|b) = a^b

// a+b = a|b + a&b
// a+b = a^b + 2(a&b)

// a-b = (a^(a&b))-((a|b)^a)
// a-b = ((a|b)^b)-((a|b)^a)
// a-b = (a^(a&b))-(b^(a&b))
// a-b = ((a|b)^b)-(b^(a&b))
