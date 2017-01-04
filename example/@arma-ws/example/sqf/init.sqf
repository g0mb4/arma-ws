null = [] execVM "arma-ws.sqf";

while { true } do
{
	_pos = getPos player;

	_posX = _pos select 0;
	_posY = _pos select 1;
	_posZ = _pos select 2;

	hint format ["[%1, %2, %3]", _posX, _posY, _posZ];

	sleep 0.5;
}