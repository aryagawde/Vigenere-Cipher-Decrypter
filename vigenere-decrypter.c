// CS425 Assignment: Vigenere Cipher Decryption
// Ananya Alekar (2103108, CSE), Arya Gawde (2103115, CSE)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Helper function to remove spaces and special characters in the given cipher text for Kasiski analysis
char* removeSpecialChars(char* string) {
    int position = 0;
    for (int index = 0; string[index] != '\0'; index++) {
        if (isalpha(string[index])) {
            if (string[index] >= 'a' && string[index] <= 'z') {
                string[position] = string[index] - 32;
            } else {
                string[position] = string[index];
            }
            position++;
        }
    }
    string[position] = '\0';
    return string;
}

// Helper function to get the length of the string
int length(char* string) {
    int index = 0;
    while (string[index] != '\0') {
        index++;
    }
    return index;
}

// Helper function to return the substring of a specified length at a given start position of the given text
void substringFn(char* string, int position, int length, char* substring) {
    int index = 0;
    while (index < length) {
        substring[index] = string[position + index];
        index++;
    }
    substring[length] = '\0';
}

// Helper function to compute distance lengths between consequent occurrences of a substring and store it in 
// the distancesArray and returns the length of the array
int computeSubsequentDistances(char* string, int* distancesArray) {
    int distanceIndex = 0;
    // Since the Kasiski methods works optimally for key distances >= 3
    for (int keyLength = 3; keyLength < 50; keyLength++) {
        for (int position = 0; position < length(string) - keyLength; position++) {
            int frequency = 1;
            int positionIndex[100];
            positionIndex[0] = position;
            int wordPos = 1;

            char substring[keyLength + 1];                          
            substringFn(string, position, keyLength, substring);  // substring starting at position and has length = keyLength
            int stringLength = length(string);

            // Checking for frequency of the given substring in the text
            for (int index = position + 1; index < stringLength - keyLength; index++) {
                char subword[keyLength + 1];
                substringFn(string, index, keyLength, subword);

                if (strcmp(substring, subword) == 0) {
                    frequency++;
                    positionIndex[wordPos] = index;
                    wordPos++;
                }
            }

            // We analyze the distances between substrings of a substring with a high frequency of occurrence for
            // optimization i.e frequency > 10
            if (frequency > 10) {
                int gcdFactors = positionIndex[0];
                for (int j = 0; j < frequency - 1; j++) {
                    distancesArray[distanceIndex] = positionIndex[j + 1] - positionIndex[j];
                    distanceIndex++;
                }
            }
        }
    }
    return distanceIndex;
}

// Helper function to find the element in an array with the highest frequency
int maxFrequencyElement(int* numArray, int length) {
    int maxCount = 0;
    int elementMaxFreq;

    for (int index = 0; index < length; index++) {
        int freqCount = 0;
        for (int j = 0; j < length; j++) {
            if (numArray[index] == numArray[j]) {
                freqCount++;
            }
        }

        if (freqCount >= maxCount && numArray[index] > elementMaxFreq) {
            maxCount = freqCount;
            elementMaxFreq = numArray[index];
        }
    }
    return elementMaxFreq;    
}

// Helper function to obtain the optimal key length to decode the cipher text
int keyLengthApproximation(int* numbers, int length) {
    int index2 = 0;
    int factorsArray[10000];
    for (int index = 0; index < length; index++) {
        for (int factor = 3; factor <= numbers[index]; factor++) {
            if (numbers[index] % factor == 0) {
                factorsArray[index2] = factor;
                index2++;
            }
        }
    }
    return maxFrequencyElement(factorsArray, index2);
}

// Helper function to obtain the probabilistic keyword of the given keyword length using chi-squared method
char statisticalAnalysis(char* charArray, int n) {
    // frequencies of all letters in the English alphabet based on a large sample of words: each index represents the frequency for a letter 'A' + index
    float freqArray[26] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

    // Frequency of words at a given index based on current sample in fArray
    int charFreq[26] = {0};

    for (int index = 0; index < n; index++) {
        int letter = charArray[index] - 'A';
        charFreq[letter] = charFreq[letter] + 1.0; 
    }

    float fArray[26];
    for (int index = 0; index < 26; index++) {
        fArray[index] = (float)charFreq[index] / (float)n;
    }

    // The shift with the least gamma-squared value is the shift position
    int shiftPosition = 0;
    float minVariance = 1000000.00;
    for (int shift = 0; shift < 26; shift++) {
        float variance = 0.0;
        for (int index = 0; index < 26; index++) {
            variance += ((fArray[(index + shift) % 26] - freqArray[index]) * (fArray[(index + shift) % 26] - freqArray[index])) / freqArray[index]; 
        }

        if (variance < minVariance) {
            minVariance = variance;
            shiftPosition = shift;
        }
    }
    return 'A' + shiftPosition;
}

// Helper function to decrypt text using the keyword
void decryptCipherText(char* fileText, char* keyword) {
    int len = length(fileText);
    int keyLength = length(keyword);
    int keyPosition = 0;

    for (int index = 0; index < len; index++) {
        if (isalpha(fileText[index])) {
            int letter;
            if (fileText[index] >= 'a' && fileText[index] <= 'z') {
                letter = fileText[index] - 'a';
            } else if (fileText[index] >= 'A' && fileText[index] <= 'Z') {
                letter = fileText[index] - 'A';
            }
            fileText[index] = 'A' + ((letter - (keyword[keyPosition] - 'A') + 26) % 26);
            keyPosition = (keyPosition + 1) % keyLength;
        }
    }
}

int main() {
    // Open the ciphertext in read mode
    FILE *fptr;
    
    fptr = fopen("ciphertext.txt", "r");

    if (fptr == NULL) {
        printf("No such file exists.\n");
        return 1;
    }

    // Maximum possible string length
    int stringLength = 10000;                                   
    char fileText[stringLength];
    char lineText[stringLength];

    fgets(fileText, stringLength, fptr);
    while (fgets(lineText, sizeof(lineText), fptr) != NULL) {
        strcat(fileText, lineText);
    }

    printf(" =================== Running Vigenere Cipher: =============================\n");
    
    char unencryptedText[stringLength];
    strcpy(unencryptedText, fileText);

    char nonSpaceFileText[stringLength];                        
    strcpy(nonSpaceFileText, removeSpecialChars(fileText));

    // Determining the key length in order to break the cipher
    // This array maintains distances between two consecutive repeating substrings
    int distances[stringLength];                                

    // KEY LENGTH ANALYSIS USING KASISKI METHOD: Checking for each substring of given key length
    int distanceLength = computeSubsequentDistances(nonSpaceFileText, distances);
    int keyLength = keyLengthApproximation(distances, distanceLength);
    printf("Key Length: %d \n", keyLength);

    char keyword[keyLength + 1];

    // KEY ANALYSIS
    // Frequency analysis and decryption of keyword using the chi-squared method
    // charArray used to store all letters at the index, index + keyLength, index + 2 * keyLength, .... position
    char charArray[stringLength];                      
    for (int index = 0; index < keyLength; index++) {
        int n = 0;
        for (int position = index; nonSpaceFileText[position] != '\0'; position = position + keyLength) {
            charArray[n] = nonSpaceFileText[position];
            n++;
        }
        // Returns the key letter at that index
        keyword[index] = statisticalAnalysis(charArray, n);     
    }
    keyword[keyLength] = '\0';

    printf("The given codeword is: %s", keyword);

    // Decryption of cipher using keyword
    decryptCipherText(unencryptedText, keyword);
    printf("\n\nThe decrypted message is: \n%s", unencryptedText);
    
    return 0;
}