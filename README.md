# Vigenere-Cipher-Decrypter
A decrypter for a file encrypted with the Vigenere cipher.

## Methodology
To break the encryption provided in the text, we use Kasiski's method, which relies on analysing repeated substrings in the given text and measure the distances between these occurrences. The key length is most likely a common factor of these distances, which helps us determine the key length in order to aid the decryption process. We then use the index of coincidence, which compares the frequency of occurrence of characters in the text to a table with the expected frequencies of characters in the English alphabet. Through this method, we can estimate the likelihood of a text being encrypted with a word of a specific key length in order to determine the keyword by mapping these character frequencies.

## Steps to use the Vigenere decrypter
To use the decrypter, upload the text to be decrypted as a "ciphertext.txt" file in the same directory as the vigenere-decrypter.c file.

Run the following code block in the terminal:
````
gcc vigenere-decrypter.c
./a.out
````

## Sources of reference
- [Kasiski's Method (Dr. Ching-Kuang Shene, Michigan Technological University)](https://pages.mtu.edu/~shene/NSF-4/Tutorial/VIG/Vig-Kasiski.html)
- [Index of Coincidence (Dr. Ching-Kuang Shene, Michigan Technological University)](https://pages.mtu.edu/~shene/NSF-4/Tutorial/VIG/Vig-IOC.html)
