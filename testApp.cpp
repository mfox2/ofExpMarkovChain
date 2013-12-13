#include "testApp.h"
#include "Poco/RegularExpression.h"
#include <iostream>
#include <regex>

//--------------------------------------------------------------
void testApp::setup(){
	
	numPrefixWords = 0;
	mapSize = 0;
	phraseSize = 0;

	while(numPrefixWords == 0)
	{
		cout << "How many words in prefix? Must be greater than zero" << endl;

		cin >> numPrefixWords;
	}

	while(phraseSize < numPrefixWords + 1)
	{
		cout << "How many words would you like in your phrase? Must be at least 1 greater than the number of words in your prefix" << endl;

		cin >> phraseSize;
	}
	
	std::string ugh = inputTextFile();

	cout << ugh << endl;

	breakItDown(ugh);

	generateNewText(markovTabl, phraseSize);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

string testApp::inputTextFile()
{
	std::string myCIN;
	cout << "Input name of file" << endl;

	cin >> myCIN;

	std::string strInput;

	ofBuffer inputTextBuffer = ofBufferFromFile(myCIN);

	strInput = inputTextBuffer.getText();

	return strInput;
}

void testApp::breakItDown(string bbbuffer)
{
	vector<string> breakItDownString;

	regex RegEx_getEveryWord("\\w+");

	smatch myMatches;

	for(sregex_iterator it(bbbuffer.begin(), bbbuffer.end(), RegEx_getEveryWord), it_end; it != it_end; ++it )
	{
        //cout << (*it)[0] << "\n";
		breakItDownString.push_back((*it)[0]);
	}

	//vector<string> breakItDownString = ofSplitString(bbbuffer," ");

	for(int i = 0; i < breakItDownString.size(); i++)
	{
		if(numPrefixWords > 1)
		{
			newBreakDown.push_back(breakItDownString[i]);
			
			int subtract = breakItDownString.size()-i;

			if(subtract < numPrefixWords) 
			{
				for(int j = 1; j < subtract; j++)
				{
					if(breakItDownString[i+j].size()>0)
					{
						newBreakDown[i] += " ";
						newBreakDown[i] += breakItDownString[i+j];
					}
				}
			}
			
			else
			{
				for(int j = 1; j < numPrefixWords; j++)
				{
					if(breakItDownString[i+j].size()>0)
					{
						newBreakDown[i] += " ";
						newBreakDown[i] += breakItDownString[i+j];
					}
				}
			}
		}
		
		else
		{
			newBreakDown.push_back(breakItDownString[i]);
		}

		newBreakDown[i] += " ";
	}

	cout << "Successful prefixation" << endl;

	for(int i = 0; i<newBreakDown.size(); i++)
	{
		int numDifferent = 0;

		for(std::map<string, vector<string>>::iterator it = markovTabl.begin(); it!=markovTabl.end(); it++)
		{
			if(newBreakDown[i]!=it->first)
			{
				numDifferent++;
			}
		}

		if(numDifferent >= markovTabl.size())
		{	
			insertIntoMarkovTabl(newBreakDown[i]);

			//I could go through the entire text here again and then make a vector that holds
			//all the suffixes to this prefix, then send that vector in the insert function
			//which would allow for me to set the prefix equal to the suffix immediately
		}
	}

	cout << markovTabl.size() << endl;

	for(std::map<string, vector<string>>::iterator it = markovTabl.begin(); it!=markovTabl.end(); it++)
	{
		textRegEx(it->first, bbbuffer);

		//we need to find the prefixes within the prefix vector which is newBreakDown
		//then we need to find the words that come immediately after those prefixes
		//so once we've found the index of the prefix how does the index of the suffix relate
		//to the index of the prefix? once i find this relation all i have to do is 
		//take the suffix and add it to the markovTabl vector<string> which i believe would be
		//it->second = suffix
		//...maybe
	}

	cout << "Successful suffixation" << endl;
}

void testApp::insertIntoMarkovTabl(string blah)
{
	vector<string> myVecStd;
	
	markovTabl[blah] = myVecStd;

	savedPrefixes.push_back(blah);
}

void testApp::textRegEx(string partyTime, string fullText)
{
	std::smatch m;

	string match;

	string partyTime2 = fullText;
	string partyTime3 = partyTime;

	string regEx = partyTime3 + "[a-zA-Z'-.&,]+";
	regex RegEx_stripLastWord2("[a-zA-Z'-.&,]+$");
	regex RegEx_prefixPlusOne2(regEx);

	Poco::RegularExpression RegEx_prefixPlusOne(regEx);
	Poco::RegularExpression RegEx_stripLastWord("[a-zA-Z'-.&,]+$");

	for(sregex_iterator it(fullText.begin(), fullText.end(), RegEx_prefixPlusOne2), it_end; it != it_end; ++it )
	{
		//cout << (*it)[0] << endl;
		
		string finalMatch;
		RegEx_stripLastWord.extract((*it)[0], finalMatch);

		markovTabl[partyTime].push_back(finalMatch);
	}

	
	for(int u = 0; u<markovTabl[partyTime].size(); u++)
	{
		//cout << "Prefix = " << partyTime << endl;
		//cout << "Suffix " << u << "= " << markovTabl[partyTime][u] << endl;
	}

}

void testApp::generateNewText(map<string, vector<string>>, int length)
{
	int phraseLength = 0;

	string generatedString;

	while(phraseLength < length)
	{
		int randomkey = int(ofRandom(0,markovTabl.size()-1));

		cout << "markovTabl.size = " << markovTabl.size() << endl;
		cout << "random key = " << randomkey << endl;

		string thekey = savedPrefixes[randomkey];

		cout << "THE KEY = " << thekey << endl;

		generatedString += thekey;

		phraseLength += numPrefixWords;

		int keySize = 0;

		map<string, vector<string>>::iterator it = markovTabl.find(thekey);

		keySize = it->second.size();
		keySize -= 1;
		
		if(keySize < 0)
		{
			//cout << "somehow theres no suffix to the prefix?" << endl;
			keySize = 0;
		}

		else
		{
			int randomvalue = int(ofRandom(0, keySize));

			cout << "markovTabl[thekey].size = " << keySize << endl;
			cout << "random key = " << randomvalue << endl;

			string thevalue = markovTabl[thekey][randomvalue];

			cout << "THE VALUE = " << thevalue << endl;

			generatedString += thevalue;
			generatedString += " ";

			phraseLength ++;
		}
	}

	cout << endl;
	cout << "GENERATED STRING:" << endl;
	cout << endl;
	cout << generatedString << endl;
}
