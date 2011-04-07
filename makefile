all: examples
	g++ main.cpp ./OptFrame/Scanner++/Scanner.cpp -lpthread -o mainOptFrame

examples:
	g++ ./OptFrame/Examples/mainTSP.cpp ./OptFrame/Scanner++/Scanner.cpp -lpthread -o mainOptFrameTSP
	g++ ./OptFrame/Examples/mainEternityII.cpp ./OptFrame/Scanner++/Scanner.cpp -lpthread -o mainOptFrameEternityII
	
