function interpretMessage(msg)
{
	var arr = msg.split(":");

	var type = arr[0];
	var result = arr[1];

	switch(type){
		case "SCALAR" :
			return Number(result);

		case "STRING" :
			return result;

		case "ARRAY" :
			var result = result.slice(1, -1);
			var foo = result.split(",");
			var array = []

			if(isNaN(foo[0])){
				for(var i = 0; i < foo.length; i++){
					array.push(foo[i]);
				} 
			} else {
				for(var i = 0; i < foo.length; i++){
					array.push(Number(foo[i]));
				} 
			}

			return array;
			break;
	}
}
