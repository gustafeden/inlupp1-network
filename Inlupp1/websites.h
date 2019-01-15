#pragma 
//String selectionHtml = "<!DOCTYPE html><html> <head> <title>Overview - WebServer</title> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'> <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css'> </head> <body> <div class='jumbotron text-center'> <h1>Please select one</h1> </div><div class='d-flex justify-content-center'> <div class='btn-group '> <form action='/selection/action' method='post'> <button class='btn btn-lg btn-primary' name='btn' value='temp'>Temperature</button> <button class='btn btn-lg btn-primary' name='btn' value='msgbtn'>Message</button> <button class='btn btn-lg btn-primary' name='btn' value='out'>Sign out</button> </form> </div></div></body></html>";
String TempHtml = "<div class='d-flex justify-content-center'><div class='p-2'>Temp recieved from other device :</div><div class='p-2'>change here</div></div>";
String MessageHtml = "<div class='d-flex justify-content-center'> <form action='/selection/action' method='post' class='form-inline'> <label for='msg' class='mr-sm-2'>Enter Message to send:</label> <input type='text' name='msgin' class='form-control mb-2 mr-sm-2' id='msg'> <button type='submit' class='btn btn-primary mb-2'>Submit</button> </form></div>";

String selectionHtml = "<!DOCTYPE html><html> <script src='https://code.jquery.com/jquery-3.3.1.slim.min.js'></script> <head> <title>Overview - WebServer</title> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'> <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css'> </head> <body > <div class='jumbotron text-center'> <h1>Please select one</h1> </div><div class='d-flex justify-content-center'> <div class='btn-group '> <form action='/selection/action' method='post'> <button class='btn btn-lg btn-primary' name='btn' value='temp' id='temp'>Temperature</button> <button class='btn btn-lg btn-primary' name='btn' value='msgbtn' id='msgbtn' onclick='clearTimeout(myVar)'>Message</button> <button class='btn btn-lg btn-primary' name='btn' value='out' id='out' onclick='clearTimeout(myVar)'>Sign out</button> </form></div></div><script>function getTemperature(){var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function(){if (this.readyState==4 && this.status==200){var stateValue=this.responseText; document.getElementById('temptext').innerHTML=stateValue;}}; xhttp.open('GET', 'update', true); xhttp.send();}</script> </body></html>";
//"<!DOCTYPE html><html> <head> <title>Overview - WebServer</title> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'> <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css'> </head>  <script src='https://code.jquery.com/jquery-3.3.1.slim.min.js'> var myVar;function getTemperature(){var xhttp=new XMLHttpRequest(); xhttp.open('GET', 'http://172.20.10.6/selection/update', true); xhttp.send();}function StartInterval(){myVar=setInterval(getTemperature(), 4000);}function StopInterval(){clearInterval(myVar);}</script> <body> <div class='jumbotron text-center'> <h1>Please select one</h1> </div><div class='d-flex justify-content-center'> <div class='btn-group '>  <form action='/selection/action' method='post'> <button class='btn btn-lg btn-primary' name='btn' value='temp' onclick='StartInterval()'>Temperature</button> <button class='btn btn-lg btn-primary' name='btn' value='msgbtn' onclick='StopInterval()' >Message</button> <button class='btn btn-lg btn-primary' name='btn' value='out' onclick='StopInterval()'>Sign out</button> </form> </div></div></body></html>";

void loginHtml() {
	if (radiomodule->isrec())
		radiomodule->stopRecieving();
	if (usernamecheck && passwordcheck) {
		server.sendHeader("Location", "/selection");
		server.send(303);
	}
	else {
		String page = "<!DOCTYPE html><html> <head> <title>Login - WebServer</title> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'> <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css'> <link rel='stylesheet' href='https://www.epndata.com/iot18/demo/css/style.css'> </head> <body class='text-center'> <form action='/login' method='post' class='form-signin'> <h1 class='h3 mb-3 font-weight-normal'>Sign in</h1> <label for='inputEmail' class='sr-only'>Username</label> <input type='username' name='inputEmail' id='inputEmail' class='form-control' placeholder='Username' required autofocus> <label for='inputPassword' class='sr-only'>Password</label> <input type='password' name='inputPassword' id='inputPassword' class='form-control' placeholder='Password' required> <button class='btn btn-lg btn-primary btn-block' type='submit'>Sign in</button> <p class='mt-4 mb-3 text-muted'>&copy; 2019</p></form> </body></html>";
		server.send(200, "text/html", page);
	}
}


void selectionPage() {
	if (!usernamecheck && !passwordcheck) {
		server.sendHeader("Location", "/");
		server.send(303);
	}
	else {
		Serial.println("sending selection site to server");
		String fullpage = selectionHtml;
		if (showmsg)
			fullpage = fullpage + MessageHtml;
		if (showtemps) {
			//fullpage = fullpage + "<div class='d-flex justify-content-center'><div class='p-2'>Temp recieved from other device :</div><div class='p-2' id='temptext'>  " + radiomodule->getTemps() + "</div></div><div class='d-flex justify-content-center'> <button id='temp' class='btn btn-info' onclick='myVar=setInterval(getTemperature, 3000)'>Update Temps</button></div>";
			fullpage = fullpage + "<div class='d-flex justify-content-center'><div class='p-2'>Temp recieved from other device :</div><div class='p-2' id='temptext'></div></div><div class='d-flex justify-content-center'> <button id='temp' class='btn btn-info' onclick='myVar=setInterval(getTemperature, 3000)'>Update Temps</button></div>";
		}
		server.send(200, "text/html", fullpage);
	}
	

}

void handleNotFound() {
	String msg = "File not Found\n\n";
	msg += "URI: ";
	msg += server.uri();
	msg += (server.method() == HTTP_GET) ? "GET" : "POST";
	msg += "\nArguments: ";
	msg += server.args();
	msg += "\n";
	for (uint8_t i = 0; i < server.args(); i++) 
		msg += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	server.send(404, "text/plain", msg);
}
