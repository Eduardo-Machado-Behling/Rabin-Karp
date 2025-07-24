import os
import sys
import time

def main():
    pattern = sys.argv[1]
    mod = int( sys.argv[2] )
    radix = int( sys.argv[3] )
    
    file = open( "./assets/Bible.txt" , "r" );
    text = file.read()
    text = text.replace( "\n", ' ' ).replace("\t", ' ')
    file.close()
    
    start = time.perf_counter()
    positions , colissions = Rabinkarp( text , pattern , radix , mod )
    end = time.perf_counter()
    
    print( "RabinKarp" + "," + f'"{pattern}"' + "," + str( len(positions) ) + "," + str( end - start ) + "," + str( colissions ) + "," + str( mod ) + "," + str( radix ) )

    
def Rabinkarp( text , pattern , radix , mod ):
    patternSize = len( pattern );
    textSize = len( text );
    lowOrderProportion = pow( radix , patternSize - 1 ) % mod
    
    positions = []
    colissions = 0
    
    patternHash = 0
    textHash = 0
    
    for index in range( 0 , patternSize ): # preprocessing
        patternHash = ( radix * patternHash + ord( pattern[index] ) ) % mod
        textHash = ( radix * textHash + ord( text[index] ) ) % mod
        
    for index in range( 0 , textSize - patternSize ):
        if patternHash == textHash:
            if pattern == text[index: index + patternSize]:   
                positions.append( index )
            else:
                colissions += 1
        if index < textSize - patternSize:
            textHash = ( radix * ( textHash - ord( text[index] ) * lowOrderProportion ) + ord( text[index + patternSize] ) ) % mod
            
    return positions , colissions

main()
