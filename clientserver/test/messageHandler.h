#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;

void printError(const Connection& conn){
	cout<<"ERROR: ";
        unsigned char err = conn.read();
	switch(err){
		case static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS):
			cout<<"News group already exists"<<endl;
		break;
		case static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST):
			cout<<"News group does not exist"<<endl;
		break;
		case static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST):
			cout<<"Article does not exist"<<endl;
	}
}

//read num_p
int readNumber(const Connection& conn){
	//remove PAR_NUM
	conn.read();

	//read number
        unsigned char byte1 = conn.read();
        unsigned char byte2 = conn.read();
        unsigned char byte3 = conn.read();
        unsigned char byte4 = conn.read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

//read string_p
string readString(const Connection& conn){
	//remove PAR_STRING
	cout<<conn.read()<<endl;
	
        unsigned char byte1 = conn.read();
        unsigned char byte2 = conn.read();
        unsigned char byte3 = conn.read();
        unsigned char byte4 = conn.read();
        int len = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
	cout<<"TRTTT"<<len<<endl;
        string s;

        char c;
	for(int i = 0; i != len; i++){
		c = conn.read();
		s+=c;
		cout<<i<<endl;
	}	
        return s;
}

// write string_p
void writeString(const Connection& conn, string s){
	    conn.write(static_cast<char>(Protocol::PAR_STRING));
	    unsigned int len = s.size();
	    cout<<"CCCCCCC"<<len<<endl;
            conn.write((len >> 24) & 0xFF);
            conn.write((len >> 16) & 0xFF);
	    conn.write((len >> 8) & 0xFF);
	    conn.write(len & 0xFF);
	    for(char c : s)conn.write(c);
}
// write string_p
void writeString(const shared_ptr<Connection>& conn, string s){
	    writeString(*conn, s);
}

// write num_p
void writeNumber(const Connection& conn, int value){
	cout<<"AAAAAAAA"<<value<<endl;
	conn.write(static_cast<char>(Protocol::PAR_NUM));
        conn.write((value >> 24) & 0xFF);
        conn.write((value >> 16) & 0xFF);
        conn.write((value >> 8) & 0xFF);
        conn.write(value & 0xFF);
}


void sendByte(const Connection& conn, unsigned char byte) {
    conn.write(byte);
}


void sendCode(const Connection& conn, Protocol code) {
    sendByte(conn, static_cast<unsigned char>(code));
}




#endif
