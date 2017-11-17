const User = require("../models/user");
const Tweet = require("../models/tweet");

const express = require("express");
const tweetsController = express.Router();
const moment = require("moment");

tweetsController.get("/new", (req, res, next) => {
	res.render("tweets/new", 
		{ username: req.session.currentUser.username }
	);
});

tweetsController.post("/", (req, res, next) => {
	const user  = req.session.currentUser;

	User.findOne({ username: user.username }).exec((err, user) => {
		if (err) { return; }

		const newTweet = Tweet({
			user_id: user._id,
			user_name: user.username,
			tweet: req.body.tweetText
		});

		newTweet.save((err) => {
			if (err) {
				res.render("tweets/new", 
				{
					username: user.username, 
					errorMessage: err.errors.tweet.message
				});
			} else {
				res.redirect("/tweets");
			}
		});
	});
});

tweetsController.get("/", (req, res, next) => {
	User.findOne({ username: req.session.currentUser.username }, "_id username").exec((err, user) => {
		if (!user) {
			res.redirect("/login");
		}

		//sort be descending creation date
		//we're also passing the moment function to be used in the ejs
		Tweet.find({ "user_name": user.username }, "tweet created_at")
		.sort({ created_at: -1 })
		.exec((err, tweets) => {
			console.log("tweets");
			res.render("profile/show", {
				tweets,
				moment,
				username: user.username,
				session: req.session.currentUser
			});
		});
	});
});

//if have to have logged in in order to see our tweets
tweetsController.use((req, res, next) => {
	if (req.session.currentUser) { next(); }
	else { res.redirect("/login"); }
});

module.exports = tweetsController;