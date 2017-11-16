#!/usr/bin/env node

'use strict';

const express = require('express');
const path = require('path');
const favicon = require('serve-favicon');
const logger = require('morgan');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');
const session = require('express-session');
const FileStore = require('session-file-store')(session);

const passportSocketIo = require("passport.socketio");
var debug = require('debug')('notes:server');
const http = require('http');
const log = require('debug')('notes:server');

const FileStreamRotator = require('file-stream-rotator');

const sessionCookie = 'notes.sid';
const sessionSecret = 'keyboard mouse';
const sessionStore = new FileStore({ path: "sessions" });

//for handling uncaught exceptions
const error = require('debug')('notes:error');
process.on('uncaughtException', function(err) {
	error("I've crashed!!! - "+ (err.stack || err));
});

const index = require('./routes/index');
const users = require('./routes/users');
const notes = require('./routes/notes');

var app = express();


var port = normalizePort(process.env.PORT || '3000');
app.set('port', port);

var server = http.createServer(app);
var io = require('socket.io')(server);


io.sockets.on('connection', function(socket){    
	console.log("connection made");
});

io.use(passportSocketIo.authorize({
	cookieParser: cookieParser,
	key: sessionCookie,
	secret: sessionSecret,
	store: sessionStore
}));

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use('/vendor/bootstrap', express.static(path.join(__dirname, 'bower_components', 'bootstrap', 'dist')));
app.use('/vendor/jquery', express.static(path.join(__dirname, 'bower_components', 'jquery', 'dist')));

app.use(session({
	store: sessionStore,
	secret: sessionSecret,
	key: sessionCookie,
	resave: true,
	saveUninitialized: true
}));
users.initPassport(app);

app.use('/', index);
app.use('/users', users.router);
app.use('/notes', notes);

index.socketio(io);
notes.socketio(io);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

var accessLogStream;
if (process.env.REQUEST_LOG_FILE) {
	var logDirectory = path.dirname(process.env.REQUEST_LOG_FILE);
	
	fs.existsSync(logDirectory) || fs.mkdirSync(logDirectory);
	
	accessLogStream = FileStreamRotator.getStream({
		filename: process.env.REQUEST_LOG_FILE,
		frequency: 'daily',
		verbose: false
	});
}

app.use(logger(process.env.REQUEST_LOG_FORMAT || 'dev', {
	stream: accessLogStream ? accessLogStream : process.stdout
}));

//for uncaught exceptions
if (app.get('env') === 'development') {
	app.use(function(err, req, res, next) {
		// util.log(err.message);
		res.status(err.status || 500);
		error((err.status || 500) +' '+ error.message);
		res.render('error', {
			message: err.message,
			error: err
		});
	});
}

app.use(function(err, req, res, next) {
	// util.log(err.message);
	res.status(err.status || 500);
	error((err.status || 500) +' '+ error.message);
	res.render('error', {
		message: err.message,
		error: {}
	});
});

module.exports = app;


/**
 * Listen on provided port, on all network interfaces.
 */

server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

/**
 * Normalize a port into a number, string, or false.
 */

function normalizePort(val) {
	var port = parseInt(val, 10);

	if (isNaN(port)) {
		// named pipe
		return val;
	}

	if (port >= 0) {
		// port number
		return port;
	}

	return false;
}

/**
 * Event listener for HTTP server "error" event.
 */

function onError(error) {
	if (error.syscall !== 'listen') {
		throw error;
	}

	var bind = typeof port === 'string'
		? 'Pipe ' + port
		: 'Port ' + port;

	// handle specific listen errors with friendly messages
	switch (error.code) {
		case 'EACCES':
			console.error(bind + ' requires elevated privileges');
			process.exit(1);
		break;
		case 'EADDRINUSE':
			console.error(bind + ' is already in use');
			process.exit(1);
		break;
		default:
			throw error;
	}
}

/**
 * Event listener for HTTP server "listening" event.
 */

function onListening() {
	var addr = server.address();
	var bind = typeof addr === 'string'
		? 'pipe ' + addr
		: 'port ' + addr.port;
	debug('Listening on ' + bind);
}