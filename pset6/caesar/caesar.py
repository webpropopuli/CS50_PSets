from cs50 import get_string
import sys
from inspect import signature


	# Only 1 arg allowed
if (len(sys.argv) != 2):
    print("Wrong number of arguments. Exiting...");
    sys.exit(1)

iKeyval = int(sys.argv[1]) % 26;
#print("keyval % 26=", iKeyval)

 # Get plaintext from user
print("Plaintext: ", end="");
strPlain = list(get_string())
#print(strPlain)


#ch  #current char in plaintest
#A2aDiff  # how far to shift from 'A' to 'a'
strCipher = ""

#c=(pi+k)mod26

# Shift each letter to cipher array
#len = len(strPlain)
for x in range(0, len(strPlain)):
    ch = strPlain[x];
    #print("x={}->{}".format(x, ch))


    if(not ch.isalpha()):
        strCipher += ch;
    else:
        if (ch.islower()):
            A2aDiff = ord('a')
        else:
            A2aDiff = ord('A')

        ec = chr(((ord(ch) + iKeyval - A2aDiff) %26) + A2aDiff);
        strCipher += ec;

print("ciphertext: ",str(strCipher));