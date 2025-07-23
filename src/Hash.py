import os
import sys
import time

def main():
    pattern = sys.argv[1]
    mod = int( sys.argv[2] )
    
    file = open( "./assets/Bible.txt" , "r" );
    text = file.read()
    text = text.replace( "\n", '' ).replace("\t", '')
    file.close()
    
    start = time.perf_counter()
    positions , colissions = Hash( text , pattern , mod )
    end = time.perf_counter()
    
    print( "Hash" + "," + pattern + "," + str( len(positions) ) + "," + str( end - start ) + "," + str( colissions) + "," + str( mod ) )

    
def Hash( text , pattern , mod ):
    textSize = len( text );
    patternSize = len( pattern );
    
    positions = []
    colissions = 0
    
    patternHash = 0
    textHash = 0
    
    for index in range( 0 , patternSize ):
        patternHash = patternHash + ord( pattern[index] ) * pow( 10 , patternSize - 1 - index )
        textHash = textHash + ord( text[index] ) * pow( 10 , patternSize - 1 - index )
    
    for index in range( 0 , textSize - patternSize ):
        if textHash % mod == patternHash % mod :
            if( text[index : index+ patternSize] == pattern ):
                positions.append( index )
            else:
                colissions += 1
                
        if index < textSize - patternSize :
            textHash = ( textHash - ord( text[index] ) * pow( 10 , patternSize - 1 ) ) * 10 + ord( text[index + patternSize] )
    return positions , colissions

main()