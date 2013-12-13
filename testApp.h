#pragma once

#include "ofMain.h"
#include <map>
#include <string>

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		string inputTextFile();

		void breakItDown(string bbbuffer);
		void insertIntoMarkovTabl(string blah);
		void textRegEx(string partyTime, string fulltext);

		void generateNewText(map<string, vector<string>>, int numWords);

		void keyPressed(int key);

		string prefix;

		map<string, vector<string>> markovTabl;

		int numPrefixWords;	
		int phraseSize;

		vector<string> newBreakDown;

		int mapSize;
};
