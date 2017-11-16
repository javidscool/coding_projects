var express = require('express');
var router = express.Router();

var multer  = require('multer');
const Picture = require('../models/pictures');

/* GET home page. */
router.get('/', function(req, res, next) {
	//find all pictures
	Picture.find((err, pictures) => {
		res.render('index', {pictures})
	});
});

// Route to upload from project base path
var upload = multer({ dest: './public/uploads/' });

router.post('/upload', upload.single('photo'), function(req, res){

	pic = new Picture({
		name: req.body.name,
		pic_path: `/uploads/${req.file.filename}`,
		pic_name: req.file.originalname
	});

	pic.save((err) => {
		res.redirect('/');
	});
});

module.exports = router;
