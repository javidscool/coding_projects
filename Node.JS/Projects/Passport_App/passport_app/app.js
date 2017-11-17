var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

const User = require("./models/user");

var index = require('./routes/index');
const authRoutes = require("./routes/auth-routes");

const session = require("express-session");
const bcrypt = require("bcrypt");
const passport = require("passport");
const LocalStrategy = require("passport-local").Strategy;
const FbStrategy = require('passport-facebook').Strategy;
const GoogleStrategy = require("passport-google-oauth").OAuth2Strategy;

const flash = require("connect-flash");

// Mongoose configuration
const mongoose = require("mongoose");
mongoose.connect("mongodb://localhost/passport-local");

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

//configure the session
app.use(session({
	secret: "our-passport-local-strategy-app",
	resave: true,
	saveUninitialized: true
}));

//put these three functions before passport.initialize() so that passport can use it
passport.serializeUser((user, cb) => {
	cb(null, user._id);
});

passport.deserializeUser((id, cb) => {
	User.findOne({ "_id": id }, (err, user) => {
		if (err) { return cb(err); }
		cb(null, user);
	});
});

app.use(flash());
passport.use(new LocalStrategy({
		passReqToCallback: true
	}, (req, username, password, next) => {
	User.findOne({ username }, (err, user) => {
		if (err) {
			return next(err);
		}
		if (!user) {
			return next(null, false, { message: "Incorrect username" });
		}
		if (!bcrypt.compareSync(password, user.password)) {
			return next(null, false, { message: "Incorrect password" });
		}

		return next(null, user);
	});
}));

passport.use(new FbStrategy({
	clientID: "1195452223887712",
	clientSecret: "a2972aefbf54a654d8fb8f34f9b9517a",
	callbackURL: "/auth/facebook/callback"
	}, (accessToken, refreshToken, profile, done) => {
		User.findOne({ facebookID: profile.id }, (err, user) => {
			if (err) {
				return done(err);
			}
			//this is if we already have a user with this FB profile
			if (user) {
				return done(null, user);
			}

			const newUser = new User({
				facebookID: profile.id
			});

			newUser.save((err) => {
				if (err) {
					return done(err);
				}
				done(null, newUser);
			});
	});
}));

passport.use(new GoogleStrategy({
	clientID: "361870301481-4ueb3s0s0viqdf6iuarccdujov8m70se.apps.googleusercontent.com",
	clientSecret: "Obw045UZwQlWsEhBxDSudOPn",
	callbackURL: "/auth/google/callback"
	}, (accessToken, refreshToken, profile, done) => {
		User.findOne({ googleID: profile.id }, (err, user) => {
			if (err) {
				return done(err);
			}
			//this is if we already have a user with this Google Account
			if (user) {
				return done(null, user);
			}

			const newUser = new User({
				googleID: profile.id
			});

			newUser.save((err) => {
				if (err) {
					return done(err);
				}
				done(null, newUser);
			});
	});
}));

//init passport and passport session
app.use(passport.initialize());
app.use(passport.session());

app.use('/', authRoutes);

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
