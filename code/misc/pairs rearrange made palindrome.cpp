// palindrome count
// odd <= 1
// so Xor = 0 means odd = 0
// Xor = 1 means odd = 1 for this
// i need to fllip exactly one bit of a-z
map<int, ll> freq;
int mask = 0;
// before processing any characters, we have one occurrence of mask 0
freq[mask] = 1;
ll result = 0;
// iterate over each character in the string
for (char c : s) {
    // Toggle the bit corresponding to the current character
    mask ^= (1 << (c - 'a'));
    // count substrings ending here that are already balanced (exact match)
    result += freq[mask];
    // count substrings ending here that differ by exactly one bit
    for (int b = 0; b < 26; ++b) {
        result += freq[mask ^ (1 << b)];
    }
    // update frequency for the current mask
    freq[mask]++;
}
cout << result << "\n";