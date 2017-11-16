const express = require("express");
const authRoutes = express.Router();

const passport = require("passport");
const ensureLogin = require("connect-ensure-login");

// User model
const User = require("../models/user");

// Bcrypt to encrypt passwords
const bcrypt = require("bcrypt");
const bcryptSalt = 10;

authRoutes.get("/", (req, res, next) => {
	res.render("index", {title: "Main Page"});
});

authRoutes.get("/login", (req, res, next) => {
	res.render("auth/login", { "message": req.flash("error") });
});

authRoutes.post("/login", passport.authenticate("local", {
	successRedirect: "/",
	failureRedirect: "/login",
	//this is to use flash for error messages
	failureFlash: true,
	passReqToCallback: true
}));

authRoutes.get("/logout", (req, res) => {
	req.logout();
	res.redirect("/login");
});

authRoutes.get("/signup", (req, res, next) => {
	res.render("auth/signup");
});

authRoutes.post("/signup", (req, res, next) => {
	const username = req.body.username;
	const password = req.body.password;

	//if user didn't enter anything in either or both fields
	if (username === "" || password === "") {
		res.render("auth/signup", { message: "Indicate username and password" });
		return;
	}

	User.findOne({ username }, "username", (err, user) => {
		//if user already exists
		if (user !== null) {
			res.render("auth/signup", { message: "The username already exists" });
			return;
		}

		const salt = bcrypt.genSaltSync(bcryptSalt);
		const hashPass = bcrypt.hashSync(password, salt);

		//else create new user and save
		const newUser = new User({
			username,
			password: hashPass
		});

		newUser.save((err) => {
			if (err) {
				res.render("auth/signup", { message: "Something went wrong" });
			} else {
				res.redirect("/");
			}
		});
	});
});

authRoutes.get("/auth/facebook", passport.authenticate("facebook"));

authRoutes.get("/auth/facebook/callback", passport.authenticate("facebook", {
	successRedirect: "/private-page",
	failureRedirect: "/"
}));

authRoutes.get("/auth/google", passport.authenticate("google", {
	scope: ["https://www.googleapis.com/auth/plus.login",
            "https://www.googleapis.com/auth/plus.profile.emails.read"]
}));

authRoutes.get("/auth/google/callback", passport.authenticate("google", {
	failureRedirect: "/",
	successRedirect: "/private-page"
}));

//make sure the ensureLoggedIn() routes are BELOW the rest of the routes
authRoutes.get("/private-page", ensureLogin.ensureLoggedIn(), (req, res) => {
	res.render("private", { user: req.user });
});

module.exports = authRoutes;