/*
* jsoncpp   http://jsoncpp.sourceforge.net/
*
* Makefile
*   compile
*       -I/path/to/json/include/
*   link
*       -L/path/to/json/lib/ -ljson
*/

#include <iostream>
#include <cstdio>
#include <json/json.h>

using namespace std;

int main()
{
    Json::Reader reader;
    Json::Value value;
    string input;
    value.clear();
    if(reader.parse(input,value)){
        string col1 = value["col1"].asString();
        int col2 = default_col2;
        try {
            col2 = value["col2"].asInt();
        }catch(std::exception &e){
            catch_throw(e);
        }

        for(int i=0; i<value["outer"].size(); i++){
            string col1 = value["outer"][i]["col1"].asString();
            Value inner = value["outer"][i]["inner"];
            for(int j=0; j<inner.size(); j++){
                string col2 = inner[j]["col2"].asString();
            }
        }
    } else
        cout<<reader.getFormattedErrorMessages()<<endl;

}
