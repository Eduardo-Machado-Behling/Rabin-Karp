import os
import sys
import time

def main():
    file = open( "./assets/Bible.txt" , "r" );
    pattern = sys.argv[1]
    text = file.read()
    file.close()
    text = text.replace( "\n", '' ).replace("\t", '')
    
    start = time.perf_counter()
    index = CharbyChar( text , pattern )
    end = time.perf_counter()
    # print('', *index, sep='\t\n')
    print( pattern , len(index) , end - start )
    
    start = time.perf_counter()
    index, cols = HashCharbyChar( text , pattern )
    end = time.perf_counter()
    # print('', *index, sep='\t\n')
    print( pattern , len(index) , end - start )

    
def CharbyChar( text , pattern ):
    textSize = len( text );
    patternSize = len( pattern );
    size = textSize - patternSize;
    index = []
    for i in range( 0 , size ):
       
        cur = text[ i : i + patternSize ]
        equal = True
        for cc, cp in zip(cur, pattern):
            if cc != cp:
                equal = False
                break
        
        if(equal):
            index.append( i )
    return index

def HashCharbyChar( text , pattern ):
    collisions = 0
    textSize = len( text );
    patternSize = len( pattern );
    size = textSize - patternSize;
    hs = hash(pattern)
    index = []
    for i in range( 0 , size ):
       
        cur = text[ i : i + patternSize ]
        if(hash(cur) != hs):
            continue
        
        equal = True
        for cc, cp in zip(cur, pattern):
            if cc != cp:
                equal = False
                collisions += 1
                break
        
        if(equal):
            index.append( i )
    return index, collisions




    
main()