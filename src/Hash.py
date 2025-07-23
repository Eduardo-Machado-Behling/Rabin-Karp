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
    positions , colissions = Hash( text , pattern )
    end = time.perf_counter()
    
    print( "Hash" + "," + pattern + "," + str( len(positions) ) + "," + str( end - start ) + "," + str( colissions) )

    
def Hash( text , pattern ):
    textSize = len( text );
    patternSize = len( pattern );
    
    positions = []
    colissions = 0
    
    patternHash = hash( pattern )
    
    for index in range( 0 , textSize - patternSize ):
        window = text[ index : index + patternSize ]
        
        if hash( window ) == patternHash:
            if( window == pattern ):
                positions.append( index )
            else:
                colissions += 1   
        
    return positions , colissions

main()