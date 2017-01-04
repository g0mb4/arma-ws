
var wsUri = "ws://127.0.0.1:9002";

function startWebSocketClient()
{
	try {
		websocket = new WebSocket(wsUri);
		websocket.onopen = function(evt) { onOpen(evt) };
		websocket.onclose = function(evt) { onClose(evt) };
		websocket.onmessage = function(evt) { onMessage(evt) };
		websocket.onerror = function(evt) { onError(evt) };
	} catch (err){
		setTimeout(startWebSocketClient, 1000);
	}
}

function wrtieToScreen(msg)
{
	fill(51);
	rect(0, height - 20, width, 20);
	fill(255);
	text(msg, 5, height - 5);
}

function onOpen(evt)
{
	websocket.send("_p = getPos player; _p");
}

function onClose(evt)
{
	setTimeout(startWebSocketClient, 1000);
}

function onMessage(evt)
{
	var result = interpretMessage(evt.data);
	websocket.close();

	background(51);

	var posX = map(result[0], 0, 4000, 0, width);
	var posY = map(result[1], 0, 4000, 0, height);

	fill(0, 255, 0);
	ellipse(posX, posY, 10, 10);

	var str = "[" + result[0] + ", " + result[1] + ", " + result[2] + "]";

	wrtieToScreen(str);
}

function onError(evt)
{
	console.log("Error");
}
