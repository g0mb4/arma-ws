private ["_code"];

// get code for the first time
_code = "arma-ws" callExtension ("?:");

// chack if code is valid
if( _code == "nothing" || _code == "" ) exitWith {};

while { true } do 
{
	// run code
	_result = [] call compile _code;

	// get thy datatype of the result
	_type = typeName _result;

	// create response (compatible with COMMAND mode)
	_response = ["!", "0", _type, _result ] joinString ":";

	// send back result
	null = "arma-ws" callExtension (_response);

	sleep 0.1;
};