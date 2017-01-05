private ["_command"];

while { true } do 
{
	// ping the server if there is any command in the queue
	_command = "arma-ws" callExtension ("?:");

	// if there is any command to execute
	if( _command != "nothing" && _command != "" ) then
	{
		// disassemble the message
		_commandStr = _command splitString ":";

		// get the ID of te message, need for the server
		_id = _commandStr select 0;
		// get the command
		_code = _commandStr select 1;

		// execute the command
		_result = [] call compile _code;

		// get teh type of the result
		_type = typeName _result;

		// assemble the message
		_response = ["!", _id, _type, _result ] joinString ":";

		// send back the result
		_command = "arma-ws" callExtension (_response);
	};

	sleep 0.1;
};