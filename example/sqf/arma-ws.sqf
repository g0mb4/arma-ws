private ["_command"];

while { true } do 
{
	_command = "arma-ws" callExtension ("?:");

	if( _command != "nothing" && _command != "" ) then
	{
		_commandStr = _command splitString ":";

		//diag_log format["[AWS] CMD: %1", _command];

		_id = _commandStr select 0;
		_code = _commandStr select 1;

		_result = [] call compile _code;

		//diag_log format["[AWS] RESULT: %1", _result];

		_type = typeName _result;

		//diag_log format["[AWS] TYPE: %1", _type];

		_response = ["!", _id, _type, _result ] joinString ":";

		//diag_log format["[AWS] RESPONSE: %1", _response];

		_command = "arma-ws" callExtension (_response);
	};

	sleep 0.1;
};