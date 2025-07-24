import sys
import time
from typing import *

def timeit(wrapper: Any):
    def func(text: str, pattern: str):
        start = time.perf_counter()
        positions = wrapper(text, pattern)
        end = time.perf_counter()
        print( "KMT" + "," + f'"{pattern}"' + "," + str( len(positions) ) + "," + str( end - start ) + ",,," )

    return func


def prefixArray(pattern: str):
    l = [0] * len(pattern)
    k = 0
    for q in range(1, len(pattern)):
        while k > 0 and pattern[k] != pattern[q]:
            k = l[k - 1]
        if pattern[k] == pattern[q]:
            k += 1
        l[q] = k

    return l

@timeit
def KMPMatcher(T: str, P: str):
    pos: List[int] = []
    l = prefixArray(P)
    q = 0

    for i in range(len(T)):
        while q > 0 and P[q] != T[i]:
            q = l[q - 1]
        if P[q] == T[i]:
            q = q + 1
        if q == len(P):
            idx = i -len(P) + 1
            pos.append(idx)
            # padding = 10
            # print(f"{idx}: \"{T[idx - padding:idx+len(P)+padding]}\"") 
            q = l[q - 1]
    return pos




def main() -> None:
    with open( "./assets/Bible.txt" ) as file:
        text = file.read()
        text = text.replace( "\n", ' ' ).replace("\t", ' ')

    KMPMatcher(text, sys.argv[1])
	
if __name__ == '__main__':
	main()
