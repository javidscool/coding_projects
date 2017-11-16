const express = require("express");
const profileController = express.Router();

// User model
const User = require("../models/user");
const Tweet = require("../models/tweet");

// Moment to format dates
const moment = require("moment");

var isFollowing;

profileController.get("/:username", (req, res, next) => {
	
	User.findOne({ username: req.params.username }, "_id username").exec((err, user) => {
		if (req.session.currentUser) {
			isFollowing = req.session.currentUser.following.indexOf(user._id.toString()) > -1;
		}

		Tweet.find({ "user_name": user.username }, "tweet created_at")
		.sort({ created_at: -1 })
		.exec((err, tweets) => {
			res.render("profile/show", {
				username: user.username,
				tweets,
				moment,
				session: req.session.currentUser,
				button_text: isFollowing ? "Unfollow" : "Follow"
			});
		});
	});
});

profileController.post("/:username/follow", (req, res) => {
	User.findOne({ "username": req.params.username }, "_id").exec((err, follow) => {
		if (err) {
			res.redirect("/profile/" + req.params.username);
			return;
		}

		User.findOne({ "username": req.session.currentUser.username }).exec((err, currentUser) => {
			var followingIndex = currentUser.following.indexOf(follow._id);

			//if we're following, unfollow, else push to follow list
			if (followingIndex > -1) {
				currentUser.following.splice(followingIndex, 1)
			} else {
				currentUser.following.push(follow._id);
			}

			currentUser.save((err) => {
				req.session.currentUser = currentUser;
				res.redirect("/profile/" + req.params.username);
			});
		});
	});
});

profileController.get("/:username/timeline", (req, res) => {
	//first add our Twitter account to the accounts we need to get tweets from
	const currentUser = req.session.currentUser;
	currentUser.following.push(currentUser._id);

	//get all tweets from accounts we follow
	//timeline below contains all the tweets in our timeline
	Tweet.find({ user_id: { $in: currentUser.following } })
    .sort({ created_at: -1 })
    .exec((err, timeline) => {
		res.render("profile/timeline", {
			username: currentUser.username,
			timeline,
			moment
		});
	});
});

//if have to have logged in in order to follow other accounts
profileController.use((req, res, next) => {
	if (req.session.currentUser) { next(); }
	else { res.redirect("/login"); }
});

module.exports = profileController;