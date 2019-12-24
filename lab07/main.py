from kmp import *
from bmh import *

if __name__ == '__main__':
    # text = "abababcdabababa"
    # pattern = "ababa"
    text = "ababab"
    pattern = "acd"
    KMPsearch(text, pattern)

    BMHsearch(text, pattern)