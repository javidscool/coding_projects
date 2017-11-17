'use strict';

const express = require('express');
const path = require('path');
const favicon = require('serve-favicon');
const logger = require('morgan');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');
const session = require('express-session');
const FileStore = require('session-file-store')(session);

const FileStreamRotator = require('file-stream-rotator');

//for handling uncaught exceptions
const error = require('debug')('notes:error');
process.on('uncaughtException', function(err) {
	error("I've crashed!!! - "+ (err.stack || err));
});

const index = require('./routes/index');
const users = require('./routes/users');
const notes = require('./routes/notes');

var app = express();


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
	store: new FileStore({ path: "sessions" }),
	secret: 'keyboard mouse',
	resave: true,
	saveUninitialized: true
}));
users.initPassport(app);

app.use('/', index);
app.use('/users', users.router);
app.use('/notes', notes);

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
