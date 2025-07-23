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
    positions , colissions = Naive( text , pattern )
    end = time.perf_counter()
    
    print( "Naive" + "," + pattern + "," + str( len(positions) ) + "," + str( end - start ) )

    
def Naive( text , pattern ):
    textSize = len( text );
    patternSize = len( pattern );
    
    positions = []
    colissions = 0
    
    for index in range( 0 , textSize - patternSize ):
        window = text[ index : index + patternSize ]
        
        if( window == pattern ):
            positions.append( index )
            
    return positions , colissions

main()