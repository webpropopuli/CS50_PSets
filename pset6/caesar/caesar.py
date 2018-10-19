from cs50 import get_string
import sys
from inspect import signature


	# Only 1 arg allowed
if (len(sys.argv) != 2):
    print("Wrong number of arguments. Exiting...");
    sys.exit(0)

keyval = int(sys.argv[1]) % 26;
print("keyval % 26=", keyval)

 # Get plaintext from user
print("Plaintext: ", end="");
strPlain = list(get_string())


#ch  #current char in plaintest
#A2aDiff  # how far to shift from 'A' to 'a'
strCipher = []

# Shift each letter to cipher array
#len = len(strPlain)
for x in range(0, len(strPlain)):
    print('x=', x)
    ch = strPlain[x];

    if(not ch.isalpha()):
        strCipher[x] = ch;
    else:
        if (ch.islower()):
            A2aDiff = 'a'
        else:
            A2aDiff = 'A'

        strCipher[x] = chr( ((((ch - A2aDiff) + keyval) % 26)) + A2aDiff);

print("ciphertext: ", strCipher);