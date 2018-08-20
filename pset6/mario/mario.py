#------------------------
# CS50, pset6, mario/less

from cs50 import get_int

iHeight = 5  # Force true first pass

while True:
    # prompt for input
    print("Height: ", end="")
    iHeight = get_int()
    if iHeight >= 0 and iHeight < 24:
      break

for cntBlocks in range(1, iHeight+1):
    cntSpaces = iHeight - cntBlocks
    print(" " * cntSpaces, end="")
    print("#" * cntBlocks, end="")
    print("  ", end="")			# gap
    print("#" * cntBlocks)
# end