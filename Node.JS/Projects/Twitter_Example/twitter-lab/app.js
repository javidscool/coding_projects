var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var index = require('./routes/index');
var users = require('./routes/users');

var session = require('express-session');

const mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/twitter-lab-development');

const expressLayouts = require('express-ejs-layouts');

const authController = require('./routes/authController');
const tweetsController = require("./routes/tweetsController");
const timelineController = require("./routes/timelineController");
const profileController = require("./routes/profileController");

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// ...other code
app.use(expressLayouts);
app.set("layout", "layouts/main-layout");

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use(session({
	secret: 'keyboard cat',
	resave: false,
	saveUninitialized: true,
	cookie: { secure: false }
}))

//app.use('/', index);
app.use("/", authController);
app.use("/users", users);
app.use("/tweets", tweetsController);
app.use("/timeline", timelineController);
app.use("/profile", profileController);

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

module.exports = app;
