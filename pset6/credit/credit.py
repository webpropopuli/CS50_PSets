from cs50 import get_int

def main():
	cardTypes = ["INVALID", "AMEX", "MASTERCARD", "VISA" ]

	digit = 0          # the current digit value
	cksumTotal = 0     # accumulator for checksum
	cardType = 0       # AMEX, MC, etc
	x = 0              # exponent for current power of 2
	n=0                # a counter
	first2 = 0         # first 2 digits of card
	testNum = 0		   # user input

	print("Number: ", end="")

	if testNum == 0:
		testNum = get_int()  # get user input unless I give test value

# Get first 2digits to determine card type
	cardType = int(str(testNum)[0:2])

# Find first power of 10 larger than number
	while testNum >= pow(10, x):
		x = x+1

# if not at least 13 digits then skip all checks
	if (x < 12):
		cksumTotal = 9999;  # force fail
		x=0

	##print("Before loop, x is=", x)
# From L->R, take each digit and either
#  add (evens) or
#  multiply (odd)
	while( x > 0 ):
		powX = pow(10, x-1)          # Largest power of 10 < num, e.g. 1,000,000

    # Get next digit
		digit = int(testNum / powX);     # get digit value
		##print("digit", digit)

    # add Checksums
    # follow Luhrs, adding evens and mult/flatten odds
		if((x % 2) != 0):
			cksumTotal += digit;
		else:
			#printf("oddnum: digit is %d - ck_in=%d...", digit, cksumTotal);
			n = digit * 2;
			if (n > 9):
				n = 1 + (n - 10 )

			cksumTotal = cksumTotal + n

		##print("chksum=", cksumTotal)

		testNum = testNum - (digit * powX)  # decrement num by subtracting digit * powX

		##print("testnum=", testNum)
		x-=1

	##print("c", cksumTotal, cardType);
#define CARD_INVALID 0
#define CARD_AMEX 1
#define CARD_MC 2
#define CARD_VISA 3

	if (cksumTotal % 10 != 0):
		##print("chksum failure")
		cardType = 0        # failed cksum
	else:
		if (cardType == 34 or cardType == 37):
			cardType = 1
		elif (cardType >=40 and cardType <=49):
			cardType = 3
		elif (cardType >=50 and cardType <=55):
			cardType = 2
		else:
			cardType = 0


	print(cardTypes[cardType])

if __name__ == "__main__":
    main()